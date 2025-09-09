import matplotlib.pyplot as plt
import matplotlib.dates as mdates
import time
import os
from datetime import datetime
import threading

# 全局变量用于存储数据和控制线程
log_data = {}
log_lock = threading.Lock()
is_running = True

# 解析日志行
def parse_log_line(line):
    """
    解析日志行，提取时间戳、变量名和值。
    日志格式: [YYYY-MM-DD HH:MM:SS] var_name: value
    """
    try:
        parts = line.split("] ")
        if len(parts) < 2:
            return None, None, None
        
        timestamp_str = parts[0][1:]
        # 使用 strptime 将字符串时间转换为 datetime 对象
        timestamp = datetime.strptime(timestamp_str, "%Y-%m-%d %X")
        
        message_parts = parts[1].split(": ")
        if len(message_parts) < 2:
            return None, None, None
            
        variable_name = message_parts[0].strip()
        value = float(message_parts[1].strip())
        
        return timestamp, variable_name, value
    except (ValueError, IndexError):
        # 忽略无法解析的行
        return None, None, None

# 线程函数：读取日志并更新数据
def read_logs_and_update_data(log_file_path):
    """
    在一个单独的线程中持续读取日志文件的新行，并更新全局数据。
    """
    global is_running
    print(f"正在监控日志文件: {log_file_path}")
    
    with open(log_file_path, 'r', encoding='utf-8') as f:
        # 初始时，将文件指针移到文件末尾，只读取新写入的内容
        f.seek(0, os.SEEK_END)
        
        while is_running:
            new_line = f.readline()
            if new_line:
                timestamp, var, value = parse_log_line(new_line)
                if timestamp and var and value is not None:
                    # 使用锁确保线程安全
                    with log_lock:
                        if var not in log_data:
                            log_data[var] = {'timestamps': [], 'values': []}
                        log_data[var]['timestamps'].append(timestamp)
                        log_data[var]['values'].append(value)
            else:
                # 如果没有新行，则暂停一小段时间再继续
                time.sleep(0.5)

# 封装好的绘图方法
def visualize_log(log_file_path):
    """
    根据传入的日志文件路径，实时可视化日志数据。
    """
    global is_running
    
    # 检查并创建日志目录
    log_dir = os.path.dirname(log_file_path)
    if not os.path.exists(log_dir):
        os.makedirs(log_dir)
        print(f"已创建日志目录: {log_dir}")

    # 检查并清空日志文件
    if os.path.exists(log_file_path):
        print(f"日志文件 {log_file_path} 已存在，正在清空内容...")
        with open(log_file_path, 'w') as f:
            f.truncate(0)
    else:
        print(f"日志文件 {log_file_path} 不存在，正在创建...")
        open(log_file_path, 'a').close()

    # 启动日志读取线程
    reader_thread = threading.Thread(target=read_logs_and_update_data, args=(log_file_path,))
    reader_thread.daemon = True # 主程序退出时，线程也随之退出
    reader_thread.start()

    # 设置绘图风格和初始图表
    plt.style.use('dark_background')
    fig, ax = plt.subplots(figsize=(10, 6))
    ax.set_title(f"实时日志可视化 - {os.path.basename(log_file_path)}")
    ax.set_xlabel("时间")
    ax.set_ylabel("变量值")
    ax.grid(True)
    
    # 启用交互模式，以便实时更新图表
    plt.ion()
    
    try:
        while is_running:
            with log_lock:
                if log_data:
                    # 清除旧的绘图，重新绘制
                    ax.cla()
                    ax.set_title(f"实时日志可视化 - {os.path.basename(log_file_path)}")
                    ax.set_xlabel("时间")
                    ax.set_ylabel("变量值")
                    ax.grid(True)
                    
                    # 遍历所有变量并绘制它们的趋势线
                    for var, data in log_data.items():
                        ax.plot(data['timestamps'], data['values'], label=var, marker='o', linestyle='-')
                    
                    # 格式化X轴时间显示
                    ax.xaxis.set_major_formatter(mdates.DateFormatter('%H:%M:%S'))
                    fig.autofmt_xdate()
                    
                    ax.legend()
                
                plt.draw()
                plt.pause(0.5) # 每0.5秒更新一次图表
            
    except KeyboardInterrupt:
        print("\n程序已终止。")
    finally:
        is_running = False
        reader_thread.join()
        plt.ioff()
        plt.close(fig)

# 主程序入口，调用封装好的方法
if __name__ == "__main__":
    default_log_file = "log/my_log_file.txt"
    visualize_log(default_log_file)
