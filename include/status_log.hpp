#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <typeinfo>
#include "types.hpp"

#undef concat
#include <filesystem>


inline std::string getCurrentTime() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
    return ss.str();
}

inline void log_chassis(const std::string& filename, const std::array<float, 4>& cmd_power) {
     std::filesystem::path log_dir = "../log";

    if (!std::filesystem::exists(log_dir)) {
        std::filesystem::create_directories(log_dir);

    }

    std::ofstream logfile("../log/" + filename + ".txt", std::ios::app);
    if (logfile.is_open()) {
        std::string currentTime = getCurrentTime();
        logfile << "[" << currentTime << "] " << "wheel1: " << cmd_power[0] << std::endl;
        logfile << "[" << currentTime << "] " << "wheel2: " << cmd_power[1] << std::endl;
        logfile << "[" << currentTime << "] " << "wheel3: " << cmd_power[2] << std::endl;
        logfile << "[" << currentTime << "] " << "wheel4: " << cmd_power[3] << std::endl;
        logfile.close(); 
    } else {
        std::cerr << "错误: 无法打开文件 " << filename << std::endl;
    }
}


struct LastValues {
    int ch0 = -1;
    int ch1 = -1;
    int ch2 = -1;
    int ch3 = -1;
    int ch4 = -1;
    int s1 = -1;
    int s2 = -1;
    int mouse_x = -1;
    int mouse_y = -1;
    int mouse_z = -1;
    int mouse_l = -1;
    int mouse_r = -1;
    int key = -1;
};

inline void log_rc_ctrl_packet(const std::string& filename, const Types::ReceivePacket_RC_CTRL& packet) {
    // 使用静态变量存储上一次的值，只在函数第一次调用时初始化一次
    static LastValues lastPacketValues;

    std::filesystem::path log_dir = "../log";
    if (!std::filesystem::exists(log_dir)) {
        std::filesystem::create_directories(log_dir);
    }

    std::ofstream logfile("../log/" + filename + ".txt", std::ios::app);
    if (logfile.is_open()) {
        std::string currentTime = getCurrentTime();
        
        // 检查值是否与上一次不同，如果不同则打印并更新上一次的值
        if (packet.ch0 != lastPacketValues.ch0) {
            logfile << "[" << currentTime << "] " << "ch0: " << packet.ch0 << std::endl;
            lastPacketValues.ch0 = packet.ch0;
        }
        if (packet.ch1 != lastPacketValues.ch1) {
            logfile << "[" << currentTime << "] " << "ch1: " << packet.ch1 << std::endl;
            lastPacketValues.ch1 = packet.ch1;
        }
        if (packet.ch2 != lastPacketValues.ch2) {
            logfile << "[" << currentTime << "] " << "ch2: " << packet.ch2 << std::endl;
            lastPacketValues.ch2 = packet.ch2;
        }
        if (packet.ch3 != lastPacketValues.ch3) {
            logfile << "[" << currentTime << "] " << "ch3: " << packet.ch3 << std::endl;
            lastPacketValues.ch3 = packet.ch3;
        }
        if (packet.ch4 != lastPacketValues.ch4) {
            logfile << "[" << currentTime << "] " << "ch4: " << packet.ch4 << std::endl;
            lastPacketValues.ch4 = packet.ch4;
        }
        if (packet.s1 != lastPacketValues.s1) {
            logfile << "[" << currentTime << "] " << "s1: " << packet.s1 << std::endl;
            lastPacketValues.s1 = packet.s1;
        }
        if (packet.s2 != lastPacketValues.s2) {
            logfile << "[" << currentTime << "] " << "s2: " << packet.s2 << std::endl;
            lastPacketValues.s2 = packet.s2;
        }
        if (packet.mouse_x != lastPacketValues.mouse_x) {
            logfile << "[" << currentTime << "] " << "mouse_x: " << packet.mouse_x << std::endl;
            lastPacketValues.mouse_x = packet.mouse_x;
        }
        if (packet.mouse_y != lastPacketValues.mouse_y) {
            logfile << "[" << currentTime << "] " << "mouse_y: " << packet.mouse_y << std::endl;
            lastPacketValues.mouse_y = packet.mouse_y;
        }
        if (packet.mouse_z != lastPacketValues.mouse_z) {
            logfile << "[" << currentTime << "] " << "mouse_z: " << packet.mouse_z << std::endl;
            lastPacketValues.mouse_z = packet.mouse_z;
        }
        if (packet.mouse_l != lastPacketValues.mouse_l) {
            logfile << "[" << currentTime << "] " << "mouse_l: " << packet.mouse_l << std::endl;
            lastPacketValues.mouse_l = packet.mouse_l;
        }
        if (packet.mouse_r != lastPacketValues.mouse_r) {
            logfile << "[" << currentTime << "] " << "mouse_r: " << packet.mouse_r << std::endl;
            lastPacketValues.mouse_r = packet.mouse_r;
        }
        if (packet.key != lastPacketValues.key) {
            logfile << "[" << currentTime << "] " << "key: " << packet.key << std::endl;
            lastPacketValues.key = packet.key;
        }
        
        logfile.close(); 
    } else {
        std::cerr << "错误: 无法打开文件 " << filename << std::endl;
    }
}