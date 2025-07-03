#include "step2_callback_system.hpp"
#include <iostream>
#include <cassert>
#include <vector>
#include <string>

// 测试计数器，用于验证回调是否被正确调用
struct TestCounters {
    int motor_callback_count = 0;
    int sensor_callback_count = 0;
    int string_callback_count = 0;
    int multiple_args_count = 0;
    
    void reset() {
        motor_callback_count = 0;
        sensor_callback_count = 0;
        string_callback_count = 0;
        multiple_args_count = 0;
    }
};

TestCounters counters;

// 测试回调系统的基本功能
void test_basic_callback_functionality() {
    std::cout << "=== 测试基本回调功能 ===" << std::endl;
    
    Practice::CallbackManager<int, Practice::MotorData> motor_callbacks;
    Practice::CallbackManager<std::string, Practice::SensorData> sensor_callbacks;
    
    // 测试1: 注册和触发Lambda回调
    std::cout << "\n1. 测试Lambda回调注册和触发..." << std::endl;
    
    motor_callbacks.register_callback(1, [](const Practice::MotorData& data) {
        counters.motor_callback_count++;
        std::cout << "电机回调触发 - ID: " << data.motor_id 
                  << " 速度: " << data.speed 
                  << " 位置: " << data.position << std::endl;
        assert(data.motor_id == 1);
        assert(data.speed == 100.5f);
        assert(data.position == 45.2f);
    });
    
    sensor_callbacks.register_callback("temp_sensor", [](const Practice::SensorData& data) {
        counters.sensor_callback_count++;
        std::cout << "传感器回调触发 - 温度: " << data.temperature 
                  << "°C 电压: " << data.voltage << "V" << std::endl;
        assert(data.temperature == 25.3f);
        assert(data.voltage == 3.3f);
    });
    
    // 触发回调
    motor_callbacks.trigger(1, Practice::MotorData(1, 100.5f, 45.2f));
    sensor_callbacks.trigger("temp_sensor", Practice::SensorData(25.3f, 3.3f));
    
    // 验证回调被正确调用
    assert(counters.motor_callback_count == 1);
    assert(counters.sensor_callback_count == 1);
    
    std::cout << "✅ Lambda回调测试通过！" << std::endl;
}

void test_multiple_callbacks() {
    std::cout << "\n=== 测试多回调管理 ===" << std::endl;
    
    Practice::CallbackManager<int, Practice::MotorData> manager;
    counters.reset();
    
    // 注册多个回调
    std::cout << "\n1. 注册多个不同键值的回调..." << std::endl;
    
    manager.register_callback(1, [](const Practice::MotorData& data) {
        counters.motor_callback_count++;
        std::cout << "电机1回调: " << data.motor_id << std::endl;
    });
    
    manager.register_callback(2, [](const Practice::MotorData& data) {
        counters.motor_callback_count++;
        std::cout << "电机2回调: " << data.motor_id << std::endl;
    });
    
    manager.register_callback(3, [](const Practice::MotorData& data) {
        counters.motor_callback_count++;
        std::cout << "电机3回调: " << data.motor_id << std::endl;
    });
    
    // 测试分别触发
    manager.trigger(1, Practice::MotorData(1, 50.0f, 10.0f));
    manager.trigger(2, Practice::MotorData(2, 75.0f, 20.0f));
    manager.trigger(3, Practice::MotorData(3, 90.0f, 30.0f));
    
    assert(counters.motor_callback_count == 3);
    std::cout << "✅ 多回调管理测试通过！" << std::endl;
}

void test_callback_existence_check() {
    std::cout << "\n=== 测试回调存在性检查 ===" << std::endl;
    
    Practice::CallbackManager<std::string, Practice::SensorData> manager;
    
    // 测试has_callback功能
    std::cout << "\n1. 测试回调存在性检查..." << std::endl;
    
    assert(!manager.has_callback("sensor1"));  // 应该不存在
    
    manager.register_callback("sensor1", [](const Practice::SensorData& data) {
        std::cout << "传感器1: " << data.temperature << std::endl;
    });
    
    assert(manager.has_callback("sensor1"));   // 现在应该存在
    assert(!manager.has_callback("sensor2"));  // sensor2不存在
    
    std::cout << "✅ 回调存在性检查测试通过！" << std::endl;
}

void test_callback_removal() {
    std::cout << "\n=== 测试回调移除功能 ===" << std::endl;
    
    Practice::CallbackManager<int, Practice::MotorData> manager;
    counters.reset();
    
    std::cout << "\n1. 测试回调移除..." << std::endl;
    
    // 注册回调
    manager.register_callback(1, [](const Practice::MotorData& data) {
        counters.motor_callback_count++;
        std::cout << "这个回调应该被移除" << std::endl;
    });
    
    assert(manager.has_callback(1));
    
    // 触发回调（应该成功）
    manager.trigger(1, Practice::MotorData(1, 0.0f, 0.0f));
    assert(counters.motor_callback_count == 1);
    
    // 移除回调
    manager.remove_callback(1);
    assert(!manager.has_callback(1));
    
    // 再次触发（应该无效果）
    manager.trigger(1, Practice::MotorData(1, 0.0f, 0.0f));
    assert(counters.motor_callback_count == 1);  // 计数器不应该增加
    
    std::cout << "✅ 回调移除测试通过！" << std::endl;
}

