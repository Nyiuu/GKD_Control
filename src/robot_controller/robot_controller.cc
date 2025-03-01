#include "robot_controller.hpp"

#include "config.hpp"
#include "io.hpp"

namespace Robot
{
    Robot_ctrl::Robot_ctrl()
        : chassis(Config::chassis_config),
          gimbal(Config::gimbal_right_config),
          gimbal_l(Config::gimbal_left_config),
          // fixup: fix this pls
          rc_controller("/dev/IMU_BIG_YAW") {
        robot_set = std::make_shared<Robot_set>();
    }

    Robot_ctrl::~Robot_ctrl() = default;

    void Robot_ctrl::start_init() {
        // NOTE: register motors here

        // cv_controller_.init(robot_set);
        chassis.init(robot_set);
        gimbal.init(robot_set);
        gimbal_l.init(robot_set);
        gimbal_big_yaw.init(robot_set);
        rc_controller.enable();
        //  shoot.init(robot_set);

        // start DJIMotorManager thread
        Hardware::DJIMotorManager::start();
        gimbal_init_thread = std::make_unique<std::thread>([&]() { gimbal.init_task(); });
        gimbal_l_init_thread = std::make_unique<std::thread>([&]() { gimbal_l.init_task(); });
        gimbal_big_yaw_init_thread = std::make_unique<std::thread>([&]() { gimbal_big_yaw.init_task(); });
    }

    void Robot_ctrl::init_join() const {
        if (gimbal_init_thread != nullptr) {
            gimbal_init_thread->join();
        }
        if (gimbal_l_init_thread != nullptr) {
            gimbal_l_init_thread->join();
        }
        if (gimbal_big_yaw_init_thread != nullptr) {
            gimbal_big_yaw_init_thread->join();
        }
    }

    void Robot_ctrl::start() {
        // chassis_thread = std::make_unique<std::thread>(&Chassis::Chassis::task, &chassis);
        gimbal_thread = std::make_unique<std::thread>(&Gimbal::GimbalT::task, &gimbal);
        gimbal_l_thread = std::make_unique<std::thread>(&Gimbal::GimbalT::task, &gimbal_l);
        gimbal_big_yaw_thread = std::make_unique<std::thread>(&Gimbal::GimbalSentry::task, &gimbal_big_yaw);

        // shoot_thread = std::make_unique<std::thread>(&Shoot::Shoot::task, &shoot);
        // vision_thread = std::make_unique<std::thread>(&Device::Cv_controller::task, &cv_controller_);
    }

    void Robot_ctrl::join() const {
        // if (chassis_thread != nullptr) {
        //     chassis_thread->join();
        // }
        if (gimbal_thread != nullptr) {
            gimbal_thread->join();
        }
        if (gimbal_l_thread != nullptr) {
            gimbal_l_thread->join();
        }
        if (gimbal_big_yaw_thread != nullptr) {
            gimbal_big_yaw_thread->join();
        }
    }

    void Robot_ctrl::load_hardware() {
        for (auto& name : Config::CanInitList) {
            IO::io<CAN>.insert(name);
        }
        for (auto& [name, baud_rate, simple_timeout] : Config::SerialInitList) {
            IO::io<SERIAL>.insert(name, baud_rate, simple_timeout);
        }
        // for(auto & name : Config :: SocketInitList) {
        //     IO::io<SOCKET>.insert(name);
        // }
    }
};  // namespace Robot
