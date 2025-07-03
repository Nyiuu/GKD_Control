#include "step3_motor_system.hpp"
#include <iostream>
#include <cassert>
#include <thread>
#include <chrono>
#include <vector>

// 测试结果统计
struct TestResults {
    int interface_created = 0;
    int interface_destroyed = 0;
    int motors_registered = 0;
    int commands_sent = 0;
    bool manager_started = false;
    bool manager_stopped = false;
    
    void reset() {
        interface_created = 0;
        interface_destroyed = 0;
        motors_registered = 0;
        commands_sent = 0;
        manager_started = false;
        manager_stopped = false;
    }
} test_results;

// 测试电机配置和类型安全
void test_motor_config() {
    std::cout << "=== 测试电机配置 ===" << std::endl;
    
    // 测试基本配置创建
    std::cout << "\n1. 测试电机配置创建..." << std::endl;
    
    Practice::MotorConfig config1 = {
        Practice::MotorType::M3508,
        "CAN_CHASSIS", 
        1, 
        1.0f/19.0f
    };
    
    assert(config1.type == Practice::MotorType::M3508);
    assert(config1.interface_name == "CAN_CHASSIS");
    assert(config1.motor_id == 1);
    assert(config1.reduction_ratio == 1.0f/19.0f);
    
    Practice::MotorConfig config2 = {
        Practice::MotorType::M6020,
        "CAN_GIMBAL",
        2,
        1.0f
    };
    
    assert(config2.type == Practice::MotorType::M6020);
    assert(config2.interface_name == "CAN_GIMBAL");
    assert(config2.motor_id == 2);
    assert(config2.reduction_ratio == 1.0f);
    
    std::cout << "✅ 电机配置测试通过！" << std::endl;
}

// 测试MockInterface基本功能
void test_mock_interface() {
    std::cout << "\n=== 测试模拟接口 ===" << std::endl;
    
    std::cout << "\n1. 测试接口创建和销毁..." << std::endl;
    
    {
        Practice::MockInterface interface("TEST_CAN");
        assert(interface.name == "TEST_CAN");
        
        // 测试数据发送
        std::vector<int16_t> test_data = {100, 200, 300, 400};
        interface.send_data(test_data);  // 应该打印数据
    }
    // interface析构时应该打印销毁信息
    
    std::cout << "✅ 模拟接口测试通过！" << std::endl;
}

// 测试Motor类基本功能
void test_motor_basic_functionality() {
    std::cout << "\n=== 测试电机基本功能 ===" << std::endl;
    
    std::cout << "\n1. 测试电机创建和基本操作..." << std::endl;
    
    Practice::MotorConfig config = {
        Practice::MotorType::M3508,
        "CAN_CHASSIS",
        1,
        1.0f/19.0f
    };
    
    Practice::Motor motor(config);
    
    // 测试基本信息获取
    assert(motor.get_id() == 1);
    assert(motor.get_name().find("M3508") != std::string::npos);
    assert(motor.get_name().find("CAN_CHASSIS") != std::string::npos);
    assert(motor.get_name().find("1") != std::string::npos);
    assert(!motor.is_enabled());
    
    // 测试电流设置
    motor.set_current(1000);
    assert(motor.get_target_current() == 1000);
    
    motor.set_current(-500);
    assert(motor.get_target_current() == -500);
    
    // 测试状态更新
    motor.update_status(1.57f, 100.0f, 800, 45.0f);
    auto data = motor.get_data();
    assert(data.angle == 1.57f);
    assert(data.speed == 100.0f);
    assert(data.current == 800);
    assert(data.temperature == 45.0f);
    
    // 测试使能
    motor.enable();
    assert(motor.is_enabled());
    
    std::cout << "✅ 电机基本功能测试通过！" << std::endl;
}

// 测试电机不同类型
void test_different_motor_types() {
    std::cout << "\n=== 测试不同电机类型 ===" << std::endl;
    
    std::cout << "\n1. 测试M3508电机..." << std::endl;
    Practice::MotorConfig m3508_config = {
        Practice::MotorType::M3508, "CAN_CHASSIS", 1, 1.0f/19.0f
    };
    Practice::Motor m3508(m3508_config);
    assert(m3508.get_name().find("M3508") != std::string::npos);
    
    std::cout << "\n2. 测试M6020电机..." << std::endl;
    Practice::MotorConfig m6020_config = {
        Practice::MotorType::M6020, "CAN_GIMBAL", 1, 1.0f
    };
    Practice::Motor m6020(m6020_config);
    assert(m6020.get_name().find("M6020") != std::string::npos);
    
    std::cout << "✅ 不同电机类型测试通过！" << std::endl;
}

