#include "shoot.hpp"

#include "pid_controller.hpp"
#include "robot_type_config.hpp"
#include "user_lib.hpp"

namespace Shoot
{

    Shoot::Shoot(const ShootConfig& config)
        : friction_ramp(Config::FRICTION_ADD_SPEED, Config::SHOOT_CONTROL_TIME * 1e-3f),
          left_friction(config.left_friction_motor_config),
          right_friction(config.right_friction_motor_config),
          trigger(config.trigger_motor_config) {
        left_friction.setCtrl(
            Pid::PidPosition(config.friction_speed_pid_config, left_friction.data_.output_linear_velocity));
        right_friction.setCtrl(
            Pid::PidPosition(config.friction_speed_pid_config, right_friction.data_.output_linear_velocity));
        trigger.setCtrl(Pid::PidPosition(config.trigger_speed_pid_config, trigger.data_.output_angular_velocity));
    }

    void Shoot::init(const std::shared_ptr<Robot::Robot_set>& robot) {
        robot_set = robot;
    }

    void Shoot::decomposition_speed() {
        fp32 trigger_speed = 0.f;
        if (robot_set->mode == Types::ROBOT_MODE::ROBOT_NO_FORCE) {
            friction_ramp.clear();
            trigger_speed = 0.f;
        } else {
            friction_ramp.update(robot_set->friction_open ? Config::FRICTION_MAX_SPEED : 0.f);
            if (back_time) {
                back_time--;
                trigger_speed = robot_set->shoot_open ? -Config::CONTINUE_TRIGGER_SPEED : 0.f;
            } else {
                if (isJam()) {
                    jam_time++;
                }
                if (jam_time > 500) {
                    back_time = 1000;
                    jam_time = 0;
                }
                trigger_speed = robot_set->shoot_open ? Config::CONTINUE_TRIGGER_SPEED : 0.f;
            }
        }
        // friction[0].speed_set = -friction_ramp.out;
        // friction[1].speed_set = friction_ramp.out;
        // friction[2].speed_set = friction_ramp.out;
        // friction[3].speed_set = -friction_ramp.out;
        // for(auto & mot : trigger) {
        //     mot.speed_set = trigger_speed;
        // }
    }

    [[noreturn]] void Shoot::task() {
        while (true) {
            decomposition_speed();
            if (robot_set->mode == Types::ROBOT_MODE::ROBOT_NO_FORCE) {
                left_friction.set(0);
                right_friction.set(0);
                trigger.set(0);
            } else if (robot_set->friction_open) {
                left_friction.set(17);
                right_friction.set(-17);
            }
            if (robot_set->mode == Types::ROBOT_MODE::ROBOT_NO_FORCE || !robot_set->shoot_open) {
                trigger.set(0);
            } else {
                trigger.set(1);
            }
            UserLib::sleep_ms(Config::SHOOT_CONTROL_TIME);
        }
    }

    bool Shoot::isJam() {
        return trigger.motor_measure_.given_current > 4000 && trigger.motor_measure_.speed_rpm < 1;
    }
}  // namespace Shoot
