#ifndef __CAN_INTERFACE__
#define __CAN_INTERFACE__

#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <asio.hpp>

#include <asio/posix/stream_descriptor.hpp>
#include <cstdint>
#include <cstdlib>

#include "hardware_callback.hpp"
#include "types.hpp"

typedef asio::posix::stream_descriptor can_descriptor;

namespace IO
{
    class Can_interface : public Callback_key<uint32_t, can_frame>
    {
       public:
        Can_interface(asio::io_context& external_io_context, const std::string &name);
        ~Can_interface();
        asio::awaitable<void>  send(const can_frame &frame);
        void task();
        int init(const char *can_channel);

       private:
        sockaddr_can *addr;
        can_frame frame_r;
        ifreq *ifr;
        Types::debug_info_t *debug;
        can_descriptor can_descriptor_;

        asio::awaitable<void> receive_loop();

       public:
        std::string name;
    };

}  // namespace IO

#endif
