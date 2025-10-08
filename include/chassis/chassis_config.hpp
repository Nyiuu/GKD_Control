#pragma once

#include <array>
#include <iostream>

#include "dji_motor.hpp"
#include "io.hpp"
#include "pid_controller.hpp"

namespace Chassis
{
    struct ChassisConfig
    {
        std::array<Hardware::DJIMotorConfig, 4> wheels_config;
        Pid::PidConfig chassis_follow_gimbal_pid_config{};
        Pid::PidConfig wheel_speed_pid_config{};
        const int ControlTime{}; //控制周期
        int8_t follow_dir;
    };
}  // namespace Chassis
