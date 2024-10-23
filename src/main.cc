#include "robot_controller.hpp"
#include "utils.hpp"

using namespace std::chrono;

int main(int argc, char **argv) {
    Robot::Robot_ctrl robot;

    robot.load_hardware();
    robot.start_init();
    robot.init_join();
    LOG_INFO("init finished!\n");

    robot.robot_set->mode = Types::ROBOT_MODE::ROBOT_FINISH_INIT;

    robot.start();
    robot.join();

    return 0;
}
