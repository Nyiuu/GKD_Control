#include "device/rc_controller.hpp"

#include "io.hpp"
#include "serial_interface.hpp"
#include "types.hpp"

namespace Device
{

    Rc_Controller::Rc_Controller(const std::string &serial_name) : serial_name(serial_name) {
    }

    void Rc_Controller::enable(const std::shared_ptr<Robot::Robot_set> &robot) {
        robot_set = robot;
        auto serial_interface = IO::io<SERIAL>[serial_name];
        if (serial_interface == nullptr) {
            LOG_ERR("RC_CONTROLLER Error: no serial named %s\n", serial_name.c_str());
            return;
        }
        serial_interface->register_callback<Types::ReceivePacket_RC_CTRL>(
            [&](const Types::ReceivePacket_RC_CTRL &rp) { unpack(rp); });
    }

    void Rc_Controller::unpack(const Types::ReceivePacket_RC_CTRL &pkg) {
        LOG_INFO("rc controller ch1 %d\n", pkg.ch2);
        robot_set->vx_set = ((float)pkg.ch3 / 660) * 3;
        robot_set->vy_set = ((float)pkg.ch2 / 660) * 3;
        robot_set->gimbalT_1_yaw_set += ((float)pkg.ch0 / 660) / 200;
        robot_set->gimbalT_1_pitch_set = ((float)pkg.ch1 / 660) * 0.3;
        if (pkg.s1 == 3)
            robot_set->wz_set = 0.5;
        else
            robot_set->wz_set = 0;
        update_time();
    }
}  // namespace Device
