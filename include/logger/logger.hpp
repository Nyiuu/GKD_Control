#pragma once

#include "utils.hpp"
#include <concurrentqueue.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <chrono>
#include <cstdint>
#include <string>
#include <map>
#include <functional> 
#include <iostream>
#include <thread>
#include <unordered_map>
#include <sstream>
#include <cstring>
#include "singleton.hpp"

//tcp发送端初始化

//入列并转换，用map存储名字

//tcp发送

enum class MessageType : uint8_t{
        RegisterName,
        UpdateValue,
        Console,
        MessageBox
    };

struct LogMessage{
    
    uint16_t package_size;
    MessageType type;

    virtual ~LogMessage() = default;

    virtual std::string to_data() = 0;
};

struct LogRegisterNameMessage : public LogMessage{
    uint8_t name_length;
    std::string name;

    LogRegisterNameMessage(const std::string& var_name) {
        this->package_size = sizeof(LogMessage) + sizeof(name_length) + var_name.length();
        this->type = MessageType::RegisterName;
        this->name_length = static_cast<uint8_t>(var_name.length());
        this->name = var_name;
    }

    ~LogRegisterNameMessage() = default;

    std::string to_data() override{
        std::string buffer;
        buffer.resize(this->package_size);
        char* ptr = buffer.data();

        std::memcpy(ptr, &this->package_size, sizeof(this->package_size));
        ptr += sizeof(this->package_size);
        std::memcpy(ptr, &this->type, sizeof(this->type));
        ptr += sizeof(this->type);

        std::memcpy(ptr, &this->name_length, sizeof(this->name_length));
        ptr += sizeof(this->name_length);

        std::memcpy(ptr, this->name.data(), this->name_length);

        return buffer;

    }


};

//待实现
struct LogUpdateValueMessage : public LogMessage{
    uint32_t id;
    double value;
    
    LogUpdateValueMessage(std::string name, double var_value) {
        this->package_size = sizeof(LogUpdateValueMessage);
        this->type = MessageType::UpdateValue;
        this->id = hash(name);
        this->value = var_value;
    }

    ~LogUpdateValueMessage() = default;

    inline uint32_t hash(const std::string& str){
        unsigned int hash = 5381; 
        for (char c : str) {
            hash = ((hash << 5) + hash) + c;
        }
        return hash;
    }

    std::string to_data() override{
        std::string buffer;
        buffer.resize(this->package_size);
        char* ptr = buffer.data();

        std::memcpy(ptr, &this->package_size, sizeof(this->package_size));
        ptr += sizeof(this->package_size);
        std::memcpy(ptr, &this->type, sizeof(this->type));
        ptr += sizeof(this->type);

        std::memcpy(ptr, &this->id, sizeof(this->id));
        ptr += sizeof(this->id);

        std::memcpy(ptr, &this->value, this->value);

        return buffer;

    }
};


//待实现
struct LogConsoleMessage : public LogMessage{
    uint16_t message_length;
    std::string message;

    LogConsoleMessage(const std::string& msg) {
        this->package_size = sizeof(LogMessage) + sizeof(message_length) + msg.length();
        this->type = MessageType::Console;
        this->message_length = static_cast<uint16_t>(msg.length());
        this->message = msg;
    }

    ~LogConsoleMessage() = default;

    std::string to_data() override {
        return "hello";
    }
};

//待实现
struct LogMessageBoxMessage : public LogMessage{
    uint16_t message_length;
    std::string message;

    LogMessageBoxMessage(const std::string& msg) {
        this->package_size = sizeof(LogMessage) + sizeof(message_length) + msg.length();
        this->type = MessageType::MessageBox;
        this->message_length = static_cast<uint16_t>(msg.length());
        this->message = msg;
    }

    ~LogMessageBoxMessage() = default;

    std::string to_data() override {
        return "hello"; 
    }
};





class Logger:public Singleton<Logger>{
    private:
        
        std::unordered_map<uint32_t, std::string> _name_map;
        moodycamel::ConcurrentQueue<std::string> _q;
        int client_socket;

        inline void send_data() {
            std::string buffer;
            if (_q.try_dequeue(buffer)) {
                if (buffer.empty()) {
                    return;
                }
                
                auto is_send = send(client_socket, buffer.data(), buffer.length(), 0);

                if(is_send < 0) {
                    LOG_ERR("发送失败");
                    return;
                } 
            }
        }

    public:

        inline void init() {
            client_socket = socket(AF_INET, SOCK_STREAM, 0);
            if (client_socket < 0) {
                return;
            }

            sockaddr_in server_addr;
            server_addr.sin_family = AF_INET;
            server_addr.sin_port = htons(8080);

            inet_pton(AF_INET, "192.168.1.23", &server_addr.sin_addr);

            if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
                LOG_ERR("没连上");
                return;
            }

            LOG_ERR("连上了");


        }

        [[noreturn]] inline void task() {
            while (client_socket >= 0) {
                send_data();
            }
        }

        inline void push_message(LogMessage* msg){            
#ifdef __DEBUG__
            _q.enqueue(msg->to_data());      
#endif
        }

};

#define logger (Logger::instance())