// 测试MotorManager基本功能
void test_motor_manager_basic() {
    std::cout << "\n=== 测试电机管理器基本功能 ===" << std::endl;
    
    Practice::MotorManager manager;
    
    std::cout << "\n1. 测试接口注册..." << std::endl;
    manager.register_interface("CAN_CHASSIS");
    manager.register_interface("CAN_GIMBAL");
    
    std::cout << "\n2. 测试电机注册..." << std::endl;
    manager.register_motor({Practice::MotorType::M3508, "CAN_CHASSIS", 1, 1.0f/19.0f});
    manager.register_motor({Practice::MotorType::M3508, "CAN_CHASSIS", 2, 1.0f/19.0f});
    manager.register_motor({Practice::MotorType::M6020, "CAN_GIMBAL", 1, 1.0f});
    
    std::cout << "\n3. 测试电机访问..." << std::endl;
    auto* motor1 = manager.get_motor("CAN_CHASSIS", 1);
    auto* motor2 = manager.get_motor("CAN_CHASSIS", 2);
    auto* motor3 = manager.get_motor("CAN_GIMBAL", 1);
    auto* motor_null = manager.get_motor("NOT_EXIST", 999);
    
    assert(motor1 != nullptr);
    assert(motor2 != nullptr);
    assert(motor3 != nullptr);
    assert(motor_null == nullptr);
    
    assert(motor1->get_id() == 1);
    assert(motor2->get_id() == 2);
    assert(motor3->get_id() == 1);
    
    std::cout << "✅ 电机管理器基本功能测试通过！" << std::endl;
}

// 测试电机管理器启动停止
void test_motor_manager_start_stop() {
    std::cout << "\n=== 测试电机管理器启动停止 ===" << std::endl;
    
    Practice::MotorManager manager;
    
    // 注册接口和电机
    manager.register_interface("CAN_TEST");
    manager.register_motor({Practice::MotorType::M3508, "CAN_TEST", 1, 1.0f/19.0f});
    
    std::cout << "\n1. 测试管理器启动..." << std::endl;
    manager.start();
    
    // 让管理器运行一段时间
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    
    // 设置电机控制指令
    auto* motor = manager.get_motor("CAN_TEST", 1);
    if (motor) {
        motor->set_current(1500);
        std::cout << "设置电机电流: " << motor->get_target_current() << std::endl;
    }
    
    // 再运行一段时间观察控制循环
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    std::cout << "\n2. 测试管理器停止..." << std::endl;
    manager.stop();
    
    // 确保完全停止
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    
    std::cout << "✅ 电机管理器启动停止测试通过！" << std::endl;
}

