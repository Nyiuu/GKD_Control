#include "referee_base.hpp"

namespace Device
{

    void Base::initSerial() {
        serial::Timeout timeout = serial::Timeout::simpleTimeout(50);
        serial_.setPort("/dev/ttyUSB0");
        serial_.setBaudrate(115200);
        serial_.setTimeout(timeout);
        if (serial_.isOpen())
            return;
        try {
            serial_.open();
        } catch (serial::IOException &e) {
            printf("Cannot open referee port\n");
        }
    }

    // CRC check
    uint8_t Base::getCRC8CheckSum(unsigned char *pch_message, unsigned int dw_length, unsigned char uc_crc_8) {
        unsigned char uc_index;
        while (dw_length--) {
            uc_index = uc_crc_8 ^ (*pch_message++);
            uc_crc_8 = Referee::kCrc8Table[uc_index];
        }
        return (uc_crc_8);
    }

    uint32_t Base::verifyCRC8CheckSum(unsigned char *pch_message, unsigned int dw_length) {
        unsigned char uc_expected;
        if ((pch_message == nullptr) || (dw_length <= 2))
            return 0;
        uc_expected = getCRC8CheckSum(pch_message, dw_length - 1, Referee::kCrc8Init);
        return (uc_expected == pch_message[dw_length - 1]);
    }

    void Base::appendCRC8CheckSum(unsigned char *pch_message, unsigned int dw_length) {
        unsigned char uc_crc;
        if ((pch_message == nullptr) || (dw_length <= 2))
            return;
        uc_crc = getCRC8CheckSum((unsigned char *)pch_message, dw_length - 1, Referee::kCrc8Init);
        pch_message[dw_length - 1] = uc_crc;
    }

    uint16_t Base::getCRC16CheckSum(uint8_t *pch_message, uint32_t dw_length, uint16_t w_crc) {
        uint8_t chData;
        if (pch_message == nullptr)
            return 0xFFFF;
        while (dw_length--) {
            chData = *pch_message++;
            (w_crc) = (static_cast<uint16_t>(w_crc) >> 8) ^
                      Referee::wCRC_table[(static_cast<uint16_t>(w_crc) ^ static_cast<uint16_t>(chData)) & 0x00ff];
        }
        return w_crc;
    }

    uint32_t Base::verifyCRC16CheckSum(uint8_t *pch_message, uint32_t dw_length) {
        uint16_t w_expected;
        if ((pch_message == nullptr) || (dw_length <= 2))
            return 0;
        w_expected = getCRC16CheckSum(pch_message, dw_length - 2, Referee::kCrc16Init);
        return (
            (w_expected & 0xff) == pch_message[dw_length - 2] &&
            ((w_expected >> 8) & 0xff) == pch_message[dw_length - 1]);
    }

    void Base::appendCRC16CheckSum(uint8_t *pch_message, uint32_t dw_length) {
        uint16_t wCRC;
        if ((pch_message == nullptr) || (dw_length <= 2))
            return;
        wCRC = getCRC16CheckSum(static_cast<uint8_t *>(pch_message), dw_length - 2, Referee::kCrc16Init);
        pch_message[dw_length - 2] = static_cast<uint8_t>((wCRC & 0x00ff));
        pch_message[dw_length - 1] = static_cast<uint8_t>(((wCRC >> 8) & 0x00ff));
    }

}  // namespace Device
