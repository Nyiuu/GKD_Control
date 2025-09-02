#pragma once

#include <memory>

#include "device/deviece_base.hpp"
#include "robot.hpp"
#include "types.hpp"

#define KEY_D 0x1
#define KEY_A 0x2
#define KEY_S 0x4
#define KEY_W 0x8
#define KEY_R 0x40
#define KEY_F 0x80


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
    };
}  // namespace Device
