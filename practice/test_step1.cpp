#include "step1_io_manager.hpp"
#include <iostream>
#include <cassert>

// 简单测试函数
void test_io_manager() {
    std::cout << "=== 测试IO管理器 ===" << std::endl;
    
    Practice::IOManager<Practice::MockDevice> manager;
    
    // 测试设备注册
    std::cout << "\n1. 测试设备注册..." << std::endl;
    manager.register_device("motor1", "M3508电机", 1);
    manager.register_device("motor2", "M6020电机", 2);
    manager.register_device("sensor1", "IMU传感器", 100);
    
    // 测试设备数量
    std::cout << "\n2. 测试设备数量..." << std::endl;
    std::cout << "注册的设备数量: " << manager.size() << std::endl;
    assert(manager.size() == 3);  // 应该有3个设备
    
    // 测试设备访问
    std::cout << "\n3. 测试设备访问..." << std::endl;
    auto* motor1 = manager["motor1"];
    auto* motor2 = manager["motor2"];
    auto* sensor1 = manager["sensor1"];
    auto* not_exist = manager["not_exist"];
    
    // 检查设备是否正确获取
    assert(motor1 != nullptr);
    assert(motor2 != nullptr);
    assert(sensor1 != nullptr);
    assert(not_exist == nullptr);  // 不存在的设备应该返回nullptr
    
    // 测试设备功能
    std::cout << "\n4. 测试设备功能..." << std::endl;
    if (motor1) {
        motor1->work();
        std::cout << "Motor1 名称: " << motor1->name_ << ", ID: " << motor1->id_ << std::endl;
    }
    
    if (motor2) {
        motor2->work();
        std::cout << "Motor2 名称: " << motor2->name_ << ", ID: " << motor2->id_ << std::endl;
    }
    
    if (sensor1) {
        sensor1->work();
        std::cout << "Sensor1 名称: " << sensor1->name_ << ", ID: " << sensor1->id_ << std::endl;
    }
    
    std::cout << "\n✅ 所有测试通过！IO管理器工作正常。" << std::endl;
}

int main() {
    try {
        test_io_manager();
        std::cout << "\n🎉 恭喜！您成功实现了IO管理器！" << std::endl;
        std::cout << "现在可以继续第二步：回调系统的学习。" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "\n❌ 测试失败: " << e.what() << std::endl;
        std::cout << "请检查您的实现，确保所有TODO项都已完成。" << std::endl;
        return 1;
    }
    
    return 0;
}

// 编译命令：
// g++ -std=c++17 test_step1.cpp -o test_step1
// ./test_step1 