#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <typeinfo>
#include "types.hpp"

std::ostream& operator<<(std::ostream& os, const ReceivePacket_RC_CTRL& packet) {
    os << "Packet RC_CTRL {"
       << "  ch0: " << packet.ch0
       << ", ch1: " << packet.ch1
       << ", ch2: " << packet.ch2
       << ", ch3: " << packet.ch3
       << ", ch4: " << packet.ch4
       << ", s1: " << packet.s1
       << ", s2: " << packet.s2
       << ", mouse_x: " << packet.mouse_x
       << ", mouse_y: " << packet.mouse_y
       << ", mouse_z: " << packet.mouse_z
       << ", mouse_l: " << packet.mouse_l
       << ", mouse_r: " << packet.mouse_r
       << ", key: " << packet.key
       << "}";
    return os;
}


std::string getCurrentTime() const {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
    return ss.str();
}

void log_status(const std::string& filename, const std::string& message) {
     std::filesystem::path log_dir = "../log";

    if (!std::filesystem::exists(log_dir)) {
        try {
            std::filesystem::create_directories(log_dir);
            std::cout << "已创建目录: " << log_dir << std::endl;
        } catch (const std::filesystem::filesystem_error& ex) {
            std::cerr << "错误: 无法创建目录 " << log_dir << " - " << ex.what() << std::endl;
            return; 
        }
    }

    std::ofstream logfile("../log/" + filename + ".txt", std::ios::app);
    if (logfile.is_open()) {
        logfile << "[" << getCurrentTime() << "] " << message << std::endl;
        logfile.close(); 
    } else {
        std::cerr << "错误: 无法打开文件 " << filename << std::endl;
    }
}
