#include "socket_interface.hpp"
#include <thread>
#include "robot.hpp"
#include "user_lib.hpp"


namespace IO
{
    void Server_socket_interface::task() {
        asio::ip::udp::endpoint listen_endpoint(asio::ip::udp::v4(), port_num);
        socket_.open(listen_endpoint.protocol());
        socket_.bind(listen_endpoint);

        asio::co_spawn(socket_.get_executor(), std::bind(&Server_socket_interface::receive_loop, this), asio::detached);
    }   

    asio::awaitable<void> Server_socket_interface::receive_loop() {
        for (;;) {
            asio::ip::udp::endpoint remote_endpoint;
            
            std::size_t n = co_await socket_.async_receive_from(
                asio::buffer(buffer),
                remote_endpoint,
                asio::use_awaitable
            );

            if (n > 0) {
                uint8_t header = buffer[0];
                {
                    std::lock_guard<std::mutex> lock(clients_mutex_);
                    if (clients.find(header) == clients.end()) {
                        LOG_OK("Register new client %d from %s:%d\n", header, remote_endpoint.address().to_string().c_str(), remote_endpoint.port());
                        clients[header] = remote_endpoint;
                    }
                }

                switch (header) {
                    case GIMBAL_HEADER: {
                        Robot::ReceiveNavigationInfo pkg{};
                        UserLib::unpack(pkg, buffer.data());
                        this->callback(pkg);
                        break;
                    }
                    default: {
                        Robot::Auto_aim_control vc;
                        UserLib::unpack(vc, buffer.data());
                        this->callback_key(vc.header, vc);
                        break;
                    }
                }
            }
        }
    }

    Server_socket_interface::Server_socket_interface(asio::io_context& io_ctx, std::string name)
        : port_num(11451),
          name(name),
          socket_(io_ctx) 
    {

    }



    Server_socket_interface::~Server_socket_interface() {
        if (socket_.is_open()) {
            socket_.close();
        }
    }

    void Server_socket_interface::add_client(uint8_t header, std::string ip, int port) {
        asio::ip::address addr = asio::ip::make_address(ip);
        asio::ip::udp::endpoint endpoint(addr, port);

        std::lock_guard<std::mutex> lock(clients_mutex_);
        clients[header] = endpoint;
    }

}  // namespace IO