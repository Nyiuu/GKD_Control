#include "device/rc_controller.hpp"

#include "io.hpp"
#include "serial_interface.hpp"
#include "types.hpp"

namespace Device
{

    Rc_Controller::Rc_Controller(const std::string &serial_name) : serial_name(serial_name) {
    }

    void Rc_Controller::init(const std::shared_ptr<Robot::Robot_set> &robot) {
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
        float vx = 0, vy = 0;
        float speed = .5;
        if (pkg.key & 0x1)
            vx++;
        if (pkg.key & 0x2)
            vx--;
        if (pkg.key & 0x4)
            vy--;
        if (pkg.key & 0x8)
            vy++;
        
        robot_set->vx_set = vx * speed;
        robot_set->vy_set = vy * speed;
        if (pkg.key)
        LOG_INFO("key : %d\n", pkg.key);

        static std::vector<int> key_status(8);

        if (pkg.key & 0x40)
        {
            if (key_status[0] == 0)
                robot_set->wz_set = 1 - robot_set->wz_set;
            key_status[0] = 1;
        }
        else
        {
            key_status[0] = 0;
        }

        if (pkg.key & 0x80)
        {
            if (key_status[1] == 0)
                robot_set->friction_open = !robot_set->friction_open;
            key_status[1] = 1;
        }
        else
        {
            key_status[1] = 0;
        }
        
        static bool skip_control = false;
        if (pkg.mouse_r)
        {
            if (key_status[2] == 0)
                skip_control = !skip_control;
            key_status[2] = 1;
        }
        else
        {
            key_status[2] = 0;
        }

            

        if (pkg.mouse_l)
        {
            robot_set->shoot_open = 1;
        }
        else
        {
            robot_set->shoot_open = 0;
        }

        if (skip_control)
        return;
        
        robot_set->gimbalT_1_yaw_set += pkg.mouse_x / 2000.;
        robot_set->gimbalT_2_yaw_set += pkg.mouse_x / 2000.;
        // robot_set->gimbalT_1_pitch_set += pkg.mouse_y / 40;
        static bool use_key = false;
        if (pkg.key & 0xff)
        {
            use_key = true;
        }

        if (use_key)
            return;


        // LOG_INFO("rc controller ch1 %d %d %d %d\n", pkg.s1, pkg.s2, pkg.ch1, pkg.ch3);
        if (pkg.s1 == 2 && pkg.s2 == 2 && pkg.ch4 == -660) {
            inited = true;
        }

        // auto-aim, disable control
        if (pkg.s1 == 2)
            return;

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
                robot_set->wz_set = 1.0;
            else
                robot_set->wz_set = 0;

            if (pkg.ch4 == 660)
                robot_set->shoot_open = 1;
            else
                robot_set->shoot_open = 0;

            if (pkg.s2 == 1)
                robot_set->friction_open = true;
            else
                robot_set->friction_open = false;
        }
        update_time();
    }
}  // namespace Device
