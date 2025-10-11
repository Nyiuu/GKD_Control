#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <boost/asio.hpp>
#include "concurrentqueue.h" 


namespace asio = boost::asio;
using asio::ip::udp;
using error_code = boost::system::error_code;


class UdpClient : public std::enable_shared_from_this<UdpClient> {
public:
    UdpClient(
        asio::io_context& io_context,
        const std::string& host,
        unsigned short port,
        moodycamel::ConcurrentQueue<std::string>& q)
        : socket_(io_context),
          endpoint_(asio::ip::make_address(host), port),
          queue_(q),
          timer_(io_context),
          send_buffer_pool_(16) // 初始化缓冲区池
    {
        // 打开UDP socket
        socket_.open(udp::v4());
    }

    // 开始异步发送循环
    void start() {
        // 首次尝试从队列发送数据
        async_send_from_queue();
    }

private:
    void async_send_from_queue() {
        // 从池中获取一个缓冲区vector
        auto buffers = std::make_shared<std::vector<std::string>>(16);

        // 尝试从队列中批量取出数据，非阻塞
        size_t dequeued_count = queue_.try_dequeue_bulk(buffers->begin(), 16);

        if (dequeued_count > 0) {
            // 如果成功取出数据，则准备发送
            auto send_buffer = std::make_shared<std::string>();
            for (size_t i = 0; i < dequeued_count; ++i) {
                *send_buffer += (*buffers)[i];
            }

            // 异步发送数据
            socket_.async_send_to(
                asio::buffer(*send_buffer),
                endpoint_,
                [self = shared_from_this(), send_buffer, buffers](const error_code& ec, std::size_t bytes_transferred) {
                    self->handle_send(ec, bytes_transferred);
                });
        } else {
            // 如果队列为空，设置一个短暂的定时器后再次尝试
            // 避免了空转消耗CPU
            timer_.expires_after(std::chrono::milliseconds(10)); // 10毫秒后重试
            timer_.async_wait([self = shared_from_this()](const error_code& ec) {
                if (!ec) {
                    self->async_send_from_queue();
                }
            });
        }
    }

    void handle_send(const error_code& ec, std::size_t bytes_transferred) {
        if (ec) {
           
        }

        // 一次发送完成后，立即尝试下一次发送，形成闭环
        async_send_from_queue();
    }

    udp::socket socket_;
    udp::endpoint endpoint_;
    moodycamel::ConcurrentQueue<std::string>& queue_;
    asio::steady_timer timer_;
    std::vector<std::string> send_buffer_pool_;
};

