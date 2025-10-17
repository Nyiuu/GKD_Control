#include "can.hpp"

#include <asio/detached.hpp>
#include <cstddef>
#include <cstring>

#include "utils.hpp"

namespace IO
{
    Can_interface::Can_interface(asio::io_context& external_io_context, const std::string &name) : name(name), can_descriptor_(external_io_context, init(name.c_str())) {
        if (!can_descriptor_.is_open()) { 
            throw std::runtime_error("Failed to create and bind CAN socket.");
        }    
    }

void Can_interface::task() {
        asio::co_spawn(can_descriptor_.get_executor(), 
                       std::bind(&Can_interface::receive_loop, this), 
                       asio::detached);
    }

    asio::awaitable<void> Can_interface::receive_loop() {
        for (;;) { 
            std::size_t n = co_await can_descriptor_.async_read_some(
                asio::buffer(&frame_r, sizeof(frame_r)),
                asio::use_awaitable
            );

            callback_key(frame_r.can_id, frame_r);
        }


    }

    int Can_interface::init(const char *can_channel) {
        addr = new sockaddr_can;
        ifr = new ifreq;
        // create CAN socket
        int socket_id = socket(PF_CAN, SOCK_RAW, CAN_RAW);
        if (socket_id < 0) {
            LOG_ERR("Error while creating socket");
            delete addr;
            delete ifr;
            exit(-1);
        }

        std::strcpy(ifr->ifr_name, can_channel);
        ioctl(socket_id, SIOCGIFINDEX, ifr);

        addr->can_family = AF_CAN;
        addr->can_ifindex = ifr->ifr_ifindex;

        // bind CAN socket to the given interface
        if (bind(socket_id, (sockaddr *)addr, sizeof(*addr)) < 0) {
            perror("Error in socket bind");
            close(socket_id); // 绑定失败时关闭套接字
            delete addr;
            delete ifr;
            exit(-1);
        }
        
        return socket_id;
    }

    Can_interface::~Can_interface() {
        delete addr;
        delete ifr;
    }

    asio::awaitable<void> Can_interface::send(const can_frame &frame) {

        co_await can_descriptor_.async_write_some(
            asio::buffer(&frame, sizeof(can_frame)),
            asio::use_awaitable
        );
    }

}  // namespace IO