void test_non_existent_callback() {
    std::cout << "\n=== 测试不存在回调的触发 ===" << std::endl;
    
    Practice::CallbackManager<int, Practice::MotorData> manager;
    
    std::cout << "\n1. 触发不存在的回调（应该安全忽略）..." << std::endl;
    
    // 触发不存在的回调，不应该崩溃
    manager.trigger(999, Practice::MotorData(999, 0.0f, 0.0f));
    
    std::cout << "✅ 不存在回调测试通过！" << std::endl;
}

void test_function_object_callback() {
    std::cout << "\n=== 测试函数对象回调 ===" << std::endl;
    
    Practice::CallbackManager<std::string, Practice::SensorData> manager;
    counters.reset();
    
    // 使用std::function
    std::function<void(const Practice::SensorData&)> func = [](const Practice::SensorData& data) {
        counters.sensor_callback_count++;
        std::cout << "函数对象回调: " << data.temperature << std::endl;
    };
    
    manager.register_callback("func_test", func);
    manager.trigger("func_test", Practice::SensorData(30.0f, 5.0f));
    
    assert(counters.sensor_callback_count == 1);
    std::cout << "✅ 函数对象回调测试通过！" << std::endl;
}

void test_multiple_parameter_types() {
    std::cout << "\n=== 测试多参数类型回调 ===" << std::endl;
    
    // 测试多个参数的回调
    Practice::CallbackManager<int, int, float, std::string> multi_param_manager;
    counters.reset();
    
    multi_param_manager.register_callback(1, [](const int& id, const float& value, const std::string& name) {
        counters.multiple_args_count++;
        std::cout << "多参数回调 - ID: " << id << " 值: " << value << " 名称: " << name << std::endl;
        assert(id == 42);
        assert(value == 3.14f);
        assert(name == "test");
    });
    
    multi_param_manager.trigger(1, 42, 3.14f, std::string("test"));
    
    assert(counters.multiple_args_count == 1);
    std::cout << "✅ 多参数类型回调测试通过！" << std::endl;
}

void test_callback_overwrite() {
    std::cout << "\n=== 测试回调覆盖 ===" << std::endl;
    
    Practice::CallbackManager<int, Practice::MotorData> manager;
    counters.reset();
    
    // 第一个回调
    manager.register_callback(1, [](const Practice::MotorData& data) {
        counters.motor_callback_count++;
        std::cout << "第一个回调" << std::endl;
    });
    
    // 覆盖同一个键的回调
    manager.register_callback(1, [](const Practice::MotorData& data) {
        counters.sensor_callback_count++;  // 使用不同计数器
        std::cout << "第二个回调（覆盖）" << std::endl;
    });
    
    manager.trigger(1, Practice::MotorData(1, 0.0f, 0.0f));
    
    // 只有第二个回调应该被调用
    assert(counters.motor_callback_count == 0);
    assert(counters.sensor_callback_count == 1);
    
    std::cout << "✅ 回调覆盖测试通过！" << std::endl;
}

int main() {
    try {
        std::cout << "🚀 开始测试回调系统实现..." << std::endl;
        
        test_basic_callback_functionality();
        test_multiple_callbacks();
        test_callback_existence_check();
        test_callback_removal();
        test_non_existent_callback();
        test_function_object_callback();
        test_multiple_parameter_types();
        test_callback_overwrite();
        
        std::cout << "\n🎉 恭喜！所有回调系统测试都通过了！" << std::endl;
        std::cout << "您已经成功实现了一个功能完整的事件驱动回调系统！" << std::endl;
        std::cout << "\n💡 您掌握的技能：" << std::endl;
        std::cout << "✅ std::function 类型擦除技术" << std::endl;
        std::cout << "✅ Lambda表达式和函数对象" << std::endl;
        std::cout << "✅ 可变参数模板的实际应用" << std::endl;
        std::cout << "✅ 事件驱动编程模式" << std::endl;
        std::cout << "\n🚀 现在可以继续第三步：电机驱动系统的学习！" << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "\n❌ 测试失败: " << e.what() << std::endl;
        std::cout << "请检查您的step2实现，确保所有TODO项都已正确完成。" << std::endl;
        std::cout << "\n💡 常见问题检查：" << std::endl;
        std::cout << "1. 是否正确实现了 register_callback 方法？" << std::endl;
        std::cout << "2. 是否正确实现了 trigger 方法？" << std::endl;
        std::cout << "3. 是否正确实现了 has_callback 和 remove_callback 方法？" << std::endl;
        std::cout << "4. 参数包展开是否正确？" << std::endl;
        return 1;
    }
    
    return 0;
}

// 编译命令：
// g++ -std=c++17 test_step2.cpp -o test_step2
// ./test_step2 