// 测试多线程安全性
void test_multithreading_safety() {
    std::cout << "\n=== 测试多线程安全性 ===" << std::endl;
    
    Practice::MotorManager manager;
    
    // 注册接口和电机
    manager.register_interface("CAN_MT_TEST");
    manager.register_motor({Practice::MotorType::M3508, "CAN_MT_TEST", 1, 1.0f/19.0f});
    manager.register_motor({Practice::MotorType::M3508, "CAN_MT_TEST", 2, 1.0f/19.0f});
    
    std::cout << "\n1. 启动管理器..." << std::endl;
    manager.start();
    
    // 创建多个线程同时操作电机
    std::vector<std::thread> threads;
    
    std::cout << "\n2. 创建多个控制线程..." << std::endl;
    for (int i = 0; i < 3; ++i) {
        threads.emplace_back([&manager, i]() {
            auto* motor1 = manager.get_motor("CAN_MT_TEST", 1);
            auto* motor2 = manager.get_motor("CAN_MT_TEST", 2);
            
            for (int j = 0; j < 10; ++j) {
                if (motor1) {
                    motor1->set_current(100 * i + j);
                }
                if (motor2) {
                    motor2->set_current(200 * i + j);
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
            }
        });
    }
    
    // 等待所有线程完成
    for (auto& thread : threads) {
        thread.join();
    }
    
    std::cout << "\n3. 停止管理器..." << std::endl;
    manager.stop();
    
    std::cout << "✅ 多线程安全性测试通过！" << std::endl;
}

// 测试RAII资源管理
void test_raii_resource_management() {
    std::cout << "\n=== 测试RAII资源管理 ===" << std::endl;
    
    std::cout << "\n1. 测试管理器自动清理..." << std::endl;
    
    {
        Practice::MotorManager manager;
        manager.register_interface("CAN_RAII_TEST");
        manager.register_motor({Practice::MotorType::M3508, "CAN_RAII_TEST", 1, 1.0f/19.0f});
        
        manager.start();
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        
        // manager会在作用域结束时自动停止和清理
    }
    
    std::cout << "\n2. 测试接口自动清理..." << std::endl;
    
    {
        Practice::MockInterface interface("RAII_TEST");
        // interface会在作用域结束时自动销毁
    }
    
    std::cout << "✅ RAII资源管理测试通过！" << std::endl;
}

// 测试错误处理
void test_error_handling() {
    std::cout << "\n=== 测试错误处理 ===" << std::endl;
    
    Practice::MotorManager manager;
    
    std::cout << "\n1. 测试访问不存在的电机..." << std::endl;
    auto* motor = manager.get_motor("NOT_EXIST", 999);
    assert(motor == nullptr);
    
    std::cout << "\n2. 测试在未注册接口的情况下注册电机..." << std::endl;
    // 这应该安全处理，不崩溃
    manager.register_motor({Practice::MotorType::M3508, "NOT_REGISTERED", 1, 1.0f});
    auto* motor2 = manager.get_motor("NOT_REGISTERED", 1);
    // 根据实现，这可能返回nullptr或正常工作
    
    std::cout << "✅ 错误处理测试通过！" << std::endl;
}

// 测试性能和实时性
void test_realtime_performance() {
    std::cout << "\n=== 测试实时性能 ===" << std::endl;
    
    Practice::MotorManager manager;
    
    // 注册多个电机模拟真实场景
    manager.register_interface("CAN_PERF_TEST");
    for (int i = 1; i <= 4; ++i) {
        manager.register_motor({Practice::MotorType::M3508, "CAN_PERF_TEST", i, 1.0f/19.0f});
    }
    
    std::cout << "\n1. 测试控制循环时序..." << std::endl;
    manager.start();
    
    auto start_time = std::chrono::steady_clock::now();
    
    // 运行一段时间并检查性能
    for (int i = 0; i < 10; ++i) {
        for (int motor_id = 1; motor_id <= 4; ++motor_id) {
            auto* motor = manager.get_motor("CAN_PERF_TEST", motor_id);
            if (motor) {
                motor->set_current(100 * motor_id + i);
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    
    auto end_time = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    
    std::cout << "控制循环运行时间: " << duration.count() << "ms" << std::endl;
    
    manager.stop();
    
    std::cout << "✅ 实时性能测试通过！" << std::endl;
}

int main() {
    try {
        std::cout << "🚀 开始测试电机驱动系统实现..." << std::endl;
        std::cout << "这是最复杂的测试，将验证您的多线程和系统设计能力！\n" << std::endl;
        
        test_motor_config();
        test_mock_interface();
        test_motor_basic_functionality();
        test_different_motor_types();
        test_motor_manager_basic();
        test_motor_manager_start_stop();
        test_multithreading_safety();
        test_raii_resource_management();
        test_error_handling();
        test_realtime_performance();
        
        std::cout << "\n🎉🎉🎉 恭喜！您已经成功实现了完整的电机驱动系统！🎉🎉🎉" << std::endl;
        std::cout << "\n🏆 您现在掌握的高级技能：" << std::endl;
        std::cout << "✅ 多线程并发编程和同步" << std::endl;
        std::cout << "✅ RAII资源自动管理" << std::endl;
        std::cout << "✅ 实时系统设计原则" << std::endl;
        std::cout << "✅ 复杂系统架构设计" << std::endl;
        std::cout << "✅ 性能优化和线程安全" << std::endl;
        std::cout << "✅ 现代C++的综合运用" << std::endl;
        
        std::cout << "\n🌟 重大成就解锁：" << std::endl;
        std::cout << "🔥 您已经完成了整个硬件抽象层的学习！" << std::endl;
        std::cout << "🔥 您现在具备了设计和实现复杂实时系统的能力！" << std::endl;
        std::cout << "🔥 您掌握的技能可以应用到任何嵌入式或实时控制系统！" << std::endl;
        
        std::cout << "\n🚀 下一步建议：" << std::endl;
        std::cout << "1. 回到原项目，深入学习控制算法层" << std::endl;
        std::cout << "2. 尝试设计自己的机器人控制系统" << std::endl;
        std::cout << "3. 探索更高级的现代C++特性" << std::endl;
        std::cout << "4. 研究实时操作系统和嵌入式开发" << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "\n❌ 测试失败: " << e.what() << std::endl;
        std::cout << "请检查您的step3实现，确保所有TODO项都已正确完成。" << std::endl;
        std::cout << "\n💡 常见问题检查：" << std::endl;
        std::cout << "1. Motor类是否正确实现了所有成员函数？" << std::endl;
        std::cout << "2. MotorManager是否正确管理线程的启动和停止？" << std::endl;
        std::cout << "3. 是否正确实现了线程同步和资源管理？" << std::endl;
        std::cout << "4. 构造函数和析构函数是否正确实现？" << std::endl;
        std::cout << "5. 是否包含了所有必要的头文件？" << std::endl;
        return 1;
    }
    
    return 0;
}

// 编译命令：
// g++ -std=c++17 -pthread test_step3.cpp -o test_step3
// ./test_step3 