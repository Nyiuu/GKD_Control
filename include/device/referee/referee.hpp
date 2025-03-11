// NOTE: lagacy from rm_control (needs update every YEAR!)
#pragma once
#include "device/deviece_base.hpp"
#include "referee_base.hpp"

namespace Device
{
    class Dji_referee : public DeviceBase
    {
       public:
        Dji_referee() {
            base_.initSerial();
        };
        void task();
        void read();
        void clearRxBuffer() {
            rx_buffer_.clear();
            rx_len_ = 0;
        }
        Base base_;
        std::vector<uint8_t> rx_buffer_;
        int rx_len_;

       private:
        int unpack(uint8_t *rx_data);
        void publishCapacityData();

        const int k_frame_length_ = 128, k_header_length_ = 5, k_cmd_id_length_ = 2, k_tail_length_ = 2;
        const int k_unpack_buffer_length_ = 256;
        uint8_t unpack_buffer_[256]{};
    };
}  // namespace Device
