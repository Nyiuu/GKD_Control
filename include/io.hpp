#pragma once

#include <string>
#include <thread>
#include <type_traits>
#include <unordered_map>
#include <utils.hpp>

#include "can.hpp"
#include "socket_interface.hpp"

using CAN = IO::Can_interface;

namespace IO
{
    template<typename T>
    class IO
    {
       private:
        std::unordered_map<std::string, T *> data;
        std::vector<std::thread> io_handles;

       public:
        ~IO() {
            for (auto &handle : io_handles) {
                handle.detach();
            }
            io_handles.clear();
        }

        T *operator[](const std::string &key) const {
            auto p = data.find(key);
            if (p == data.end()) {
                LOG_ERR("IO error: no device named %s\n", key.c_str());
                return nullptr;
            }
            return p->second;
        }

        template<typename... Args>
        void insert(Args &&...args) {
            T *data_p = new T(std::forward<Args>(args)...);
            insert(*data_p);
        }

        void insert(T *device) {
            insert(*device);
        }

        void insert(T &device) {
            auto &p = data[device.name];
            if (p != nullptr) {
                LOG_ERR("IO error: double register device named %s\n", device.name.c_str());
                throw std::runtime_error("IO error: double register device named " + device.name);
            }
            p = &device;

            io_handles.emplace_back(std::thread([&]() { device.task(); }));
            
        }


    };

    template<typename T>
    IO<T> io;

    template<typename T>
    class IOManager
    {
    private:
        asio::io_context io_context_;
        asio::executor_work_guard<asio::io_context::executor_type> work_guard_;   // 防止在没有任务时 io_context.run() 立刻退出
        std::unordered_map<std::string, T*> data;
        std::vector<std::jthread> io_threads_;

    public:
        IOManager() 
            : work_guard_(asio::make_work_guard(io_context_))
        {
            const unsigned int thread_count = std::max(1u, std::thread::hardware_concurrency());
            for (unsigned int i = 0; i < thread_count; ++i) {
                io_threads_.emplace_back([this]() {
                    io_context_.run();
                });
            }
        }

        ~IOManager() {
            io_context_.stop();
            for (auto const& [name, ptr] : data) {
                delete ptr;
            }
        }

        T* operator[](const std::string& key) const {
            auto it = data.find(key);
            if (it == data.end()) {
                LOG_ERR("IOManager error: no device named %s\n", key.c_str());
                return nullptr;
            }
            return it->second;
        }

        template<typename... Args>
        void insert(Args&&... args) {
            T* socket_p = new T(io_context_, std::forward<Args>(args)...);       
            insert(*socket_p);
        }

        void insert(T& device) {
            if (data.count(device.name)) {
                LOG_ERR("IOManager error: double register device named %s\n", device.name.c_str());
                throw std::runtime_error("IOManager error: double register device named " + device.name);
            }
            data[device.name] = &device;
            device.task();
        }
    };

    template<typename T>
    IOManager<T> io_manager;

}  // namespace IO




