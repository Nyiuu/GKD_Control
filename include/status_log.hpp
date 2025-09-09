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


inline void log_rc_ctrl_packet(const std::string& filename, const Types::ReceivePacket_RC_CTRL& packet) {
     std::filesystem::path log_dir = "../log";
    if (!std::filesystem::exists(log_dir)) {
        std::filesystem::create_directories(log_dir);

    }

    std::ofstream logfile("../log/" + filename + ".txt", std::ios::app);
    if (logfile.is_open()) {
        std::string currentTime = getCurrentTime();
        logfile << "[" << currentTime << "] " << "ch0: " << packet.ch0 << std::endl;
        logfile << "[" << currentTime << "] " << "ch1: " << packet.ch1 << std::endl;
        logfile << "[" << currentTime << "] " << "ch2: " << packet.ch2 << std::endl;
        logfile << "[" << currentTime << "] " << "ch3: " << packet.ch3 << std::endl;
        logfile << "[" << currentTime << "] " << "ch4: " << packet.ch4 << std::endl;
        logfile << "[" << currentTime << "] " << "s1: " << packet.s1 << std::endl;
        logfile << "[" << currentTime << "] " << "s2: " << packet.s2 << std::endl;
        logfile << "[" << currentTime << "] " << "mouse_x: " << packet.mouse_x << std::endl;
        logfile << "[" << currentTime << "] " << "mouse_y: " << packet.mouse_y << std::endl;
        logfile << "[" << currentTime << "] " << "mouse_z: " << packet.mouse_z << std::endl;
        logfile << "[" << currentTime << "] " << "mouse_l: " << packet.mouse_l << std::endl;
        logfile << "[" << currentTime << "] " << "mouse_r: " << packet.mouse_r << std::endl;
        logfile << "[" << currentTime << "] " << "key: " << packet.key << std::endl;
        logfile.close(); 
    } else {
        std::cerr << "错误: 无法打开文件 " << filename << std::endl;
    }
}