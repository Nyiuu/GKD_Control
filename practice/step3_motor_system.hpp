#pragma once
#include <iterator>
#include <thread>
#include <mutex>
#include <chrono>
#include <vector>
#include <memory>
#include <unordered_map>
#include <iostream>
#include <atomic>
#include <string>

// 第三步：实现简化的电机驱动系统
// 学习目标：线程管理、RAII、设计模式的综合运用

namespace Practice {
    
    // 电机类型枚举
    enum class MotorType {
        M3508 = 3508,
        M6020 = 6020,
        INVALID = 0
    };
    
    // 电机配置结构
    struct MotorConfig {
        MotorType type;
        std::string interface_name;
        int motor_id;
        float reduction_ratio;
        
        // TODO: 实现构造函数，支持编译时类型检查
        // 提示：可以参考原项目的TypeCast设计
        consteval MotorConfig (const int type_) {
            switch (type_) {
                case 3508: type = MotorType::M3508;
                case 6020: type = MotorType::M6020;
                default: type = MotorType::INVALID;
            }
        }
    };
    
    // 模拟的通信接口
    class MockInterface {
    public:
        std::string name;
        
        MockInterface(const std::string& n) : name(n) {
            std::cout << "创建接口: " << name << std::endl;
        }
        
        ~MockInterface() {
            std::cout << "销毁接口: " << name << std::endl;
        }
        
        // 模拟发送数据
        void send_data(const std::vector<int16_t>& data) {
            std::cout << name << " 发送数据: ";
            for (auto d : data) {
                std::cout << d << " ";
            }
            std::cout << std::endl;
        }
    };
    
    // TODO: 实现电机类
    class Motor {
    private:
        // 电机基本信息
        MotorType type_;
        std::string name_;
        int motor_id_;
        float reduction_ratio_;
        
        // 电机状态数据
        struct MotorData {
            float angle = 0.0f;
            float speed = 0.0f;
            float current = 0.0f;
            float temperature = 0.0f;
        } data_;
        
        // 控制相关
        int16_t target_current_ = 0;
        bool enabled_ = false;
        
    public:
        // TODO: 实现构造函数
        explicit Motor(const MotorConfig& config) {
            type_ = config.type;
            motor_id_ = config.motor_id;
            reduction_ratio_ = config.reduction_ratio;
        }
        
        // TODO: 实现禁用复制和移动
        Motor(const Motor&) = delete;
        Motor& operator=(const Motor&) = delete;
        Motor(Motor&&) = delete;
        Motor& operator=(Motor&&) = delete;
        
        // TODO: 实现设置目标电流
        void set_current(int16_t current) {
            data_.current = current;
        }
        
        // TODO: 实现获取电机数据
        const MotorData& get_data() const {
            return data_;  
        }
        
        // TODO: 实现电机使能
        void enable() {
           enabled_ = true;
        }
        
        // TODO: 获取电机信息
        int get_id() const {
            return motor_id_;
        }
        const std::string& get_name() const {
            return name_;
        }
        int16_t get_target_current() const {
            return data_.current;
        }
        bool is_enabled() const {
            return enabled_;
        }
        
        // 模拟更新电机状态（在实际项目中这来自CAN消息）
        void update_status(float angle, float speed, float current, float temp) {
            data_.angle = angle;
            data_.speed = speed;
            data_.current = current;
            data_.temperature = temp;
        }
    };
    
    // TODO: 实现电机管理器
    class MotorManager {
    private:
        // 接口管理
        std::unordered_map<std::string, std::unique_ptr<MockInterface>> interfaces_;
        
        // 电机管理
        std::unordered_map<std::string, std::vector<std::unique_ptr<Motor>>> motor_groups_;
        
        // 线程管理
        std::unique_ptr<std::thread> control_thread_;
        std::mutex motor_mutex_;
        std::atomic<bool> running_{false};
        
    public:
        // TODO: 实现构造函数和析构函数
        MotorManager() = default;
        ~MotorManager() = default;
        
        // TODO: 注册接口
        void register_interface(const std::string& name) {
            
        }
        
        // TODO: 注册电机
        void register_motor(const MotorConfig& config) {
            
        }
        
        // TODO: 启动管理器
        void start();
        
        // TODO: 停止管理器
        void stop();
        
        // TODO: 获取电机指针
        Motor* get_motor(const std::string& interface_name, int motor_id);
        
    private:
        // TODO: 控制线程主循环
        void control_loop();
        
        // TODO: 发送电机控制指令
        void send_motor_commands(const std::string& interface_name, 
                                 const std::vector<std::unique_ptr<Motor>>& motors);
    };
}

