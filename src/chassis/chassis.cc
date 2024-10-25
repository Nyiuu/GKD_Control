#include "chassis.hpp"

#include "user_lib.hpp"

namespace Chassis
{
    Chassis::Chassis() : chassis_angle_pid(Config::CHASSIS_FOLLOW_GIMBAL_PID_CONFIG) {
        motors.assign(4, Hardware::Motor{ Config::M3508_SPEED_PID_CONFIG });
    }

    void Chassis::init(const std::shared_ptr<Robot::Robot_set> &robot) {
        robot_set = robot;
        for (size_t i = 0; i < motors.size(); i++) {
            auto &mot = motors[i];
            Robot::hardware->register_callback<CAN2>(0x201 + i, [&mot](const auto &frame) { mot.unpack(frame); });
        }
    }

    [[noreturn]] void Chassis::task() {
        while (true) {
            update_data();
            decomposition_speed();
            if (robot_set->mode == Types::ROBOT_MODE::ROBOT_NO_FORCE) {
                for (auto &m : motors) {
                    m.give_current = 0;
                }
            } else {
                fp32 max_speed = 0.f;
                for (int i = 0; i < 4; i++) {
                    motors[i].speed_set = wheel_speed[i];
                    max_speed = std::max(max_speed, fabsf(wheel_speed[i]));
                }
                if (max_speed > max_wheel_speed) {
                    fp32 speed_rate = max_wheel_speed / max_speed;
                    for (auto &m : motors) {
                        m.speed_set *= speed_rate;
                    }
                }
                for (auto &m : motors) {
                    m.pid_ctrler.calc(m.speed, m.speed_set);
                    m.give_current = (int16_t)(m.pid_ctrler.out);
                }
            }

            Robot::hardware->send<CAN2>(Hardware::get_frame(0x200, motors));
            UserLib::sleep_ms(Config::CHASSIS_CONTROL_TIME);
        }
    }

    void Chassis::decomposition_speed() {
        if (robot_set->mode != Types::ROBOT_MODE::ROBOT_NO_FORCE) {
            fp32 sin_yaw, cos_yaw;
            sincosf(robot_set->gimbal3_yaw_relative, &sin_yaw, &cos_yaw);
            vx_set = cos_yaw * robot_set->vx_set - sin_yaw * robot_set->vy_set;
            vy_set = sin_yaw * robot_set->vx_set + cos_yaw * robot_set->vy_set;

            if (robot_set->mode == Types::ROBOT_MODE::ROBOT_FOLLOW_GIMBAL ||
                robot_set->mode == Types::ROBOT_MODE::ROBOT_SEARCH ||
                robot_set->mode == Types::ROBOT_MODE::ROBOT_IDLE) {
                chassis_angle_pid.calc(robot_set->gimbal3_yaw_relative, 0.f);
                wz_set = chassis_angle_pid.out;
            } else {
                wz_set = robot_set->wz_set;
            }
        }

        wheel_speed[0] = -vx_set + vy_set + wz_set;
        wheel_speed[1] = vx_set + vy_set + wz_set;
        wheel_speed[2] = vx_set - vy_set + wz_set;
        wheel_speed[3] = -vx_set - vy_set + wz_set;
    }

    void Chassis::update_data() {
        for (auto &m : motors) {
            m.speed = Config::CHASSIS_MOTOR_RPM_TO_VECTOR_SEN * (fp32)m.motor_measure.speed_rpm;
            m.accel = Config::CHASSIS_CONTROL_FREQUENCE * m.pid_ctrler.Dbuf;
        }
    }
}  // namespace Chassis
