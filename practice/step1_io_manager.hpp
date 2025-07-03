#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include <iostream>

// 第一步：实现一个简化版的IO管理器
// 学习目标：理解模板、智能指针、操作符重载

namespace Practice {
    
    // TODO: 请您实现这个模板类
    template<typename DeviceType>
    class IOManager {
    private:
        // TODO: 使用unordered_map存储设备，键为string，值为智能指针
        // 提示：std::unordered_map<std::string, std::unique_ptr<DeviceType>> devices_;
        std::unordered_map<std::string, std::unique_ptr<DeviceType>> devices_;
        size_t size_ = 0;
    public:
        // TODO: 实现析构函数（智能指针会自动管理内存）
        ~IOManager() = default;
        // TODO: 实现[]操作符重载，返回设备指针
        DeviceType* operator[](const std::string& name) const {
            auto p = devices_.find(name);
            if(p == devices_.end()){
                std::cerr << "non-exist device" << std::endl;
                return nullptr;
            }
            return p->second.get();       
        }

        // TODO: 实现设备注册函数，使用完美转发
        template<typename... Args>
        void register_device(const std::string& name, Args&&... args) {
            DeviceType* device_ptr = new DeviceType(std::forward<Args>(args)...);
            devices_.insert(std::pair(name, device_ptr));
            size_++;
        }

        // TODO: 实现设备数量查询
        size_t size() const { 
            return this->size_;
        }
    };
    
    // 简单的测试设备类
    class MockDevice {
    public:
        std::string name_;
        int id_;
        
        MockDevice(const std::string& name, int id) 
            : name_(name), id_(id) {
            std::cout << "创建设备: " << name_ << " ID: " << id_ << std::endl;
        }
        
        ~MockDevice() {
            std::cout << "销毁设备: " << name_ << std::endl;
        }
        
        void work() {
            std::cout << "设备 " << name_ << " 正在工作..." << std::endl;
        }
    };
}

// 使用示例（您需要实现上面的类后才能运行）
/*
int main() {
    Practice::IOManager<Practice::MockDevice> manager;
    
    // 注册设备
    manager.register_device("motor1", "M3508电机", 1);
    manager.register_device("motor2", "M6020电机", 2);
    
    // 使用设备
    auto* motor1 = manager["motor1"];
    if (motor1) {
        motor1->work();
    }
    
    std::cout << "总共有 " << manager.size() << " 个设备" << std::endl;
    
    return 0;
}
*/ 