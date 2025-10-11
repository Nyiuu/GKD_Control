#pragma once

#include "utils.hpp"
#include <concurrentqueue.h>
#include <blockingconcurrentqueue.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/address.hpp>
#include <chrono>
#include <cstdint>
#include <ostream>
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
#include <ctime>
#include <iomanip>
#include <boost/asio.hpp>


namespace asio = boost::asio;
using asio::ip::udp;
using error_code = boost::system::error_code;using error_code = boost::system::error_code;



enum class MessageType : uint8_t {
    RegisterName = 0x00,
    UpdateValue  = 0x01,
    Console      = 0x02,
    MessageBox   = 0x03   
};

struct LogMessage {
    static std::string build(const std::string& data, MessageType type) {
        uint16_t package_size = static_cast<uint16_t>(data.size() + sizeof(uint16_t) + sizeof(uint8_t));
        // = 2字节长度 + 1字节类型 + data.size()

        std::string res;
        res.resize(package_size);

        memcpy(res.data(), &package_size, sizeof(package_size));
        res[2] = static_cast<char>(type);

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



class Logger : public Singleton<Logger>, public std::enable_shared_from_this<Logger> {
private:
    std::unordered_set<std::string> _registered_names;
    moodycamel::BlockingConcurrentQueue<std::string> _q;

    asio::io_context _io_context;
    asio::ip::udp::socket _socket;
    asio::ip::udp::endpoint _server_endpoint;
    std::thread _network_thread;
    std::string _send_buffer; // 用于聚合消息的缓冲区

public:
    Logger()
        : _socket(_io_context) {
    }

    ~Logger() {
        stop();
    }

    void start(const std::string& ip_address, unsigned short port) {
        error_code ec;
        _socket.open(asio::ip::udp::v4(), ec);
        if (ec) {
            LOG_ERR("Socket open failed");
            return;
        }

        _server_endpoint = asio::ip::udp::endpoint(asio::ip::make_address(ip_address, ec), port);
        if (ec) {
            LOG_ERR("Invalid address");
            return;
        }

        // 启动网络线程
        _network_thread = std::thread([this]() {
            do_receive_from_queue(); // 开始从队列中取数据并发送
            _io_context.run(); // 阻塞运行io_context事件循环
        });
    }

    void stop() {
        _io_context.stop();
        if (_network_thread.joinable()) {
            _network_thread.join();
        }
    }

    template<typename T, typename... Args>
    inline void push_message(Args&&... args) {
        auto message = T::build(std::forward<Args>(args)...);
        _q.enqueue(message);
    }

    void push_value(const std::string& name, double value) {
        uint32_t hash = string_hash(name);

        if (_registered_names.find(name) == _registered_names.end()) {
            push_message<LogRegisterNameMessage>(hash, name);
            _registered_names.insert(name);
        }

        push_message<LogUpdateValueMessage>(hash, value);
    }

    // TODO
    void push_console_message(const std::string& msg) {}

    // TODO
    void push_message_box(const std::string& msg) {}

private:
    void do_receive_from_queue() {
        // 从队列中批量获取消息
        std::vector<std::string> messages(16);
        size_t count = _q.wait_dequeue_bulk(messages.begin(), 16);

        _send_buffer.clear();
        for (size_t i = 0; i < count; ++i) {
            _send_buffer += messages[i];
        }

        if (!_send_buffer.empty()) {
            // 开始异步发送
            _socket.async_send_to(
                asio::buffer(_send_buffer),
                _server_endpoint,
                [this](const error_code& ec, std::size_t bytes_transferred) {
                    if (ec) {
                        LOG_ERR("Send failed");
                    }

                    // 本次发送完成后，立即开始下一次的接收和发送循环
                    do_receive_from_queue();
                }
            );
        } else {
            // 如果队列为空，则继续下一次循环
             do_receive_from_queue();
        }
    }
};

#define logger (Logger::instance())

