#include "serial_interface.hpp"

#include "user_lib.hpp"

namespace IO
{
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
        else if (pkg_id == 3) {
            uint8_t frame_type;
            read(&frame_type, 1); 
            
            uint8_t rx_buffer[19]; 
            
            if (frame_type == 0x02) { 
                size_t bytes_read = read(rx_buffer, 15);
                
                if (bytes_read == 15 && rx_buffer[14] == 0x0A) { 
                    float gyro_x, gyro_y, gyro_z;
                    memcpy(&gyro_x, &rx_buffer[0], 4);
                    memcpy(&gyro_y, &rx_buffer[4], 4);
                    memcpy(&gyro_z, &rx_buffer[8], 4);
                    
                    imu_pkg.roll_v = gyro_x;
                    imu_pkg.pitch_v = gyro_y;
                    imu_pkg.yaw_v = gyro_z;
                    
                    callback(imu_pkg);
                }
            } 
            else if (frame_type == 0x03) { 
                size_t bytes_read = read(rx_buffer, 15);
                
                if (bytes_read == 15 && rx_buffer[14] == 0x0A) { 
                    float roll, pitch, yaw;
                    memcpy(&roll, &rx_buffer[0], 4);
                    memcpy(&pitch, &rx_buffer[4], 4);
                    memcpy(&yaw, &rx_buffer[8], 4);
                    
                    imu_pkg.roll = roll;
                    imu_pkg.pitch = pitch;
                    imu_pkg.yaw = yaw;
                    
                    callback(imu_pkg);
                }
            }

        }
        return 0;
    }

    void Serial_interface::task() {
        while (true) {
            try {
                if (isOpen()) {
                    read((uint8_t *)&header, 2);
                    if (header == 0xAA55) {
                        read((uint8_t *)&header, 1);
                        unpack(header);
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
