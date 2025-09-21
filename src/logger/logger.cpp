

#pragma once
#include "logger/logger.hpp"

[[noreturn]] void Logger::task() {
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

                auto is_sent = send(client_socket, buffer.data(), buffer.length(), 0);

                if(is_sent < 0) {
                    LOG_ERR("发送失败");
                    continue;
                } 
            }
        }