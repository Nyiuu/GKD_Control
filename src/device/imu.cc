#include "device/imu.hpp"

#include "io.hpp"
#include "serial_interface.hpp"
#include "user_lib.hpp"

namespace Device
{
    IMU::IMU(const std::string &serial_name) : serial_name(serial_name) {
    }

    void IMU::enable() {
        auto serial_interface = IO::io<SERIAL>[serial_name];
        if (serial_interface == nullptr) {
            LOG_ERR("IMU Error: no serial named %s\n", serial_name.c_str());
            return;
        }
        serial_interface->register_callback<Types::ReceivePacket_IMU>(
            [&](const Types::ReceivePacket_IMU &rp) { unpack(rp); });
        }

    void IMU::unpack(const Types::ReceivePacket_IMU &pkg) {
        yaw = UserLib::rad_format(pkg.yaw * (M_PIf / 180));
        pitch = -UserLib::rad_format(pkg.pitch * (M_PIf / 180));
        roll = UserLib::rad_format(pkg.roll * (M_PIf / 180));
        yaw_rate = pkg.yaw_v * (M_PIf / 180) / 1000;
        pitch_rate = pkg.pitch_v * (M_PIf / 180) / 1000;
        roll_rate = pkg.roll_v * (M_PIf / 180) / 1000;
        // if (serial_name.compare("/dev/IMU_HERO") == 0)
        //     LOG_INFO("imu %.6f %.6f %.6f\n", pkg.yaw, pkg.pitch, pkg.yaw_v);
        update_time();
    }
}  // namespace Device
