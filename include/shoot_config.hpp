#pragma once
#include <vector>

#include "dji_motor.hpp"
#include "pid_controller.hpp"

namespace Shoot
{
    struct ShootConfig
    {
        Hardware::DJIMotorConfig left_friction_motor_config;
        Hardware::DJIMotorConfig right_friction_motor_config;
        Hardware::DJIMotorConfig trigger_motor_config;
        Pid::PidConfig friction_speed_pid_config;
        Pid::PidConfig trigger_speed_pid_config;
        int8_t trigger_dir;
        int gimbal_id;
    };

}  // namespace Shoot
