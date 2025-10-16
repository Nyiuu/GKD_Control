#ifndef __SOCKET_INTERFACE__
#define __SOCKET_INTERFACE__

#include <asio.hpp>
#include <asio/awaitable.hpp>
#include <asio/co_spawn.hpp>
#include <asio/detached.hpp>
#include <asio/ip/udp.hpp>
#include <asio/registered_buffer.hpp>
#include <asio/ts/internet.hpp>
#include <asio/use_awaitable.hpp>
#include <asio/write.hpp>
#include <cstddef>

#include <unistd.h>

#include <cstdio>
#include <cstring>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

#include "hardware_callback.hpp"
#include "robot.hpp"
#include "utils.hpp"

namespace IO
{   
     class Server_socket_interface : public Callback<Robot::ReceiveNavigationInfo>, public Callback_key<uint8_t, Robot::Auto_aim_control>

    {
       public:
            Server_socket_interface(std::string name);
            ~Server_socket_interface();
            void task();
            void add_client(uint8_t header, std::string ip, int port);
 
            template<typename T>
            void send(const T &pkg) {
                uint8_t header = *(uint8_t *)(&pkg);
                // LOG_INFO("header %d client: %d\n", header, clients.find(header)->second.sin_addr.s_addr);
                
                std::unique_lock<std::mutex> lock(clients_mutex_);
                auto it = clients.find(header);
                if (it == clients.end()) {
                    return;
                }
                asio::ip::udp::endpoint destination_endpoint = it->second;
                lock.unlock();

                asio::post(io_context, [this, pkg_data = std::make_shared<T>(pkg), destination_endpoint]() {
                    socket_.async_send_to(
                        asio::buffer(pkg_data.get(), sizeof(T)),
                        destination_endpoint,
                        asio::detached 
                    );
                });
            }
        public:
            std::string name;
        private:
            asio::io_context io_context;
            asio::ip::udp::socket socket_;
            std::jthread io_thread;

            int64_t port_num;
            std::map<uint8_t, asio::ip::udp::endpoint> clients;
            std::array<char, 256> buffer;
            
            std::mutex clients_mutex_;

            asio::awaitable<void> receive_loop();
    };

}  // namespace IO
#endif

using SOCKET = IO::Server_socket_interface;
