// NOTE: lagacy from rm_control (needs update every YEAR!)
#pragma once
#include <stdint.h>

namespace Referee
{
    struct GameStatus
    {
        uint8_t game_type;
        uint8_t game_progress;
        uint16_t stage_remain_time;
        uint64_t sync_time_stamp;
        uint8_t IN_BATTLE = 4;
    };

    struct BulletAllowance
    {
        uint16_t bullet_allowance_num_17_mm;
        uint16_t bullet_allowance_num_42_mm;
        uint16_t coin_remaining_num;
    };

    struct GameRobotStatus
    {
        uint8_t robot_id;
        uint8_t robot_level;
        uint16_t remain_hp;
        uint16_t max_hp;
        uint16_t shooter_cooling_rate;
        uint16_t shooter_cooling_limit;
        uint16_t chassis_power_limit;
        uint8_t mains_power_gimbal_output;
        uint8_t mains_power_chassis_output;
        uint8_t mains_power_shooter_output;
    };
    struct PowerHeatData
    {
        uint16_t chassis_power_buffer;
        uint16_t shooter_id_1_17_mm_cooling_heat;
        uint16_t shooter_id_2_17_mm_cooling_heat;
        uint16_t shooter_id_1_42_mm_cooling_heat;
    };
}  // namespace Referee
