# 🎯 硬件抽象层实践学习指南

## 📖 学习方法

**重要原则：不要急于看答案！** 先尝试自己实现，遇到困难时再查阅资料或寻求帮助。

## 🚀 第一步：IO管理器 (2-3天)

### 学习目标
- 理解C++模板系统
- 掌握智能指针的使用
- 学会操作符重载
- 理解完美转发机制

### 实现任务
打开 `step1_io_manager.hpp`，实现 `IOManager` 类的所有TODO项：

```cpp
template<typename DeviceType>
class IOManager {
private:
    std::unordered_map<std::string, std::unique_ptr<DeviceType>> devices_;
    
public:
    // 1. 实现[]操作符 - 返回设备指针，找不到返回nullptr
    DeviceType* operator[](const std::string& name) const {
        // TODO: 在devices_中查找name，返回对应的裸指针
    }
    
    // 2. 完美转发设备注册
    template<typename... Args>
    void register_device(const std::string& name, Args&&... args) {
        // TODO: 创建设备对象，存储到devices_中
        // 提示：使用std::make_unique和std::forward
    }
    
    // 3. 简单的查询函数
    size_t size() const {
        // TODO: 返回设备数量
    }
};
```

### 关键知识点
- **模板特化**: `template<typename DeviceType>`
- **智能指针**: `std::unique_ptr` 自动内存管理
- **完美转发**: `std::forward<Args>(args)...` 保持参数类型
- **可变参数模板**: `typename... Args` 支持任意参数

### 测试方法
```bash
g++ -std=c++17 test_step1.cpp -o test_step1
./test_step1
```

---

## ⚡ 第二步：回调系统 (3-4天)

### 学习目标
- 掌握 `std::function` 的使用
- 理解Lambda表达式的捕获机制
- 学会可变参数模板的实际应用
- 理解事件驱动编程

### 实现任务
打开 `step2_callback_system.hpp`，实现 `CallbackManager` 类：

```cpp
template<typename KeyType, typename... ArgTypes>
class CallbackManager {
private:
    std::map<KeyType, std::function<void(const ArgTypes&...)>> callbacks_;
    
public:
    // 1. 注册回调函数
    void register_callback(const KeyType& key, 
                          const std::function<void(const ArgTypes&...)>& callback) {
        // TODO: 将回调函数存储到callbacks_中
    }
    
    // 2. 支持Lambda的注册（更灵活）
    template<typename Callable>
    void register_callback(const KeyType& key, Callable&& callback) {
        // TODO: 将任意可调用对象转换为std::function并存储
    }
    
    // 3. 触发回调
    void trigger(const KeyType& key, const ArgTypes&... args) {
        // TODO: 查找并调用对应的回调函数
        // 注意：需要检查回调是否存在
    }
};
```

### 关键知识点
- **类型擦除**: `std::function` 统一不同类型的可调用对象
- **参数包展开**: `args...` 传递可变数量参数
- **Lambda捕获**: `[&]`, `[=]` 等不同捕获方式
- **SFINAE**: 模板参数推导和替换

### 实际应用场景
这种设计在原项目中用于CAN消息处理：
```cpp
can_interface->register_callback_key(0x201, [](const can_frame& frame) {
    // 处理电机反馈数据
});
```

---

## 🚗 第三步：电机驱动系统 (5-7天)

### 学习目标
- 综合运用前两步的所有知识
- 掌握多线程编程和同步
- 理解RAII资源管理
- 学会设计模式的实际应用

### 实现任务
这是最复杂的部分，分为几个子任务：

#### 3.1 实现Motor类
```cpp
class Motor {
public:
    explicit Motor(const MotorConfig& config) {
        // TODO: 初始化电机参数
        // 根据type设置reduction_ratio_
        // 生成唯一的name_
    }
    
    // 禁用复制和移动（资源独占）
    Motor(const Motor&) = delete;
    Motor& operator=(const Motor&) = delete;
    
    void set_current(int16_t current) {
        // TODO: 设置目标电流
        target_current_ = current;
    }
    
    // TODO: 实现其他成员函数
};
```

#### 3.2 实现MotorManager类
```cpp
class MotorManager {
private:
    std::unordered_map<std::string, std::unique_ptr<MockInterface>> interfaces_;
    std::unordered_map<std::string, std::vector<std::unique_ptr<Motor>>> motor_groups_;
    std::unique_ptr<std::thread> control_thread_;
    std::mutex motor_mutex_;
    std::atomic<bool> running_{false};
    
public:
    void start() {
        // TODO: 启动控制线程
        running_ = true;
        control_thread_ = std::make_unique<std::thread>(&MotorManager::control_loop, this);
    }
    
    void control_loop() {
        // TODO: 主控制循环
        // 1ms周期发送电机控制指令
        while (running_) {
            std::lock_guard<std::mutex> lock(motor_mutex_);
            // 遍历所有接口和电机，发送控制指令
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
};
```

### 关键知识点
- **RAII**: 资源获取即初始化，析构函数自动清理
- **线程同步**: `std::mutex`, `std::lock_guard`
- **原子操作**: `std::atomic<bool>` 线程安全标志
- **智能指针**: `std::unique_ptr` 管理资源生命周期

---

## 🎓 进阶挑战 (可选)

完成基本实现后，可以尝试以下进阶功能：

1. **异常安全**: 添加异常处理和资源清理
2. **性能优化**: 使用对象池减少内存分配
3. **观察者模式**: 实现电机状态变化通知
4. **配置系统**: 支持从文件加载电机配置

## 📚 学习资源

### C++参考资料
- [cppreference.com](https://cppreference.com) - 最权威的C++参考
- [Modern C++ Features](https://github.com/AnthonyCalandra/modern-cpp-features)

### 推荐书籍
- 《Effective Modern C++》- Scott Meyers
- 《C++ Concurrency in Action》- Anthony Williams

## ❓ 常见问题

### Q: 编译错误怎么办？
A: 仔细阅读错误信息，大多数模板错误都很详细。常见问题：
- 忘记包含头文件
- 模板参数不匹配
- const正确性问题

### Q: 如何调试模板代码？
A: 
1. 使用简单的类型先测试
2. 编译器的错误信息通常很详细
3. 使用`static_assert`进行编译时检查

### Q: 什么时候可以看原项目代码？
A: 建议在自己实现完每一步后，再对比原项目的实现。这样能更好地理解设计思想。

---

## 🏆 学习检验

完成所有步骤后，您应该能够：

✅ 理解模板的基本语法和特化机制  
✅ 熟练使用智能指针管理资源  
✅ 掌握回调系统的设计和实现  
✅ 理解多线程编程的基本概念  
✅ 能够设计简单的硬件抽象层  

**记住：编程是实践的艺术，只有写出来的代码才是真正掌握的知识！** 