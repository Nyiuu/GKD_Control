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
        std::map<KeyType, std::function<void(const ArgTypes&...)>> callbacks_;
        
    public:
        // TODO: 注册回调函数
         void register_callback(const KeyType& key, const std::function<void(const ArgTypes&...)>& callback) {
            callbacks_[key] = callback;  
         }
        
        // TODO: 注册回调函数（Lambda版本）
         template<typename Callable>
         void register_callback(const KeyType& key, Callable&& callback){
            callbacks_[key] = callback;
         }
        
        // TODO: 触发回调
         void trigger(const KeyType& key, const ArgTypes&... args){ 
           if (has_callback(key)) {
                callbacks_[key](args...);
           }                                                          
         }
        
        // TODO: 检查是否有回调
         bool has_callback(const KeyType& key) const {
            auto it = callbacks_.find(key);
            if(it != callbacks_.end()) {
                return true;
            }
            return false;
         }
        
        // TODO: 移除回调
         void remove_callback(const KeyType& key) {
            if (has_callback(key)) {
                callbacks_.erase(key);
            }
         }
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

