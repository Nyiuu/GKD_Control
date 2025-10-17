#ifndef __SOCKET_INTERFACE__
#define __SOCKET_INTERFACE__

#include <asio.hpp>
#include <asio/awaitable.hpp>
#include <asio/co_spawn.hpp>
#include <asio/detached.hpp>
#include <asio/ip/udp.hpp>
#include <asio/ts/internet.hpp>
#include <asio/use_awaitable.hpp>
#include <asio/write.hpp>
#include <unistd.h>
#include <cstring>
#include <map>
#include <memory>
#include <mutex>
#include <string>


#include "hardware_callback.hpp"
#include "robot.hpp"
#include "utils.hpp"

#define GIMBAL_HEADER 0x37


namespace IO
{
    class Server_socket_interface : public Callback<Robot::ReceiveNavigationInfo>, public Callback_key<uint8_t, Robot::Auto_aim_control>
    {
       public:
            Server_socket_interface(asio::io_context& external_io_context, std::string name);
            ~Server_socket_interface();

            void task();

            void add_client(uint8_t header, std::string ip, int port);

            template<typename T>
            void send(const T &pkg) {
                uint8_t header = *(uint8_t *)(&pkg);

                std::unique_lock<std::mutex> lock(clients_mutex_);
                auto it = clients.find(header);
                if (it == clients.end()) {
                    return;
                }
                asio::ip::udp::endpoint destination_endpoint = it->second;
                lock.unlock();

                asio::post(socket_.get_executor(), [this, pkg_data = std::make_shared<T>(pkg), destination_endpoint]() {
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
            asio::ip::udp::socket socket_;

            int64_t port_num;
            std::map<uint8_t, asio::ip::udp::endpoint> clients;
            std::array<char, 256> buffer;
            
            std::mutex clients_mutex_;

            asio::awaitable<void> receive_loop();
    };

}  // namespace IO
#endif

using SOCKET = IO::Server_socket_interface;