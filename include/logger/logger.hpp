#pragma once

#include "utils.hpp"
#include <concurrentqueue.h>
#include <blockingconcurrentqueue.h>
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
#include <unordered_set>
#include <utility>
#include "singleton.hpp"

//tcp发送端初始化

//入列并转换，用map存储名字

//tcp发送

enum class MessageType : uint8_t {
    RegisterName = 0x00,
    UpdateValue  = 0x01,
    Console      = 0x02,
    MessageBox   = 0x03   // 注意协议里 MessageBox 的 type 应该和 Console 区分开，这里改为 0x03
};

struct LogMessage {
    inline static std::string build(const std::string& data, MessageType type) {
        uint16_t package_size = static_cast<uint16_t>(data.size() + sizeof(uint16_t) + sizeof(uint8_t));
        // = 2字节长度 + 1字节类型 + data.size()

        std::string res;
        res.resize(package_size);

        // 拷贝长度
        memcpy(res.data(), &package_size, sizeof(package_size));
        // 写入类型
        res[2] = static_cast<char>(type);
        // 拷贝 payload
        memcpy(res.data() + 3, data.data(), data.size());

        return res;
    }
};

// RegisterName
struct LogRegisterNameMessage : public LogMessage {
    static std::string build(uint32_t id, const std::string& name) {
        uint8_t name_length = static_cast<uint8_t>(name.size());

        // payload = id(4) + name_length(1) + name(n)
        std::string payload;
        payload.resize(sizeof(uint32_t) + sizeof(uint8_t) + name_length);

        // 拷贝 id
        memcpy(payload.data(), &id, sizeof(id));
        // 拷贝 name_length
        payload[sizeof(id)] = static_cast<char>(name_length);
        // 拷贝 name
        memcpy(payload.data() + sizeof(id) + sizeof(uint8_t), name.data(), name_length);

        return LogMessage::build(payload, MessageType::RegisterName);
    }
};


// UpdateValue
struct LogUpdateValueMessage : public LogMessage {
    static std::string build(uint32_t id, double value) {
        std::string payload;
        payload.resize(sizeof(uint32_t) + sizeof(double));

        memcpy(payload.data(), &id, sizeof(id));
        memcpy(payload.data() + sizeof(id), &value, sizeof(value));

        return LogMessage::build(payload, MessageType::UpdateValue);
    }
};

// Console Message
struct LogConsoleMessage : public LogMessage {
    static std::string build(const std::string& msg) {
        uint16_t msg_len = static_cast<uint16_t>(msg.size());

        std::string payload;
        payload.resize(sizeof(uint16_t) + msg_len);

        memcpy(payload.data(), &msg_len, sizeof(msg_len));
        memcpy(payload.data() + sizeof(uint16_t), msg.data(), msg.size());

        return LogMessage::build(payload, MessageType::Console);
    }
};

// MessageBox Message
struct LogMessageBoxMessage : public LogMessage {
    static std::string build(const std::string& msg) {
        uint16_t msg_len = static_cast<uint16_t>(msg.size());

        std::string payload;
        payload.resize(sizeof(uint16_t) + msg_len);

        memcpy(payload.data(), &msg_len, sizeof(msg_len));
        memcpy(payload.data() + sizeof(uint16_t), msg.data(), msg.size());

        return LogMessage::build(payload, MessageType::MessageBox);
    }
};



inline uint32_t string_hash(const std::string& str) {
    uint32_t hash = 2166136261u;  // FNV offset basis
    for (unsigned char c : str) {
        hash ^= c;
        hash *= 16777619u;        // FNV prime
    }
    return hash;
}

class Logger:public Singleton<Logger>{
    private:
        std::unordered_set<std::string> _registered_names;
        moodycamel::BlockingConcurrentQueue<std::string> _q;
        int client_socket;

    public:
        template<typename T,typename... Args>
        inline void push_message(Args&&... args){
            auto message = T::build(std::forward<Args>(args)...);
            _q.enqueue(message);
        }

        inline void push_value(const std::string& name,double value){
            uint32_t hash = string_hash(name);

            if(!_registered_names.contains(name)){
                push_message<LogRegisterNameMessage>(hash,name);
                _registered_names.insert(name);
            }

            push_message<LogUpdateValueMessage>(hash,value);
        }

        inline void push_console_message(const std::string& msg){
            push_message<LogConsoleMessage> (msg);
        }

        inline void push_message_box(const std::string& msg){
            push_message<LogMessageBoxMessage> (msg);
        }

        [[noreturn]] inline void task() {
            client_socket = socket(AF_INET, SOCK_STREAM, 0);
            if (client_socket < 0) {
            }

            sockaddr_in server_addr;
            server_addr.sin_family = AF_INET;
            server_addr.sin_port = htons(8080);

            inet_pton(AF_INET, "192.168.1.116", &server_addr.sin_addr);

            while (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
                LOG_ERR("没连上");
                continue;
            }

            while (true) {
                std::vector<std::string> buffers(16);
                std::string buffer = "";

                _q.wait_dequeue_bulk(buffers.begin(),16);
                for(int i=0;i<16;i++)
                    buffer += buffers[i];

                printf("%lu",_q.size_approx());
                auto is_sent = send(client_socket, buffer.data(), buffer.length(), 0);

                if(is_sent < 0) {
                    LOG_ERR("发送失败");
                    continue;
                } 
            }
        }

};

#define logger (Logger::instance())

