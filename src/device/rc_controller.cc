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
        // LOG_INFO("rc controller ch1 %d %d %d %d\n", pkg.s1, pkg.s2, pkg.ch1, pkg.ch3);
        if (pkg.s1 == 2 && pkg.s2 == 2 && pkg.ch4 == -660) {
            inited = true;
        }
        if (inited) {
            robot_set->vx_set = ((float)pkg.ch3 / 660) * 3;
            robot_set->vy_set = ((float)pkg.ch2 / 660) * 3;
            if (robot_set->mode == Types::ROBOT_MODE::ROBOT_SEARCH) {
                robot_set->gimbal_sentry_yaw_set += ((float)pkg.ch0 / 660) / 200;
            } else {
                robot_set->gimbalT_1_yaw_set += ((float)pkg.ch0 / 660) / 200;
                robot_set->gimbalT_1_pitch_set = ((float)pkg.ch1 / 660) * 0.3;
                IFDEF(CONFIG_SENTRY, robot_set->gimbalT_2_yaw_set = robot_set->gimbalT_1_yaw_set;
                      robot_set->gimbalT_2_pitch_set = robot_set->gimbalT_1_pitch_set;)
            }

            if (pkg.s1 == 1)
                robot_set->wz_set = 0.5;
            else
                robot_set->wz_set = 0;

            if (pkg.ch4 == 660)
                robot_set->shoot_open = true;
            else
                robot_set->shoot_open = false;

            if (pkg.s2 == 1)
                robot_set->friction_open = true;
            else
                robot_set->friction_open = false;
        }
        update_time();
    }
}  // namespace Device
