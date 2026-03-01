#include "serial_interface.hpp"

#include "user_lib.hpp"
#include <array>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstring>

namespace IO
{
    namespace
    {
        constexpr uint8_t kLegacyHeader0 = 0x55;
        constexpr uint8_t kLegacyHeader1 = 0xAA;
        constexpr uint8_t kCh10xHeader0 = 0x5A;
        constexpr uint8_t kCh10xHeader1 = 0xA5;
        constexpr uint8_t kCh10xTagFloat = 0x91;

        struct Ch10xImu91
        {
            uint8_t tag;
            uint16_t pps_sync_stamp;
            int8_t temperature;
            float air_pressure;
            uint32_t system_time;
            float acc_b[3];
            float gyr_b[3];
            float mag_b[3];
            float roll;
            float pitch;
            float yaw;
            float quat[4];
        } __attribute__((packed));

        static_assert(sizeof(Ch10xImu91) == 76, "CH10X IMU91 payload size mismatch");

        uint16_t crc16_ccitt(uint16_t crc, const uint8_t *src, size_t length) {
            for (size_t j = 0; j < length; ++j) {
                crc ^= static_cast<uint16_t>(src[j]) << 8;
                for (int i = 0; i < 8; ++i) {
                    if (crc & 0x8000) {
                        crc = static_cast<uint16_t>((crc << 1) ^ 0x1021);
                    } else {
                        crc = static_cast<uint16_t>(crc << 1);
                    }
                }
            }
            return crc;
        }

        bool parse_ch10x_payload(const uint8_t *payload, size_t payload_len, Types::ReceivePacket_IMU *out) {
            if (payload_len < sizeof(Ch10xImu91)) {
                return false;
            }

            Ch10xImu91 pkt{};
            memcpy(&pkt, payload, sizeof(Ch10xImu91));
            if (pkt.tag != kCh10xTagFloat) {
                return false;
            }

            // CH10X outputs roll/pitch/yaw in degrees and gyro in deg/s.
            // Convert gyro to the legacy unit (0.001 deg/s) expected by imu.cc.
            constexpr float kAngleScale = 1.0f;
            constexpr float kRateScale = 1000.0f;

            // If the CH10X installation coordinate differs from the legacy IMU,
            // adjust these signs to match the previous coordinate convention.
            constexpr float kYawSign = 1.0f;
            constexpr float kPitchSign = 1.0f;
            constexpr float kRollSign = 1.0f;

            out->yaw = kYawSign * pkt.yaw * kAngleScale;
            out->pitch = kPitchSign * pkt.pitch * kAngleScale;
            out->roll = kRollSign * pkt.roll * kAngleScale;

            out->yaw_v = kYawSign * pkt.gyr_b[2] * kRateScale;
            out->pitch_v = kPitchSign * pkt.gyr_b[1] * kRateScale;
            out->roll_v = kRollSign * pkt.gyr_b[0] * kRateScale;
            return true;
        }
    }  // namespace

    Serial_interface::Serial_interface(std::string port_name, int baudrate, int simple_timeout)
        : serial::Serial(port_name, baudrate, serial::Timeout::simpleTimeout(simple_timeout)),
          name(port_name) {
    }

    Serial_interface::~Serial_interface() = default;

    inline void Serial_interface::enumerate_ports() {
        std::vector<serial::PortInfo> devices_found = serial::list_ports();
        auto iter = devices_found.begin();

        while (iter != devices_found.end()) {
            serial::PortInfo device = *iter++;
            LOG_INFO("(%s, %s, %s)\n", device.port.c_str(), device.description.c_str(), device.hardware_id.c_str());
        }
    }

    inline int Serial_interface::unpack(uint8_t pkg_id) {
        if (pkg_id == 1) {
            memcpy(buffer, read(sizeof(Types::ReceivePacket_IMU)).c_str(), sizeof(Types::ReceivePacket_IMU));
            UserLib::fromVector(buffer, &imu_pkg);
            callback(imu_pkg);
        } else if (pkg_id == 2) {
            size_t pkg_size = sizeof(Types::ReceivePacket_RC_CTRL);
            memcpy(buffer, read(pkg_size).c_str(), pkg_size);
            
            // printf("Raw Buffer (Hex): \n");
            // for (size_t i = 0; i < pkg_size; ++i) {
            //     printf("%02X ", (unsigned char)buffer[i]); 
            // }
            // printf("\n");
            UserLib::fromVector(buffer, &rc_pkg);
            // printf("mouse_x: %d | mouse_y: %d | mouse_z: %d | mouse_l: %d | mouse_r: %d\n", rc_pkg.mouse_x, rc_pkg.mouse_y, rc_pkg.mouse_z, rc_pkg.mouse_l, rc_pkg.mouse_r);
            callback(rc_pkg);
        } 
       
        return 0;
    }

    void Serial_interface::task() {
        while (true) {
            try {
                if (isOpen()) {
                    uint8_t head[2];
                    read(head, 2);
                    if (head[0] == kLegacyHeader0 && head[1] == kLegacyHeader1) {
                        read((uint8_t *)&header, 1);
                        unpack(header);
                    } else if (head[0] == kCh10xHeader0 && head[1] == kCh10xHeader1) {
                        uint8_t len_bytes[2];
                        uint8_t crc_bytes[2];
                        read(len_bytes, 2);
                        read(crc_bytes, 2);
                        uint16_t payload_len = static_cast<uint16_t>(len_bytes[0]) |
                                               (static_cast<uint16_t>(len_bytes[1]) << 8);
                        uint16_t frame_crc = static_cast<uint16_t>(crc_bytes[0]) |
                                             (static_cast<uint16_t>(crc_bytes[1]) << 8);

                        if (payload_len > sizeof(buffer)) {
                            // Drain oversized payload to keep stream aligned.
                            read(payload_len);
                            continue;
                        }

                        read(buffer, payload_len);
                        std::array<uint8_t, 4> header_and_len = {
                            kCh10xHeader0,
                            kCh10xHeader1,
                            len_bytes[0],
                            len_bytes[1],
                        };
                        uint16_t crc = 0;
                        crc = crc16_ccitt(crc, header_and_len.data(), header_and_len.size());
                        crc = crc16_ccitt(crc, buffer, payload_len);
                        if (crc != frame_crc) {
                            continue;
                        }

                        if (parse_ch10x_payload(buffer, payload_len, &imu_pkg)) {
                            callback(imu_pkg);
                        }
                    }
                } else {
                    enumerate_ports();
                    return;
                }
            } catch (serial::IOException &e) {
                LOG_ERR("serail offline! end program now\n");
                //exit(-1);
            }
        }
    }
}  // namespace IO
