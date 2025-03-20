#pragma once

#include "device/deviece_base.hpp"
#include "memory"
#include "robot.hpp"
#include "types.hpp"

namespace Device
{
    class IMU : public DeviceBase
    {
       public:
        explicit IMU(const std::string& serial_name);

        fp32 yaw = 0;
        fp32 pitch = 0;
        fp32 roll = 0;
        fp32 yaw_rate = 0;
        fp32 pitch_rate = 0;
        fp32 roll_rate = 0;

        void enable();
        void unpack(const Types::ReceivePacket_IMU& pkg);

       private:
        std::string serial_name;
        std::shared_ptr<Robot::Robot_set> robot_set;
    };
    class C_IMU : public DeviceBase
    {
    public:
      explicit C_IMU(const std::string& serial_name);

      fp32 yaw = 0;
      fp32 pitch = 0;
      fp32 roll = 0;
      fp32 yaw_rate = 0;
      fp32 pitch_rate = 0;
      fp32 roll_rate = 0;

      void enable();
      void unpack(const Types::C_IMU_PKG& pkg);

    private:
      std::string serial_name;
      std::shared_ptr<Robot::Robot_set> robot_set;
    };


}  // namespace Device
