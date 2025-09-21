#pragma once

#include <memory>

#include "device/deviece_base.hpp"
#include "robot.hpp"
#include "types.hpp"
#include "logger.hpp"

#define KEY_D 0x1
#define KEY_A 0x2
#define KEY_S 0x4
#define KEY_W 0x8
#define KEY_R 0x40
#define KEY_F 0x80
#define KEY_PRESS 0xff

#define ROLL_DOWN_MAX 660
#define ROLL_UP_MAX -660
#define S1_UP 1
#define S1_DOWN 2
#define S1_MIDDLE 3
#define S2_UP 1
#define S2_DOWN 2
#define S2_MIDDLE 3

#define RC_SCALE  660.0f
#define CHASSIS_SPEED_SCALE             3.0f
#define GIMBAL_YAW_SENSITIVITY          (1.0f / 200.0f)
#define GIMBAL_PITCH_SENSITIVITY        0.3f

#define SHOOT_PERMISSION_NONE     0  // 无射击权限  
#define SHOOT_PERMISSION_GIMBAL1  1  // 云台1射击权限  
#define SHOOT_PERMISSION_GIMBAL2  2  // 云台2射击权限    
#define SHOOT_PERMISSION_BOTH     3  // 双云台射击权限

namespace Device
{
    class Rc_Controller : public DeviceBase
    {
       public:
        explicit Rc_Controller(const std::string& serial_name);

        bool inited = 0;
        fp32 yaw = 0;
        fp32 pitch = 0;
        fp32 roll = 0;
        fp32 yaw_rate = 0;
        fp32 pitch_rate = 0;
        fp32 roll_rate = 0;

        void init(const std::shared_ptr<Robot::Robot_set>& robot);
        void unpack(const Types::ReceivePacket_RC_CTRL& pkg);

       private:
        std::string serial_name;
        std::shared_ptr<Robot::Robot_set> robot_set;
        int delta;
    };
}  // namespace Device
