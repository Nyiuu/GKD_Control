#pragma once
#include "chassis_config.hpp"
#include <cstdint>
#include <cstring>
namespace Monitor
{
    struct MonitorConfig{
        uint8_t header = 0x01;
        std::string ip = "192.168.1.23"; 
        int port = 8080;
    };

    struct MonitorData{
        uint8_t package_size;
        uint8_t package_type;
        double  value;
        uint8_t name_size;
  	    char    name[];
    }__attribute__((packed));

    inline MonitorData* create_monitor_data(double value, const std::string& name) {
        size_t name_len = name.length();

        MonitorData* m_data = (MonitorData*)malloc(sizeof(MonitorData) + name_len);
        if (m_data == nullptr) {
            return nullptr; 
        }

        m_data->package_type = 0x01;
        m_data->value = value;
        m_data->name_size = static_cast<uint8_t>(name_len);
        m_data->package_size = static_cast<uint8_t>(sizeof(MonitorData) + name_len);
        

        memcpy(m_data->name, name.c_str(), name_len);

        return m_data;
    }


    inline std::array<MonitorData*, 4> get_m_data(const std::array<float, 4>& power) {
        std::array<MonitorData*, 4> m_datas;
        
        for (size_t i = 0; i < power.size(); ++i) {
            std::string name = "Wheel." + std::to_string(i + 1);
            m_datas[i] = create_monitor_data(power[i], name);
        }
        
        return m_datas;
    }
}