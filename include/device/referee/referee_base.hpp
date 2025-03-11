// NOTE: lagacy from rm_control (needs update every YEAR!)
#pragma once
#include <stdint.h>

#include <thread>

#include "protocol.hpp"
#include "serial/serial.h"

namespace Device
{
    class Base
    {
       public:
        serial::Serial serial_;

        int client_id_ = 0;  // recipient's id
        int robot_id_ = 0;   // recent  robot's id
        int capacity_recent_mode_, capacity_expect_mode_;
        std::string robot_color_;
        bool referee_data_is_online_ = false;

        void initSerial();
        uint8_t getCRC8CheckSum(unsigned char *pch_message, unsigned int dw_length, unsigned char uc_crc_8);
        uint32_t verifyCRC8CheckSum(unsigned char *pch_message, unsigned int dw_length);
        void appendCRC8CheckSum(unsigned char *pch_message, unsigned int dw_length);
        uint16_t getCRC16CheckSum(uint8_t *pch_message, uint32_t dw_length, uint16_t w_crc);
        uint32_t verifyCRC16CheckSum(uint8_t *pch_message, uint32_t dw_length);
        void appendCRC16CheckSum(uint8_t *pch_message, uint32_t dw_length);
    };
}  // namespace Device
