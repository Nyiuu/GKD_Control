#pragma once
#include <functional>
#include <map>
#include <iostream>

// 第二步：实现回调系统
// 学习目标：理解std::function、Lambda表达式、可变参数模板

namespace Practice {
    
    // TODO: 实现一个基于键值的回调系统
    template<typename KeyType, typename... ArgTypes>
    class CallbackManager {
    private:
        // TODO: 使用map存储回调函数
        // 提示：std::map<KeyType, std::function<void(const ArgTypes&...)>> callbacks_;
        
    public:
        // TODO: 注册回调函数
        // void register_callback(const KeyType& key, 
        //                       const std::function<void(const ArgTypes&...)>& callback);
        
        // TODO: 注册回调函数（Lambda版本）
        // template<typename Callable>
        // void register_callback(const KeyType& key, Callable&& callback);
        
        // TODO: 触发回调
        // void trigger(const KeyType& key, const ArgTypes&... args);
        
        // TODO: 检查是否有回调
        // bool has_callback(const KeyType& key) const;
        
        // TODO: 移除回调
        // void remove_callback(const KeyType& key);
    };
    
    // 测试用的数据结构
    struct MotorData {
        int motor_id;
        float speed;
        float position;
        
        MotorData(int id, float s, float p) : motor_id(id), speed(s), position(p) {}
    };
    
    struct SensorData {
        float temperature;
        float voltage;
        
        SensorData(float temp, float volt) : temperature(temp), voltage(volt) {}
    };
}

// 使用示例
/*
int main() {
    // 创建不同类型的回调管理器
    Practice::CallbackManager<int, Practice::MotorData> motor_callbacks;
    Practice::CallbackManager<std::string, Practice::SensorData> sensor_callbacks;
    
    // 注册回调 - 使用Lambda
    motor_callbacks.register_callback(1, [](const Practice::MotorData& data) {
        std::cout << "电机" << data.motor_id << " 速度: " << data.speed 
                  << " 位置: " << data.position << std::endl;
    });
    
    // 注册回调 - 使用函数对象
    sensor_callbacks.register_callback("temp_sensor", 
        [](const Practice::SensorData& data) {
            std::cout << "传感器 - 温度: " << data.temperature 
                      << "°C 电压: " << data.voltage << "V" << std::endl;
        });
    
    // 触发回调
    motor_callbacks.trigger(1, Practice::MotorData(1, 100.5f, 45.2f));
    sensor_callbacks.trigger("temp_sensor", Practice::SensorData(25.3f, 3.3f));
    
    return 0;
}
*/ 