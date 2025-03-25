#include "device/super_cap.hpp"

#include "macro_helpers.hpp"
#include "power_controller.hpp"

namespace Device
{
    void Super_Cap::init(
        const std::string& can_name,
        const std::shared_ptr<Robot::Robot_set>& robot) {
        robot_set = robot;
        can = IO::io<CAN>[can_name];
        can->register_callback_key(
            0x51, std::bind(&Super_Cap::unpack, this, std::placeholders::_1));
    }

    void Super_Cap::unpack(const can_frame& frame) {
        static int delta = 0;
        delta++;
        uint16_t robot_level = robot_set->referee_info.game_robot_status_data.robot_level;
        uint16_t power_limit = MUXDEF(
            CONFIG_HERO,
            Power::HeroChassisPowerLimit_HP_FIRST[robot_level],
            MUXDEF(CONFIG_INFANTRY, Power::InfantryChassisPowerLimit_HP_FIRST[robot_level], 100U));
        if (delta >= 500) {
            set(true, power_limit);
            delta = 0;
        }

        std::memcpy(&robot_set->super_cap_info, frame.data, 8);

        LOG_INFO(
            "errorCode %d\tchassisPower %f\tchassisPowerlimit %d\tcapEnergy %d\n",
            robot_set->super_cap_info.errorCode,
            robot_set->super_cap_info.chassisPower,
            (int)robot_set->super_cap_info.chassisPowerlimit,
            (int)robot_set->super_cap_info.capEnergy);
    }

    void Super_Cap::set(bool enable, uint16_t power_limit) {
        can_frame send{};
        uint16_t chassis_power_buffer =
            robot_set->referee_info.power_heat_data.chassis_power_buffer;
        send.can_id = 0x061;
        send.can_dlc = 8;
        if (enable)
            send.data[0] = 0x01;
        send.data[1] = power_limit & 0xff;
        send.data[2] = power_limit >> 8;
        send.data[3] = chassis_power_buffer & 0xff;
        send.data[4] = chassis_power_buffer >> 8;

        can->send(send);
    }
}  // namespace Device
