CH10X系列用户手册
IMU/VRU/AHRS姿态测量模块, Rev 1.0
适用于 CH102(M)/CH104(M)/CH108M
![image](https://cdn-mineru.openxlab.org.cn/result/2026-03-01/b3f3e820-ccd3-430d-85c3-8f0001674093/0a3180ee7754cd48f5113ca194c5d1e3f798dfaf9b120ffe71e337869b1a3aa6.jpg)

文件：CH10X_UM_CN
技术支持：support@hipnuc.com
属性：公开
网站：www.hipnuc.com
HiPNUC R 
超核电子
$\circledcirc$ 2016-2022,北京超核电子科技有限公司版权所有，本文档所包含的信息在没有通知的情况下可能会发生更改。
文档变更历史
<table><tr><td>版本</td><td>日期</td><td>作者</td><td>变更内容</td></tr><tr><td>V1.0</td><td>2022年6月18日</td><td>HiPNUC</td><td>初始版本</td></tr></table>
# CH10X系列用户手册
# 1 产品简介
1.1 算法架构
1.2 主要应用场景:
1.3 产品特点
1.3.1 出厂标定
1.3.2 可输出多种数据类型
1.3.3 数据接口
1.3.4 其他
# 1.4 辅助开发套件
1.4.1 屏蔽双绞线
1.4.2 上位机软件CHCenter
# 1.5 订购信息
# 2 机械与电气特性
2.1 CH10X尺寸图
2.2 CH10X系列航插引脚定义
2.3 线缆-USB接口
2.4 线缆-CAN/232/485接口
2.5 电气与机械参数
2.6 坐标系定义
# 3 性能指标
3.1 姿态角输出精度
3.2 陀螺仪
3.3 加速度计
3.4 磁传感器参数
3.5 关键电气参数
# 4 标定
4.1 出厂标定
4.2 启动标定
4.3 磁力计校准
# 4.3.1 磁干扰分类
4.3.1.1 空间磁场干扰(干扰不随传感器位姿改变而改变)
4.3.1.2 传感器坐标系下的干扰(干扰随传感器位姿改变而改变)
# 4.3.2 地磁使用注意事项
# 4.3.3 案例分析
# 5 关于模式说明
5.1 型号的模式区别
5.2 不同模式的区别
# 6 串口(RS232/USB VCOM)通讯协议
6.1 串口数据包
6.2 产品支持数据包列表
6.2.1 0X91( IMUSOL) 
6.3 数据帧结构示例
6.3.1 数据帧配置为 0x91 数据包
7 AT指令
7.1 AT指令列表
7.2 指令详解
7.2.1 AT+ID 
7.2.2 AT+INFO 
7.2.3 AT+ODR 
7.2.4 AT+BAUD 
7.2.5 AT+EOUT 
7.2.6 AT+RST 
7.2.7 AT+SETYAW 
7.2.8 AT+MODE 
7.2.9 AT+URFR 
7.2.10 AT+RSTORT 
8 CAN通讯协议
8.1 CANopen协议
8.1.1 CANopen 默认设置
8.1.2 CANopen TPDO 
8.1.3 CANopen接口常用命令
8.1.3.1 使能数据输出(开启异步触发)
8.1.3.2 修改CAN波特率
8.1.3.3 修改节点ID
8.1.3.4 切换CAN协议
8.1.3.5 修改/关闭/开启数据输出速率
8.1.3.6 开启/关闭站点
8.1.3.7 配置TPDO为同步模式
9 包装
9.1 包装盒
9.2 外箱标签
9.3 产品标签
10 附录A - 固件升级与恢复出厂设置
11 附录B-技术支持
# 1. 产品简介
CH10X系列是利用高性能、小体积MEMS惯性器件感知物体姿态信息的姿态感知系统，它集成了惯性测量单元（IMU）、磁力计和一款搭载扩展卡尔曼融合算法（EKF）的微控制器。可输出经过传感器融合算法计算得到的基于当地地理坐标的三维方位数据，包含有绝对参考的航向角，俯仰角和横滚角。同时也可以输出校准过的原始的传感器数据。IP68级防水的外壳式的封装可以很方便的集成到用户的系统，CH10X系列主要包含CH102(M)，CH104(M)，CH108M三个系列产品：
<table><tr><td>型号</td><td>CH102</td><td>CH102M</td><td>CH104</td><td>CH104M</td><td>CH108</td><td>CH108M</td></tr><tr><td>级别</td><td colspan="2">工业级</td><td colspan="2">工业级</td><td colspan="2">战术级</td></tr><tr><td>IMU阵列</td><td colspan="2">2</td><td colspan="2">4</td><td colspan="2">8</td></tr><tr><td>地磁</td><td>-</td><td>●</td><td>-</td><td>●</td><td>-</td><td>●</td></tr></table>
# 1.1算法架构
CH10X采用超核最新的融合算法引擎，该算法引擎采用了自主研发的扩展卡尔曼滤波和IMU噪声动态分析技术，可以满足高动态下姿态角的精度，并且减小航向角的漂移。算法架构如图
![image](https://cdn-mineru.openxlab.org.cn/result/2026-03-01/b3f3e820-ccd3-430d-85c3-8f0001674093/9b96695cc89aaa0a14203e23754b1a913acf8500d5842b2b41ff5db7e656b3db.jpg)

# 1.2主要应用场景:
CH10X系列可以精确地感知移动设备的俯仰（Pitch）、横滚（Roll）、航向（Yaw）等姿态信息，比如AR/VR，自动导引小车（AGV），巡检机器人、无人机等应用领域。它可与激光雷达（Lidar）、视觉（Camera）等导航方案形成优势互补，增强设备的导航精度，并减小对外界参考物体的依赖。典型的应用市场如下：
自动导引小车（AGV/AMR）
服务机器人
巡检机器人
组合导航（INS）
工程机械
智慧农机 
倾角检测
# 1.3 产品特点
# 1.3.1 出厂标定
MEMS惯性传感器由于制造工艺、材质等问题，有一些共性的误差源，比如零偏（Bias）、比例因子（ScaleFactor）、跨轴（CrossAxis），这些误差会在出厂之前会经过严格的标定程序，减少这些误差的影响，标定参数会保存在每一个产品中。
# 1.3.2可输出多种数据类型
CH10X拥有数据预处理和基于四元数的扩展卡尔曼（EKF）数据融合引擎，可以为用户提供原始数据，姿态数据、四元数、温度等数据信息。详细介绍如下
加速度（Accleration）含有重力并标定后的加速度
角速度（Angular Rates） 标定后的角速度
姿态角（Attitude） 俯仰（Pitch）、航滚（Roll）、航向（Yaw）
四元数（Quaternion） 姿态四元数
温度（Temprature）传感器内部温度
# 1.3.3 数据接口
为更好地满足CH10X系列在各种场景下的应用，我们为CH10X系列配备了USB、232、CAN、485等数据接口，其中USB最高帧率可以达到400Hz,232最高速率400Hz ,CAN2.0最高帧率200Hz。并且提供完善的Linux、ROS1/ROS2、Win、MCU驱动。
CH10X系列CAN遵循CANopen协议或SAE J1939协议
如果使用232接口并且需要400Hz传输频率，那么用户的232芯片的波特率需要设置为921600的波特率
# 1.3.4 其他
制造优势： 选用高性能惯性传感器，并且采用自动化批量标定与测试，保证产品一致性。
软件优势：丰富的行业经验，融合先进的算法，可以在静止检测、慢速检测、快速启动，实时估算零偏、地磁自动校准、抗磁场干扰等方面，具有优势表现。
# 1.4辅助开发套件
# 1.4.1 屏蔽双绞线
![image](https://cdn-mineru.openxlab.org.cn/result/2026-03-01/b3f3e820-ccd3-430d-85c3-8f0001674093/669379954ad2fa06a8361df21ca2acb5f8bd42fa74e3d868a49688f3e7e9a1bf.jpg)

![image](https://cdn-mineru.openxlab.org.cn/result/2026-03-01/b3f3e820-ccd3-430d-85c3-8f0001674093/66afa4c4bbadbf99e5a2028ce0e783fd8019688225e815a5401c6bded10f58ff.jpg)

默认1.5m、0.5m如需其他长度可以与我们联系定做。
# 1.4.2 上位机软件CHCenter
CHCenter是我们为了用户快速评估产品而开发的一款PC端软件，可以自由的运行在WIN/Linux之上。CHCenter具有如下特点：
数据显示
. 数据记录
数据分析
产品参数配置
固件升级
CH CH Center V1.2.3 
□ 
连接设备 工具图表语言帮助

仪表板

![image](https://cdn-mineru.openxlab.org.cn/result/2026-03-01/b3f3e820-ccd3-430d-85c3-8f0001674093/bf383e82873e9a53a66be31703e71bbf019c5b16ccd2ff161e6a03b4f0108158.jpg)

![image](https://cdn-mineru.openxlab.org.cn/result/2026-03-01/b3f3e820-ccd3-430d-85c3-8f0001674093/535af7ed024261f6ea2121547d4d81dbb025de209340052ebcd7644b5c4d6737.jpg)

# 数据包信息
[Serial] 
输出协议 91
帧率：100 Hz

数据显示

<table><tr><td>Pressure[Pa] :</td><td>0.0</td><td></td><td></td><td></td></tr><tr><td>ID :</td><td>1</td><td></td><td></td><td></td></tr><tr><td>TimeStamp :</td><td>23291</td><td></td><td></td><td></td></tr><tr><td></td><td>X</td><td>Y</td><td>Z</td><td></td></tr><tr><td>Accelerometer[G] :</td><td>-0.020</td><td>-0.003</td><td>0.997</td><td></td></tr><tr><td>Gyroscope[°/s] :</td><td>-0.045</td><td>-0.006</td><td>0.061</td><td></td></tr><tr><td>Magnetometer[uT] :</td><td>1.683</td><td>-28.833</td><td>-34.542</td><td></td></tr><tr><td>Euler Angle[°] :</td><td>1.176</td><td>-0.138</td><td>0.016</td><td></td></tr><tr><td>Position[m] :</td><td>0.000</td><td>0.000</td><td>0.000</td><td></td></tr><tr><td></td><td>W</td><td>X</td><td>Y</td><td>Z</td></tr><tr><td>Quaternion :</td><td>1.000</td><td>-0.001</td><td>0.010</td><td>0.000</td></tr></table>
COM3| 115200 Connected... 
T:0.00KB/s(0%) R:8.01KB/s(71%) 
# 1.5 订购信息
<table><tr><td>型号</td><td>P/N</td><td>规格描述</td><td>级别</td></tr><tr><td>CH102_USB</td><td>AM07-17001</td><td>6轴阵列轴姿态传感器 IMU VRU 外壳封装 USB输出</td><td>工业级</td></tr><tr><td>CH102M_USB</td><td>AM07-17002</td><td>9轴阵列姿态传感器 IMU VRU AHRS外壳封装 USB输出</td><td>工业级</td></tr><tr><td>CH102_CAN_232</td><td>AM07-17003</td><td>6轴阵列姿态传感器 IMU VRU 外壳封装 CAN/232输出</td><td>工业级</td></tr><tr><td>CH102M_CAN_232</td><td>AM07-17004</td><td>9轴阵列轴姿态传感器 IMU VRU AHRS外壳封装 CAN/232输出</td><td>工业级</td></tr><tr><td>CH102_485</td><td>AM07-17005</td><td>6轴阵列姿态传感器 IMU VRU 外壳封装 485输出</td><td>工业级</td></tr><tr><td>CH102M_485</td><td>AM07-17006</td><td>9轴阵列姿态传感器 IMU VRU AHRS外壳封装 485输出</td><td>工业级</td></tr><tr><td>CH104_USB</td><td>AM07-17007</td><td>6轴阵列轴姿态传感器 IMU VRU 外壳封装 USB输出</td><td>工业级</td></tr><tr><td>CH104M_USB</td><td>AM07-17008</td><td>9轴阵列姿态传感器 IMU VRU AHRS外壳封装 USB输出</td><td>工业级</td></tr><tr><td>CH104_CAN_232</td><td>AM07-17009</td><td>6轴阵列轴姿态传感器 IMU VRU 外壳封装 CAN/232输出</td><td>工业级</td></tr><tr><td>CH104M_CAN_232</td><td>AM07-17010</td><td>9轴阵列轴姿态传感器 IMU VRU AHRS外壳封装 CAN/232输出</td><td>工业级</td></tr><tr><td>CH104_485</td><td>AM07-17011</td><td>6轴阵列姿态传感器 IMU VRU 外壳封装 485输出</td><td>工业级</td></tr><tr><td>CH104M_485</td><td>AM07-17012</td><td>9轴阵列姿态传感器 IMU VRU AHRS外壳封装 485输出</td><td>工业级</td></tr><tr><td>CH108M_USB</td><td>AM07-17013</td><td>9轴阵列姿态传感器 IMU VRU AHRS外壳封装 USB输出</td><td>战术级</td></tr><tr><td>CH108M_CAN_232</td><td>AM07-17014</td><td>9轴阵列轴姿态传感器 IMU VRU AHRS外壳封装 CAN/232输出</td><td>战术级</td></tr><tr><td>CH108M_485</td><td>AM07-17015</td><td>9轴阵列姿态传感器 IMU VRU AHRS外壳封装 485输出</td><td>战术级</td></tr></table>
# 2. 机械与电气特性
# 2.1 CH10X尺寸图
所有标注单位均为mm
![image](https://cdn-mineru.openxlab.org.cn/result/2026-03-01/b3f3e820-ccd3-430d-85c3-8f0001674093/709ed56aa80ac1972ef2b5e30a3bee81a4ba56d4c67d88ef42ea167b875d4f44.jpg)

![image](https://cdn-mineru.openxlab.org.cn/result/2026-03-01/b3f3e820-ccd3-430d-85c3-8f0001674093/63827e2a6f525f5cfbaef3fa7536ee543e9064e01b18221bb53977e32b77a2ed.jpg)

![image](https://cdn-mineru.openxlab.org.cn/result/2026-03-01/b3f3e820-ccd3-430d-85c3-8f0001674093/1bd154980b5a39d4466e1534183c1c8a006133eba0d5a573dd3a12351b53f5fc.jpg)

![image](https://cdn-mineru.openxlab.org.cn/result/2026-03-01/b3f3e820-ccd3-430d-85c3-8f0001674093/526e7bcb92a4dae9325c7786166176acd9b442a605422fa7ed258720f7d9c38b.jpg)

# 2.2 CH10X系列航插引脚定义
![image](https://cdn-mineru.openxlab.org.cn/result/2026-03-01/b3f3e820-ccd3-430d-85c3-8f0001674093/ce6d62792b947a3344a765a03f770c19186bffe73f09d2dda31f48b534c6a5cc.jpg)

<table><tr><td>序号</td><td>颜色</td><td>CH10X_USB(虚拟串口)</td><td>CH10X_CAN_232</td><td>CH10X_485</td></tr><tr><td>1</td><td>红</td><td>VCC_USB(5V)</td><td>VCC(5-36V)</td><td>VCC(5-36V)</td></tr><tr><td>2</td><td>黑</td><td>GND</td><td>GND</td><td>GND</td></tr><tr><td>3</td><td>黄</td><td>DN</td><td>H</td><td>-</td></tr><tr><td>4</td><td>绿</td><td>DP</td><td>L</td><td>-</td></tr><tr><td>5</td><td>白</td><td>-</td><td>TXD(RS232)</td><td>A(RS485)</td></tr><tr><td>6</td><td>棕</td><td>-</td><td>RXD(RS232)</td><td>B(RS485)</td></tr><tr><td>-</td><td>黑(无标签)</td><td>-</td><td>线缆屏蔽线</td><td>线缆屏蔽线</td></tr><tr><td colspan="5">1.CH10X_USB系列姿态传感器配专用的USB转接线，可以即插即用,VCC_USB供电电压绝对最大值5.2V</td></tr><tr><td colspan="5">2.线缆屏蔽线:一般可悬空,部分工业场景需要接大地。</td></tr></table>

# 2.3 线缆-USB接口
传感器端为6芯M8A母头圆形防水连接器，连接线端为1.5m的USB转6芯M8A公直头成型线，规格如下图：单位为mm
![image](https://cdn-mineru.openxlab.org.cn/result/2026-03-01/b3f3e820-ccd3-430d-85c3-8f0001674093/b694ec99fe37d9e370c763f14c1c3314053be42b4e8b0337539a1b40830246d2.jpg)

# 2.4 线缆-CAN/232/485接口
传感器端为6芯M8A母头圆形防水连接器，连接线端为1.5m的6芯M8A公直头成型线，规格如下图：单位为mm
![image](https://cdn-mineru.openxlab.org.cn/result/2026-03-01/b3f3e820-ccd3-430d-85c3-8f0001674093/c353cc93f7ef570fc80a0cdd4e2c0b60b9a8f8218f5ead97bf0b73226ff1af51.jpg)

上述默认线长均为1.5m，如需定制其他长度，请与我们联系
# 2.5 电气与机械参数
<table><tr><td>类型</td><td>条件</td><td>最小值</td><td>典型值</td><td>最大值</td><td>单位</td></tr><tr><td>电源输入VCC_USB</td><td>USB接口</td><td>4.9</td><td>5</td><td>5.2</td><td>V</td></tr><tr><td>电源输入VCC</td><td>CAN/232/485接口</td><td>5</td><td>-</td><td>36</td><td>V</td></tr><tr><td>DN、DP</td><td>USB接口</td><td>-0.3</td><td>-</td><td>3.6</td><td>V</td></tr><tr><td>RS-232</td><td></td><td>-15</td><td>-</td><td>15</td><td>V</td></tr><tr><td>CAN</td><td></td><td>-36</td><td>-</td><td>36</td><td>V</td></tr><tr><td>CAN差分电压</td><td></td><td>1.5</td><td>-</td><td>3.0</td><td>V</td></tr><tr><td>RS-485</td><td></td><td>-12</td><td>-</td><td>12</td><td>V</td></tr><tr><td>功耗P</td><td>CH102(M)</td><td>-</td><td>-</td><td>206</td><td>mW</td></tr><tr><td>功耗P</td><td>CH104(M)</td><td>-</td><td>-</td><td>245</td><td>mW</td></tr><tr><td>功耗P</td><td>CH108M</td><td>-</td><td>-</td><td>410</td><td>mW</td></tr><tr><td>工作温度</td><td></td><td>-40</td><td>-</td><td>85</td><td>°C</td></tr><tr><td>振动冲击</td><td></td><td>-2000</td><td>-</td><td>2000</td><td>g</td></tr><tr><td>MTBF</td><td></td><td></td><td>50000</td><td></td><td>h</td></tr></table>

EMC防护标准
USB接口: IEC61000-4-2 ESD 15KV Air, 8KV contact compliance
CAN/232/485: IEC61000-4-2 ESD 30KV Air, 30KV contact compliance 
EFT, IEC 61000-4-4,50A (5/50ns) 
Lightning, IEC 61000-4-5 2nd edition, 10A 
# 2.6坐标系定义
载体系使用右-前-上(RFU)坐标系，地理坐标系使用东-北-天(ENU)坐标系。其中欧拉角旋转顺序为东-北-天-312(先转Z轴，再转X轴，最后转Y轴)旋转顺序。具体定义如下：
绕 Z 轴方向旋转: 航向角\Yaw\ps $( \psi )$ 范围: -180° - 180°
绕 X 轴方向旋转:俯仰角\Pitch\theta $( \theta )$ 范围: -90°-90°
绕 Y 轴方向旋转: 横滚角\Roll\phi( $\phi )$ 范围: -180°-180°
如果将模块视为飞行器的话。Y轴正方向应视为机头方向。当传感器系与惯性系重合时，欧拉角的理想输出为:Pitch $= 0 ^ { \circ }$ , Roll = 0°, Yaw = 0°
3. 性能指标
# 3.1 姿态角输出精度
为了提升模块的精度与稳定性，我们将CH10X系列传感器系统科学地排布在PCB上，并且搭载新一代EKF算法，使得模块性能得以充分发挥。
<table><tr><td>型号</td><td>CH102</td><td>CH102M</td><td>CH104</td><td>CH104M</td><td>CH108M</td></tr><tr><td>Pitch/Roll(static)</td><td>0.2°</td><td>0.2°</td><td>0.2°</td><td>0.2°</td><td>0.2°</td></tr><tr><td>Pitch/Roll(dynamic)</td><td>0.2°</td><td>0.2°</td><td>0.2°</td><td>0.2°</td><td>0.2°</td></tr><tr><td>Yaw(VRU)(static)^1^</td><td>0.3°</td><td>0.2°</td><td>0.2°</td><td>0.2°</td><td>0.2°</td></tr><tr><td>Yaw(VRU)(dynamic)^2^</td><td>5°</td><td>5°</td><td>3°</td><td>3°</td><td>2°</td></tr><tr><td>Yaw(magnetic)^3^</td><td>-</td><td>2°</td><td>-</td><td>2°</td><td>2°</td></tr><tr><td>Yaw(VRU)^4^</td><td>0.3°</td><td>0.3°</td><td>0.2°</td><td>0.2°</td><td>0.1°</td></tr><tr><td>气压计（高度）</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr></table>

$\textcircled{1}$ 模块在 $2 5 \textdegree$ 绝对静止1h测得


$\textcircled{2}$ 模块在机器人上运动1h测得，室温 $2 5 ^ { \circ } C$ ，1


$\textcircled{3}$ 地磁校准之后，周边无磁场干扰情况下室温 $2 5 \textdegree$ 测得，CH010M、CH020M、CH040MP需要配置为9轴模式 ,配置方法参考AT指令章节


$\textcircled{4}$ 转台转10圈取每圈平均值，室温 $2 5 \textdegree$ 

# 3.2 陀螺仪
<table><tr><td>参数</td><td>CH102(M)</td><td>CH104(M)</td><td>CH108M</td><td>备注</td></tr><tr><td>测量范围</td><td>±2000°/s</td><td>±2000°/s</td><td>±2000°/s</td><td>可配置</td></tr><tr><td>分辨率</td><td>0.01°/s</td><td>0.01°/s</td><td>0.01°/s</td><td></td></tr><tr><td>零偏稳定性</td><td>3.5°/hr</td><td>2.5°/hr</td><td>1.76°/hr</td><td>@25°C,1σ</td></tr><tr><td>零偏重复性</td><td>0.07°/s</td><td>0.05°/s</td><td>0.03°/s</td><td>@25°C,1σ</td></tr><tr><td>非正交误差</td><td>±0.1%</td><td>±0.1%</td><td>±0.1%</td><td>@25°C,1σ</td></tr><tr><td>随机游走</td><td>0.42°/√hr</td><td>0.3°/√hr</td><td>0.21°/√hr</td><td>@25°C,1σ</td></tr><tr><td>刻度非线性度</td><td>±0.1%</td><td>±0.1%</td><td>±0.1%</td><td>满量程时(最大)</td></tr><tr><td>刻度系数误差</td><td>800ppm</td><td>550ppm</td><td>300ppm</td><td>出厂前校准后</td></tr><tr><td>加速度敏感性</td><td>0.1°/s/g</td><td>0.1°/s/g</td><td>0.1°/s/g</td><td></td></tr><tr><td>Z轴全温变化最大值</td><td>0.003°/s/°C</td><td>0.003°/s/°C</td><td>0.003°/s/°C</td><td>-40°C - 85°C</td></tr></table>
陀螺Allan方差曲线(CH102)
![image](https://cdn-mineru.openxlab.org.cn/result/2026-03-01/b3f3e820-ccd3-430d-85c3-8f0001674093/f38e06b985bfd2aaa4c2d80ebec26fd293a2f0228a90b26d1e196fb98185ff0c.jpg)

陀螺Allan方差曲线(CH104)
![image](https://cdn-mineru.openxlab.org.cn/result/2026-03-01/b3f3e820-ccd3-430d-85c3-8f0001674093/a202ce2d7501e506d152d493618da5d4d3ac42d003ebbf8d7dc90a07086a5e02.jpg)


陀螺Allan方差曲线(CH108M)


Gyroscope -- Allan variance

![image](https://cdn-mineru.openxlab.org.cn/result/2026-03-01/b3f3e820-ccd3-430d-85c3-8f0001674093/b09bf088685e749aacc5289185dfe94cf2d7fbe9c84ea9d97709b4e4eb649733.jpg)

# 3.3加速度计
<table><tr><td>参数</td><td>CH102</td><td>CH104</td><td>CH108</td><td>备注</td></tr><tr><td>测量范围</td><td>±8G (1G = 1x 重力加速度)</td><td>±8G (1G = 1x 重力加速度)</td><td>±24G (1G = 1x 重力加速度)</td><td></td></tr><tr><td>分辨率</td><td>1uG</td><td>1uG</td><td>1uG</td><td></td></tr><tr><td>零偏稳定性</td><td>42uG</td><td>30uG</td><td>21uG</td><td>@25°C,1σ</td></tr><tr><td>零偏重复性</td><td>2.52mG</td><td>1.8mG</td><td>0.6mG</td><td>@25°C,1σ</td></tr><tr><td>非正交误差</td><td>±0.1%</td><td>±0.1%</td><td>±0.1%</td><td>±0.1%</td></tr><tr><td>随机游走</td><td>0.056m/s√h</td><td>0.04m/s√h</td><td>0.028m/s√h</td><td>@25°C,1σ</td></tr><tr><td>刻度系数误差</td><td>±0.3% (满量程时)</td><td>±0.3% (满量程时)</td><td>±0.2% (满量程时)</td><td>出厂前校准后</td></tr><tr><td>全温范围温度变化最大值</td><td>±0.45mg/°C</td><td>±0.45mg/°C</td><td>±0.45mg/°C</td><td>-40°C - 85°C</td></tr></table>
加速度Allan方差曲线(CH102)
![image](https://cdn-mineru.openxlab.org.cn/result/2026-03-01/b3f3e820-ccd3-430d-85c3-8f0001674093/243ff47a79978503683798a787e345df180def2edac667a72b9c95da0e9db003.jpg)


加速度Allan方差曲线(CH104)

![image](https://cdn-mineru.openxlab.org.cn/result/2026-03-01/b3f3e820-ccd3-430d-85c3-8f0001674093/a48be84b3d5fa97bdd07b9330e511fe0cc0334d37b20278f0db699e2eb76b886.jpg)


加速度Allan方差曲线(CH108)

![image](https://cdn-mineru.openxlab.org.cn/result/2026-03-01/b3f3e820-ccd3-430d-85c3-8f0001674093/5caeb19471fe516cfdbad5774a7c72b82d5c1206a27d61535fd6f66dbde45f50.jpg)

# 3.4磁传感器参数
<table><tr><td>参数</td><td>值</td></tr><tr><td>测量范围</td><td>±8G(Gauss)</td></tr><tr><td>非线性度</td><td>±0.1%</td></tr><tr><td>分辨率</td><td>0.25mG</td></tr></table>
# 3.5 关键电气参数
<table><tr><td>指标</td><td>CH102系列</td><td>CH104系列</td><td>CH108M</td></tr><tr><td colspan="4">机械与电气</td></tr><tr><td>尺寸</td><td></td><td>40x40x24.5mm(螺丝孔间距)</td><td></td></tr><tr><td>重量</td><td>73g</td><td>74g</td><td>76g</td></tr><tr><td>电压</td><td></td><td>USB接口: 5V CAN/232/485接口5-36V</td><td></td></tr><tr><td>功耗</td><td>206mW</td><td>245mW</td><td>410mW</td></tr><tr><td>启动时间与方式</td><td></td><td>静止启动时间&lt;1s</td><td></td></tr><tr><td colspan="4">接口-USB</td></tr><tr><td>USB(虚拟串口波特率)</td><td></td><td>9600/115200(默认)/230400/460800/921600</td><td></td></tr></table>
<table><tr><td>模块输出帧率</td><td>1/50/100(默认)/200/400Hz</td></tr><tr><td>传感器最大输出帧率</td><td>400Hz原始数据(加速度,角速度) 400Hz姿态角 100Hz磁场原始数据</td></tr><tr><td>接口-串口RS-232</td><td></td></tr><tr><td>串口波特率</td><td>9600/115200(默认)/230400/460800/921600</td></tr><tr><td>模块输出帧率</td><td>1/50/100(默认)/200/400Hz</td></tr><tr><td>传感器最大输出帧率</td><td>400Hz原始数据(加速度,角速度) 400Hz姿态角 100Hz磁场原始数据</td></tr><tr><td>接口-CAN2.0 默认无120Ω电阻</td><td></td></tr><tr><td>波特率</td><td>125K/250K/500K(默认)/1000K</td></tr><tr><td>协议</td><td>CANopen/SAEJ1939</td></tr><tr><td>最大数据速率</td><td>200Hz原始数据(加速度,角速度) 200Hz姿态角 100Hz磁场数据</td></tr><tr><td>接口-485</td><td></td></tr><tr><td>协议</td><td>Modbus/私有</td></tr><tr><td>最大数据速率</td><td>100Hz原始数据(加速度,角速度) 100Hz姿态角 100Hz磁场数据</td></tr><tr><td>环境</td><td></td></tr><tr><td>ESD</td><td>USB接口:IEC61000-4-2 ESD 15KV Air, 8KV contact complianceCAN/232/485:IEC61000-4-2 ESD 30KV Air, 30KV contact complianceEFT, IEC 61000-4-4,50A (5/50ns)Lighting, IEC 61000-4-5 2nd edition, 10A</td></tr><tr><td>工作/存储温度</td><td>-40°C-85°C</td></tr><tr><td>振动</td><td>1mm (0-100Hz) &amp;&lt;18g(100Hz-2kHz)</td></tr><tr><td>冲击</td><td>2000g</td></tr><tr><td>MTBF</td><td>50000h</td></tr></table>
如果用232接口进行400hz传输数据，用户的232接口芯片需要支持921600的波特率
# 4. 标定
# 4.1 出厂标定
加速度计和陀螺仪出厂前会经过标定，这些被标定的误差包括比例因子、零偏、非正交误差以及温度。校准参数会被写在模块内部。
# 4.2 启动标定
为了模组获得更好的性能，我们还为用户提供了上电自动标定的功能，这需要用户在上电的过程中保持静止1s，这样可以获得更准确的零偏参数。我们独有的启动算法可以模组很好的避免上电零偏计算错误，充分发挥模块性能。
# 4.3磁力计校准
地磁传感器出厂前经过椭球校准，但磁传感器很容易受到外界环境磁场干扰，一般都需要客户在拿到产品后重新校准。
模块内部自带主动地磁校准系统，该系统不需要用户发送任何指令，该系统在后台自动采集一段时间内地磁场数据，并做分析比较，剔除异常数据，一旦数据足够，就会尝试地磁校准。所以，当使用9轴模式时，不需要用户任何干预即可完成地磁校准。但是模块仍然提供接口来让用户检查当前校准状态。自动校准的前提是需要模块有充分的姿态变化，并且维持一定时间，内部校准系统才能搜集不同姿态下的地磁场信息，从而完成校准，静止状态下是无法进行地磁校准的。
当首次使用模块并且需要使用9轴模式时，应进行如下校准操作：
1.检查周围是否存在磁场干扰：室内桌子旁，大型铁质框架结构附近，都属于常见的干扰区域。建议将模块拿到室外空旷处，即使没有条件拿到室外，尽量将模块远离 $( > 0 . 5 \mathsf { m } )$ )实验室桌子、电脑、电机、手机等容易产生地磁干扰的物体。
2. 在尽量小范围内(位置不动，只是旋转)，缓慢的让模块旋转，让模块经历尽量多的姿态位置(每个轴至少都旋转 $3 6 0 ^ { \circ }$ ，持续约1分钟)。一般情况下即可完成校准。如果始终没能成功校准模块，说明周围地磁场干扰比较大。
3.校准的成功与否可用AT指令来查看：发送 $_ { \mathsf { A T } + \ I { \mathsf { N F } } { 0 } = { \mathsf { H S } } \ I }$ 指令，模块会打印当前地磁校准系统状态，如图4-1所示：这里只需关心fiterr项即可：0.03以下说明校准结果已经足够好。如果fiterr始终 ${ \tt > } 0 . 7$ ，说明地磁干扰很大，需要再次校准以期得到更好的校准结果。拟合残差会随着时间缓慢增长。
4.虽然地磁参数估计可以在线自动采集数据，自动的动态拟合地磁校准参数。但是如果周围地磁环境改变(比如需要到另外房间或者室内室外切换，或者是模块被安装/焊接到了新的环境中)，还需重复执行1-3步。
5.虽然地磁校准无需手动开启或停止，模块自动在后台自动运行该系统，但是用户仍然可以手动控制地磁校准系统的开启与否，使用 AT+MCALCTL=0 来关闭地磁校准系统，使用 $A T + M C A \bot C T L = 1$ 来开启地磁校准系统，该命令立即生效，掉电保存。用户可以开启地磁校准后使 $_ { \mathsf { A T } + \mathtt { T N F O } = \mathsf { H S I } }$ 来判断校准质量，一旦校准成功完成，使用 AT+MCALCTL=0 来关闭校准系统,并锁定校准值。一般情况下，只要在无磁区域校准成功过一次，后面是无需再次校准的。
6.如果客户安装位置改变(比如上一次校准是拿着模块单独去校准的，使用的时候却是安装在目标设备上)。则需要带着目标设备进行重新校准。
# 4.3.1 磁干扰分类
地磁干扰可分为空间磁场干扰与传感器坐标系下的磁场干扰，如下图所示

Distortions that move with the sensor

![image](https://cdn-mineru.openxlab.org.cn/result/2026-03-01/b3f3e820-ccd3-430d-85c3-8f0001674093/caf5d4e77493e148338f9fe6ed0aeb5b79a4dfaa3b115c5d243ccfd1fcb57aec.jpg)

![image](https://cdn-mineru.openxlab.org.cn/result/2026-03-01/b3f3e820-ccd3-430d-85c3-8f0001674093/b2c3041f61cb4ce6e8597b1e8941de3389c6e68dc86147633103e895adb6a001.jpg)

· Calibration errors 
· Hard iron effects 
· Soft iron effects 
·Etc. 

Distortions that do not move with the sensor

![image](https://cdn-mineru.openxlab.org.cn/result/2026-03-01/b3f3e820-ccd3-430d-85c3-8f0001674093/5f8cc4d365ba5326592382fe1e0caf78a8a6515a8cb30dd796cbe63ac6000b24.jpg)

![image](https://cdn-mineru.openxlab.org.cn/result/2026-03-01/b3f3e820-ccd3-430d-85c3-8f0001674093/df1dfa46c7c560fae56e5fca28d2517e4f64904976fe16fc10b37397ac8328af.jpg)

·Spatial distortions 
·Temporal distortions 
·Etc. 
# 4.3.1.1空间磁场干扰(干扰不随传感器位姿改变而改变)
定义：磁场干扰不随传感器运动而运动，处于世界坐标系下
典型干扰源：各种固定的地磁干扰源，家具，家用电器，线缆，房屋内的钢筋结构等。一切不随磁传感器运动而运动的干扰源，下图是典型的室内磁场分布图。
![image](https://cdn-mineru.openxlab.org.cn/result/2026-03-01/b3f3e820-ccd3-430d-85c3-8f0001674093/8b8ee689342711a5d7376ec9de2c2ae0757de7b10cfd1036f7e5c863cbc3300f.jpg)

对模块的影响：无论磁场传感器是否校准的好，这些空间磁场的干扰(或者说环境磁场不均匀)都会使得空间地磁场发生畸变。地磁补偿会错误，无法获得正确的航向角。他们是造成室内地磁融合难以使用的主要原因。这种干扰不能被校准, 会严重影响地磁性能。空间磁场干扰在室内尤其严重。
应对措施：只能尽量避免这种干扰源
# 4.3.1.2 传感器坐标系下的干扰(干扰随传感器位姿改变而改变)
定义：地磁场干扰源随传感器运动而运动
典型干扰源：与模块固定在一起的PCB板子，仪器设备，产品等。他们和磁传感器视为同一个刚体，随磁传感器运动而运动。
对模块的影响：对传感器造成硬磁/软磁干扰。这些干扰可以通过地磁校准算法加以很好的消除。
应对措施：对模块进行地磁校准。
# 4.3.2地磁使用注意事项
在室内环境下，空间磁场干扰尤其严重，而且空间磁干扰并不能通过校准来消除。在室内环境下尽管模块内置均质磁场检测及屏蔽机制，但9轴模式航向角的准确度很大程度上取决于室内磁场畸变程度，如果室内磁场环境很差(如电脑机房旁，电磁实验室，车间，地下车库等)，即使校准后9轴的航向角精度可能还不如6轴模式甚至会出现大角度误差。
模块的自动地磁校准系统只能处理和模块安装在一起的，固定的磁场干扰。安装环境如果有磁场干扰，这种干扰必须是固定的，并且这个干扰磁场与模块安装之后不会再发生距离变化(例：模块安装在一个铁材料之上，因为铁会有磁场干扰，这时就需要把铁与模块一起旋转校准，并且这个铁在使用当中是不会和罗盘再分开的(发生相对位移)，一旦分开是需要再重新校准。如果这个铁大小是不固定的，或与罗盘的距离变化也不是固定的，这种干扰是无法校准的，即使校准成功，也会精度非常差，只能避而远之安装，安全距离控制在 50cm以上)。
# 4.3.3 案例分析
假设客户想在移动机器人上使用9轴模式获得准确的不漂移的航向角，模块安装在机器人上(看做一个刚体)，由于机器人本身的金属结构(部件，电路)会有一个很大的硬磁干扰，相当于上文提到的"传感器坐标系下的干扰"。这部分干扰可以被校准。由于机器人的电机启动停止，以及机器人在室内经过各种房间的磁干扰导致空间磁场改变，会产生上文中提到的"空间磁场干扰"。这部分干扰不能被校准。两种干扰同时存在的可能性很大，给9轴模式造成了很大挑战。这时候建议客户用6轴模式，如果9轴模式必须使用则要做到如下几点：
1. 校准： 必须带着机器人一起校准(机器人足够小)，单独把模块拿下来校准好再安装上去是不正确的做法。必须将机器人和模块视做一个刚体去校准才能得到正确的校准结果，具体校准环节请参考上文，校准成功后，重新上电(复位)生效。
2.由于室内磁环境复杂，即使校准正确完成可能还是会出现较大的航线误差，尤其是电机启停，功率改变时，对磁场影响巨大。
如果应用场合不需要地磁传感器，可以忽略地磁传感器，产品默认处于6轴模式，即地磁场不参与解算，不建议机器人行业使用9轴姿态传感器
5. 关于模式说明
# 5.1型号的模式区别
CH10X每个系列支持的模式如下表：
<table><tr><td>型号</td><td>CH102</td><td>CH102M</td><td>CH104</td><td>CH104M</td><td>CH108M</td></tr><tr><td>加速度计</td><td>●</td><td>●</td><td>●</td><td>●</td><td>●</td></tr><tr><td>陀螺仪</td><td>●</td><td>●</td><td>●</td><td>●</td><td>●</td></tr><tr><td>地磁</td><td>-</td><td>●</td><td>-</td><td>●</td><td>●</td></tr><tr><td>气压</td><td>-</td><td>-</td><td>-</td><td>-</td><td>●</td></tr><tr><td>6轴(VRU)模式</td><td>●</td><td>●</td><td>●</td><td>●</td><td>●</td></tr><tr><td>9轴(AHRS)模式</td><td>-</td><td>●</td><td>-</td><td>●</td><td>●</td></tr></table>
# 5.2 不同模式的区别
<table><tr><td>模式</td><td>使用环境</td><td>典型应用</td><td>优点</td><td>缺点</td></tr><tr><td>6轴</td><td>不用考虑磁场影响</td><td>无人驾驶倾角检测</td><td>姿态角输出稳定性好完全不受磁场干扰</td><td>在没有外界参考条件下航向角随时间缓慢漂移</td></tr><tr><td>9轴</td><td>需要考虑磁场影响</td><td>指南针寻北系统</td><td>航向角不会随时间漂移一旦检测到地磁场可快速修正航向角指北</td><td>任何磁干扰都会使航向角准确度下降。干扰严重的情况下航向角无法指向正确方向。使用前需要校准传感器</td></tr></table>
支持地磁的产品，无论在哪种模式下都会输出地磁信息，6轴模式地磁不会参与解算，9轴模式地磁会参与解算
6. 串口(RS232/USB VCOM)通讯协议
模块上电后，默认按出厂帧率(通常为100Hz)输出帧数据，帧格式如下：
串口数据帧结构：
<帧头(0x5A)><帧类型(0xA5)><长度><CRC校验><数据域>
<table><tr><td>域名称</td><td>值</td><td>长度(字节)</td><td>说明</td></tr><tr><td>帧头</td><td>0x5A</td><td>1</td><td>固定为0x5A</td></tr><tr><td>帧类型</td><td>0xA5</td><td>1</td><td>固定为0xA5</td></tr><tr><td>长度</td><td>1-512</td><td>2</td><td>帧中数据域的长度，低字节在前。长度表示数据域的长度(不包含帧头，帧类型，长度，CRC)</td></tr><tr><td>CRC校验</td><td>-</td><td>2</td><td>除CRC本身外其余所有字段(帧头,帧类型,长度,数据域)的16位CRC校验和。LSB(低字节在前)</td></tr><tr><td>数据域</td><td>-</td><td>1-512</td><td>一帧携带的数据。由若干个子数据包组成。每个数据包包含数据包标签和数据两部分。标签决定了数据的类型及长度。</td></tr></table>
![image](https://cdn-mineru.openxlab.org.cn/result/2026-03-01/b3f3e820-ccd3-430d-85c3-8f0001674093/8d29994f4e4fa57f8246a564833b5f78ab906aa5303c4e10b3ceb35d166dd6d1.jpg)

CRC实现函数：
/\* correctCrc: previous rc value, set 0 if it's first section src: source stream data lengthInBytes: length \*/ static void rc16_update( uint16_t \*correctCrc, const uint8_t \*src, uint32_t lengthInBytes) { uint32_tCRC $=$ \*correctCrc; uint32_t j; for $(\mathrm{j} = 0$ ;j<lengthInBytes；++j） { uint32_t i; uint32_t byte $=$ src[j]; crc ^=byte<<8; for $(i = 0$ ;i<8； $+ + i)$ { uint32_t temp $=$ crc<<1; if (crc&0x8000) { temp $= 0\times 1021$ 1; } crc $=$ temp; } } \*correctCrc $=$ rc; } 
# 6.1串口数据包
<table><tr><td>数据包标签</td><td>数据包长度(包含标签1字节)</td><td>名称</td><td>备注</td></tr><tr><td>0x91</td><td>76</td><td>IMUSOL</td><td>IMU数据集合</td></tr></table>
# 6.2 产品支持数据包列表
# 6.2.1 0X91( IMUSOL)
共76字节。集成了IMU的传感器原始输出和姿态解算数据。
<table><tr><td>字节偏移</td><td>类型</td><td>大小</td><td>单位</td><td>说明</td></tr><tr><td>0</td><td>uint8_t</td><td>1</td><td>-</td><td>数据包标签:0x91</td></tr><tr><td>1</td><td>uint8_t</td><td>1</td><td>-</td><td>ID</td></tr><tr><td>2</td><td>uint8_t</td><td>1</td><td>-</td><td>保留</td></tr><tr><td>3</td><td>int8_t</td><td>1</td><td>°C</td><td>模块平均温度</td></tr><tr><td>4</td><td>float</td><td>4</td><td>Pa</td><td>气压(部分型号支持)</td></tr><tr><td>8</td><td>uint32_t</td><td>4</td><td>ms</td><td>节点本地时间戳信息，从系统开机开始累加，每毫秒增加1</td></tr><tr><td>12</td><td>float</td><td>12</td><td>1G(1G = 1重力加速度)</td><td>经过出厂校准后的加速度,顺序为:XYZ</td></tr><tr><td>24</td><td>float</td><td>12</td><td>deg/s</td><td>经过出厂校准后的角速度,顺序为:XYZ</td></tr><tr><td>36</td><td>float</td><td>12</td><td>uT</td><td>磁强度,顺序为:XYZ</td></tr><tr><td>48</td><td>float</td><td>12</td><td>deg</td><td>节点欧拉角 顺序为:横滚角(Roll,-180°~180°),俯仰角(Pitch,-90°~90°),航向角(Yaw,-180°~180°)</td></tr><tr><td>60</td><td>float</td><td>16</td><td>-</td><td>节点四元数集合,顺序为WXYZ</td></tr></table>
# 6.3 数据帧结构示例
# 6.3.1 数据帧配置为 0x91 数据包
使用串口助手采样一帧数据,共82字节,前6字节为帧头,长度和CRC校验值。剩余76字节为数据域。假设数据接收到C语言数组 buf 中。如下所示:
5A A5 4C 00 6C 51 91 00 A0 3B 01 A8 02 97 BD BB 04 00 9C A0 65 3E A2 26 45 3F 5C E7 30 3F E2 D4 5A C2 E5 9D A0 C1 EB 23 EE C2 78 77 99 41 AB AA D1 C1 AB 2A 0A C2 8D E1 42 42 8F 1D A8 C1 1E 0C 36 C2 E6 E5 5A 3F C1 94 9E 3E B8 C0 9E BE BE DF 8D BE 
第一步：判断帧头，得到数据域长度和帧CRC：
帧头: 5A A5
帧数据域长度: 4C 00 : (0x00<<8) + 0x4C = 76
帧CRC校验值: 6C 51 :(0x51<<8) + 0x6C = 0x516C
第二步：校验CRC
uint16_t payload_len;   
uint16_tCRC; $\mathtt{crc} = 0$ payload_len $\equiv$ buf[2] $^+$ (buf[3]<<8); /\*calulate5A A5 and LEN filedCRC\*/CRC16_update(&crc，buf，4); /\*calulate payloadCRC\*/ CRC16_update(&crc，buf+6，payload_len); 
得到CRC值为0x516C, 与帧中携带CRC值相同，帧CRC校验通过。
第三步：接收数据
从 $\Theta \times 9 1$ 开始为数据包的数据域，定义数据结构体和常用转换宏:
```c
include"stdio.h" #include "string.h" /\*common type conversion \*/ #defineU1(p）\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\* static uint16_t U2(void_t\*p){uint16_t u;memcpy(&u,p,2);return u;} static uint32_t U4(void_t\*p){uint32_t u;memcpy(&u,p,4);return u;} static int32_t I4(void_t\*p){int32_t u;memcpy(&u,p,4);return u;} static float R4(void_t\*p){float r;memcpy(&r,p,4);return r;} typedef struct { uint8_t tag; /\*itemtag:0x91 \*/ uint32_t id; /\*user defineID \*/ float acc[3]; /\* acceleration \*/ float gyr[3]; /\* angular velocity \*/ float mag[3]; /\* magnetic field \*/ float eul[3]; /\* attitude:eular angle \*/ float quat[4]; /\* attitude:quaternion \*/ float pressure; /\* air pressure \*/ 
```
```txt
uint32_t timestamp;   
}imu_data_t; 
```
接收数据， 从buf[6]=0x91开始为payload部分:
```c
imu_data_t i0x91 = {0};  
int offset = 6; /* payload strat at buf[6] */  
i0x91.tag = U1(buf+offset+0);  
i0x91.id = U1(buf+offset+1);  
i0x91.pressure = R4(buf+offset+4);  
i0x91.timestamp = U4(buf+offset+8);  
i0x91.acc[0] = R4(buf+offset+12);  
i0x91.acc[1] = R4(buf+offset+16);  
i0x91.acc[2] = R4(buf+offset+20);  
i0x91.gyr[0] = R4(buf+offset+24);  
i0x91.gyr[1] = R4(buf+offset+28);  
i0x91.gyr[2] = R4(buf+offset+32);  
i0x91 mag[0] = R4(buf+offset+36);  
i0x91 mag[1] = R4(buf+offset+40);  
i0x91 mag[2] = R4(buf+offset+44);  
i0x91.eul[0] = R4(buf+offset+48);  
i0x91.eul[1] = R4(buf+offset+52);  
i0x91.eul[2] = R4(buf+offset+56);  
i0x91.quat[0] = R4(buf+offset+60);  
i0x91.quat[1] = R4(buf+offset+64);  
i0x91.quat[2] = R4(buf+offset+68);  
i0x91.quat[3] = R4(buf+offset+72); 
```
打印接收到的数据:
```javascript
printf("%-16s0x%%\r\n", "tag:", i0x91.tag); printf("%-16s%d\r\n", "id:", i0x91.id); printf("%-16s8.4f %8.4f %8.4f\r\n", "acc(G):", i0x91.acc[0], i0x91.acc[1], i0x91.acc[2]); printf("%-16s8.3f %8.3f %8.3f\r\n", "gyr(deg/s):", i0x91.gyr[0], i0x91.gyr[1], i0x91.gyr[2]); printf("%-16s8.3f %8.3f %8.3f\r\n", "mag(uT):", i0x91 mag[0], i0x91 mag[1], i0x91 mag[2]); printf("%-16s8.3f %8.3f %8.3f\r\n", "eul(deg):", i0x91.eul[0], i0x91.eul[1], i0x91.eul[2]); printf("%-16s8.3f %8.3f %8.3f\r\n", "quat:", i0x91.quat[0], i0x91.quat[1], i0x91.quat[2], i0x91.quat[3]); printf("%-16s8.3f\r\n", "presure(pa):", i0x91.pressure); printf("%-16s%d\r\n", "timestamp(ms):", i0x91.timestamp); 
```
打印出的解析结果:
```txt
tag: 0x91  
id: 0  
acc(G): 0.2242 0.7701 0.6910  
gyr(deg/s): -54.708 -20.077 -119.070  
mag(uT): 19.183 -26.208 -34.542  
eul(deg): 48.720 -21.014 -45.512  
quat: 0.855 0.310 -0.310 -0.277  
presure(pa): -0.000  
timestamp(ms): 310205 
```
# 7. AT指令
当使用串口与模块通讯时, 模块支持AT 指令集配置/查看模块参数。AT 指令总以ASCII 码 AT 开头，后面跟控制字符，最后以回车换行 \r\n 结束。
使用上位机输入AT指令：
![image](https://cdn-mineru.openxlab.org.cn/result/2026-03-01/b3f3e820-ccd3-430d-85c3-8f0001674093/3edea2932a5e371bf27abad0fc3bae20f517bc79094e739e577ff8bae62ff87e.jpg)

使用串口调试助手进行测试：
SSCOMV5.13.1串口/网络数据调试器，作者：大虾丁丁,2618058@qq.com.QQ群：52502449(最新版本)
通讯端口串口设置显示发送多字符串小工具帮助联系作者
![image](https://cdn-mineru.openxlab.org.cn/result/2026-03-01/b3f3e820-ccd3-430d-85c3-8f0001674093/dba1e6f1d71659883dc2584ea7b43730052bde095c6a163007142403f52dc71a.jpg)

# 7.1 AT指令列表
<table><tr><td>指令</td><td>功能</td><td>掉电保存(Y)/掉电不保存(N)</td><td>立即生效(Y)/复位生效(N)</td><td>备注</td></tr><tr><td>AT+ID</td><td>设置模块用户ID</td><td>Y</td><td>N</td><td></td></tr><tr><td>AT+INFO</td><td>打印模块信息</td><td>N</td><td>Y</td><td></td></tr><tr><td>AT+ODR</td><td>设置模块串口输出帧频率</td><td>Y</td><td>N</td><td></td></tr><tr><td>AT+BAUD</td><td>设置串口波特率</td><td>Y</td><td>N</td><td></td></tr><tr><td>AT+EOUT</td><td>数据输出开关</td><td>N</td><td>Y</td><td></td></tr><tr><td>AT+RST</td><td>复位模块</td><td>N</td><td>Y</td><td></td></tr><tr><td>AT+URFR</td><td>安装角度设置</td><td>Y</td><td>N</td><td></td></tr><tr><td>AT+SETYAW</td><td>手动设置航向角</td><td>N</td><td>Y</td><td></td></tr><tr><td>AT+RSTORT</td><td>安装对齐设置</td><td>Y</td><td>Y</td><td></td></tr><tr><td>AT+MODE</td><td>设置模块工作模式</td><td>Y</td><td>N</td><td>部分型号支持</td></tr></table>
# 7.2 指令详解
# 7.2.1 AT+ID
设置模块用户ID
例 $A T + \tt I D = 1$ 
# 7.2.2 AT+INFO
打印模块信息，包括产品型号，版本，固件发布日期等。
# 7.2.3 AT+ODR
设置模块串口输出速率。掉电保存，复位模块生效
例 设置串口输出速率为100Hz: AT+ODR=100
当ODR设置为比较高时(如200),默认的115200波特率可能不满足输出带宽要求，此时需要将模块波特率设高(如921600)后，模块才能按设置的ODR输出数据帧。输出帧率可以为1,2,5,10,20,50,100,200,400Hz。
# 7.2.4 AT+BAUD
设置串口波特率
例 AT+BAUD=115200
使用此指令需要特别注意，输入错误波特率后会导致无法和模块通讯。
波特率参数设置好后掉电保存，复位模块生效。上位机的波特率也要做相应修改。
# 7.2.5 AT+EOUT
串口输出开关
例 打开串口输出 AT+EOUT=1 关闭串口输出 AT+EOUT=0
# 7.2.6 AT+RST
复位模块
例 AT+RST
# 7.2.7 AT+SETYAW
设置航向角，格式为 AT+SETYAW=<MODE>,<VAL>
MODE $_ { = 0 }$ 绝对模式：将航向角直接设置为VAL的值。如 AT+SETYAW=0,90 将航向角直接设置为90°
MODE $^ { = 7 }$ 相对模式：将原航向角递增VAL值。如 AT+SETYAW $^ { = 1 }$ ,-10.5 将航向角递增-10.5°，如原来为$3 0 ^ { \circ }$ ，发送命令后航向角变为19.5°。
# 7.2.8 AT+MODE
设置模块工作模式
# 例
设置模块工作在6轴模式(无磁校准) AT+MODE=0
设置模块工作在9轴模式(地磁场传感器参与航向角校正) AT+MODE=1
该指令仅支持CH010M、CH020M、CH040MP，不支持CH010、CH020、CH040
# 7.2.9 AT+URFR
这条指令提供了旋转传感器XYZ轴的接口，可用于任意角度的垂直安装。
```javascript
AT+URFR=C00,C01,C02,C10,C11,C12,C20,C21,C22 
```
其中 $C _ { n n }$ 支持浮点数
$$
\left\{ \begin{array}{l} X \\ Y \\ Z \end{array} \right\} _ {U} = \left[ \begin{array}{c c c} C 0 0 & C 0 1 & C 0 2 \\ C 1 0 & C 1 1 & C 1 2 \\ C 2 0 & C 2 1 & C 2 2 \end{array} \right] \cdot \left\{ \begin{array}{l} X \\ Y \\ Z \end{array} \right\} _ {B}
$$
其中 为旋转后的传感器坐标系下传感器数据， 为旋转前传感器坐标系下传感器数据U B
下面是几种常用旋转举例：
新传感器坐标系为 绕原坐标系X轴 旋转 $\boldsymbol { \mathfrak { - 9 0 ^ { \circ } } }$ (适用于垂直安装:Y轴正方向朝下)， 输入命令：
```csv
AT+URFR=1,0,0,0,0,1,0,-1,0 
```
新传感器坐标系为 绕原坐标系X轴 旋转 $9 0 ^ { \circ }$ (适用于垂直安装:Y轴正方向朝上)， 输入命令：
```csv
AT+URFR=1,0,0,0,0,-1,0,1,0 
```
新传感器坐标系为 绕原坐标系X轴 旋转1 $8 0 ^ { \circ }$ ， 输入命令： AT+URFR=1,0,0,0,-1,0,0,0,-1
新传感器坐标系为 绕原坐标系Y轴 旋转 $9 0 ^ { \circ }$ (适用于垂直安装:X轴正方向朝上)， 输入命令：
```csv
AT+URFR=0,0,-1,0,1,0,1,0,0 
```
新传感器坐标系为绕原坐标系Y轴旋转-90°(适用于垂直安装:X轴正方向朝下)，输入命令：
```csv
AT+URFR=0,0,1,0,1,0,-1,0,0 
```
新传感器坐标系为 绕原坐标系Y轴 旋转1 $8 0 ^ { \circ }$ ， 输入命令： AT+URFR=-1,0,0,0,1,0,0,0,-1
新传感器坐标系为 绕原坐标系Z轴 旋转 $9 0 ^ { \circ }$ ， 输入命令： AT+URFR=0,-1,0,1,0,0,0,0,1
新传感器坐标系为 绕原坐标系Z轴 旋转-90°， 输入命令： AT+URFR=0,1,0,-1,0,0,0,0,1
恢复出厂默认值： AT+URFR=1,0,0,0,1,0,0,0,1
# 7.2.10 AT+RSTORT
一些情况下，模块安装时需要设置水平对齐(Pitch=Roll=0) 或者全部对齐(Pitch=Roll=Yaw=0)，这是需要给模块的一个初始姿态偏移。使用此命令可以完成对齐配置。
. AT+RSTORT ${ \bf \boldsymbol { \mathbf { \mathit { \sigma } } } } = \boldsymbol { \Theta }$ : Object Reset: Pitch $=$ Roll= Yaw = 0 
AT+RSTORT $= 1$ : Heading Reset: Pitch $= \mathsf { R o } | | = 0$ $=$ , Yaw保持不变
AT+RSTORT $^ { \cdot = 2 }$ : Alignment Reset: Pitch, Roll 保持不变, Yaw $_ { = 0 }$ 
AT+RSTORT $= 3$ : Reset Offset: 清除所有对齐偏移设置
8. CAN通讯协议
CAN通讯协议可支持 CANopen协议和SAE J1939协议。模块默认支持CANopen协议，如需J1939协议版本，请联系技术支持。
# 8.1 CANopen协议
本产品CAN接口遵循以下标准：
CAN接口符合CANopen协议，所有通讯均使用标准数据帧，只使用PTO1-4 传输数据。不接收/发送远程帧和拓展数据帧
所有PTO采用异步定时触发模式。
# 8.1.1 CANopen 默认设置
<table><tr><td>CANopen默认配置</td><td>值</td></tr><tr><td>CAN 波特率</td><td>500KHz</td></tr><tr><td>CANopen节点ID</td><td>8</td></tr><tr><td>初始化状态</td><td>Operational</td></tr><tr><td>心跳包</td><td>无</td></tr><tr><td>TPDO输出速率</td><td>10Hz - 200Hz(每个TPDO)</td></tr></table>
# 8.1.2 CANopen TPDO
<table><tr><td>PTO通道</td><td>PTO 帧ID</td><td>长度(DLC)</td><td>PTO 传输方式</td><td>异步输出频率(Hz)</td><td>发送数据</td><td>说明</td></tr><tr><td>TPD01</td><td>0x180+ID</td><td>6</td><td>异步定时(0xFE)</td><td>100</td><td>加速度</td><td>类型:int16,低字节在前,每个轴2字节,共6字节。分别为X,Y,Z轴加速度,单位为mG(0.001重力加速度)</td></tr><tr><td>TPD02</td><td>0x280+ID</td><td>6</td><td>异步定时(0xFE)</td><td>100</td><td>角速度</td><td>类型:int16,低字节在前,每个轴2字节,共6字节。分别为X,Y,Z轴角速度,单位为0.1DPS(°/s)</td></tr><tr><td>TPD03</td><td>0x380+ID</td><td>6</td><td>异步定时(0xFE)</td><td>100</td><td>欧拉角</td><td>类型:int16,低字节在前,每个轴2字节,共6字节。顺序分别为横滚角:Roll,俯仰角:Pitch,航向角:Yaw。单位为0.01°</td></tr><tr><td>TPD04</td><td>0x480+ID</td><td>8</td><td>异步定时(0xFE)</td><td>100</td><td>四元数</td><td>类型:int16,低字节在前,每个元素2字节,共8字节。分别为qw qx qy qz。单位四元数扩大10000倍后结果。如四元数为1,0,0,0时,输出10000,0,0,0.</td></tr></table>
![image](https://cdn-mineru.openxlab.org.cn/result/2026-03-01/b3f3e820-ccd3-430d-85c3-8f0001674093/dbc8d3394c719aa870b42bdaf30de4936041d8b246d01fb1ef493af09fde8f45.jpg)

例:
收到加速度CAN帧： $I { \mathsf { D } } { = } \Theta { \times } 1 8 8$ ，DATA $\underline { { \underline { { \mathbf { \delta \pi } } } } }$ 4A 00 1F 00 C8 03
$1 0 = 0 \times 7$ 88: ID为8的设备发送的加速度数据帧
加速度X轴 $= 0 { \times } 0 0 4 { \mathsf { A } } = 7 4 = 7 4 { \mathsf { m G } }$ 
加速度Y轴 $= 0 { \times } 0 0 7 5 = 7 3 7 = 3 7 5 0 5$ $=$ 
加速度Z轴 $= 0 { \times } 0 3 0 8 = 9 6 8 = 9 6 8 m G$ $=$ 
收到角速度CAN帧： $I { \mathsf { D } } { = } { \boldsymbol { \Theta } } { \times } 2 8 8$ ，DATA $\underline { { \underline { { \mathbf { \delta \pi } } } } }$ 15 00 14 01 34 00
· $V { = } 0 { \times } 2 8 8$ : ID为8的设备发送的角速度数据帧
角速度X轴 $= 0 { \times } 0 0 7 5 = 2 7 = 2 . 7$ dps
角速度Y轴 = 0x0114 = 276 = 27.6dps
角速度Z轴 $\mathbf { \tau } = 0 { \times } 0 0 3 4 = 5 2 \mathbf { = } 5 . 2$ dps
# 8.2 使用上位机连接CAN设备
使用PCAN-View工具，配合PCAN-USB，可以在接收框(Rx Message)中会显示收到的CAN消息及帧率，如下图所示:
![image](https://cdn-mineru.openxlab.org.cn/result/2026-03-01/b3f3e820-ccd3-430d-85c3-8f0001674093/cf461226c3acf65da39627367d64889960c9b47af364f3d2b4447facb9a1a1de.jpg)

# 8.2.3 CANopen接口常用命令
# 8.2.3.1 使能数据输出(开启异步触发)
发送标准CANopen协议帧，使用NMT: Start Remote Node命令:
```txt
ID=0x000，DATA=0x08 
```
其中 0x01为Start Remote Node指令， 0x08为节点ID
数据字典以下位置存放厂商参数配置数据,可通过CANopen发送快速SDO指令修改，掉电保存，重新上电生效。
<table><tr><td>数据字典位置</td><td>子偏移</td><td>名称</td><td>值类型</td><td>默认值</td><td>说明</td></tr><tr><td>0x2100</td><td>0</td><td>CAN_BAUD</td><td>INTEGER32</td><td>500000</td><td>CAN总线波特率</td></tr><tr><td>0x2101</td><td>0</td><td>NODE_ID</td><td>INTEGER32</td><td>8</td><td>CANOPEN节点ID</td></tr><tr><td>0x2104</td><td>0</td><td>CAN_TR</td><td>INTEGER32</td><td>0</td><td>1:接通120ohm终端电阻, 0:断开120ohm终端电阻</td></tr></table>
以上配置操作均使用快速SDO来写数据字典, 其中TPDO通道与其对应的参数索引为：
<table><tr><td>PTO通道</td><td>PTO 帧ID</td><td>TPDO参数索引地址(CANopen协议默认定义)</td><td>说明</td></tr><tr><td>TPD01</td><td>0x180+ID</td><td>0x1800</td><td>加速度</td></tr><tr><td>TPD02</td><td>0x280+ID</td><td>0x1801</td><td>角速度</td></tr><tr><td>TPD03</td><td>0x380+ID</td><td>0x1802</td><td>欧拉角</td></tr><tr><td>TPD04</td><td>0x480+ID</td><td>0x1803</td><td>四元数</td></tr><tr><td>TPD05</td><td>0x680+ID</td><td>0x1804</td><td>气压</td></tr></table>
# 8.2.3.2 修改CAN波特率
修改波特率:( $1 0 { = } 0 \times 6 0 8$ , 长度为8的标准数据帧, 重新上电生效)
CAN波特率修改为125K: $I { \mathsf { D } } { = } { \boldsymbol { \Theta } } { \times } 6 { \boldsymbol { \Theta } } { 8 }$ ，DATA=23,00,21,00,48,E8,01,00
CAN波特率修改为250K: $I { \mathsf { D } } { = } { \boldsymbol { \Theta } } { \times } 6 { \boldsymbol { \Theta } } { 8 }$ ，DATA=23,00,21,00,90,D0,03,00
CAN波特率修改为500K: $I { \mathsf { D } } { = } { \boldsymbol { \Theta } } { \times } 6 { \boldsymbol { \Theta } } { 8 }$ ，DATA=23,00,21,00,20,A1,07,00
CAN波特率修改为1M: $I { \mathsf { D } } { = } { \boldsymbol { \Theta } } { \times } 6 { \boldsymbol { \Theta } } { 8 }$ ，DATA $^ { = 2 3 }$ ,00,21,00,40,42,0F,00
解释：设置波特率为125KHz： $V { = } 0 { \times } 6 0 8$ 为快速写SDO地址,其中8为默认节点ID，修改节点ID后要做相应修改,如CANopen ID改为9后, $1 0 { = } 0 \times 6 0 9$ ， ${ 0 } \times { 2 } 3$ 为SDO写四个字节指令， 0x00, 0x21为写0x2100索引，0x00 子索引位置，默认0，(4-7字节)0x00, 0x01, 0x $\Xi 8 , 0 \times 4 8 = ( 0 \times 0 0 < < 2 4 ) + ( 0 \times 0 1 < < 1 6 ) + ( 0 \times \mathsf { E } 8 < < 8 ) + 0 \times 4 8 = 1 2 5 0 0 0 _ { \circ }$ 
# 8.2.3.3 修改节点ID
如将设备CANopen节点ID改为9, 重新上电生效
```txt
ID=0x608，DATA=23,01,21,00,09,00,00,00 
```
解释： ${ 0 } { \times } { 2 } { 3 }$ 为SDO写四个字节指令 ， 0x01, 0x21为写0x2101索引， 0x09 0x00, 0x00, 0x00 = (0x00<<24) +$( 0 \times 0 0 < < 1 6 ) + ( 0 \times 0 0 < < 8 ) + 0 \times 0 9 = 9 0$ 。
ID修改范围：1-64， 生效后发送启动节点命令(比如节点启动命令数据变为01 09)和SDO指令(发送CAN帧ID变为 $0 \times 6 0 9$ )时注意为新的地址
# 8.2.3.4 切换CAN协议
$I { \mathsf { D } } { = } { \boldsymbol { \Theta } } { \times } 6 { \boldsymbol { \Theta } } { 8 }$ ，DATA=23,06,21,00,00,00,00,00 
切换协议为CANopen协议，重新后生效
. $I { \mathsf { D } } { = } { \boldsymbol { \Theta } } { \times } 6 { \boldsymbol { \Theta } } { 8 }$ ，DATA=23,06,21,00,01,00,00,00 
切换协议为J1939协议，重新后生效
# 8.2.3.5修改/关闭/开启数据输出速率
# 此项配置立即生效,掉电保存
$I { \mathsf { D } } { = } { \boldsymbol { \Theta } } { \times } 6 { \boldsymbol { \Theta } } { 8 }$ ，DATA=2B,00,18,05,00,00,00,00 关闭加速度输出
$I { \mathsf { D } } { = } { \boldsymbol { \Theta } } { \times } 6 { \boldsymbol { \Theta } } { 8 }$ ，DATA=2B,00,18,05,05,00,00,00 加速度200Hz输出
$I { \mathsf { D } } { = } { \boldsymbol { \Theta } } { \times } 6 { \boldsymbol { \Theta } } { 8 }$ ，DATA=2B,00,18,05,0A,00,00,00 加速度100Hz输出
$I { \mathsf { D } } { = } { \boldsymbol { \Theta } } { \times } 6 { \boldsymbol { \Theta } } { 8 }$ ，DATA=2B,00,18,05,14,00,00,00 加速度50Hz输出
$I { \mathsf { D } } { = } { \boldsymbol { \Theta } } { \times } 6 { \boldsymbol { \Theta } } { 8 }$ ，DATA=2B,00,18,05,32,00,00,00 加速度20Hz输出
. $I { \mathsf { D } } { = } { \boldsymbol { \Theta } } { \times } 6 { \boldsymbol { \Theta } } { 8 }$ ，DATA=2B,00,18,05,64,00,00,00 加速度10Hz输出(最低10Hz)
$I { \mathsf { D } } { = } { \boldsymbol { \Theta } } { \times } 6 { \boldsymbol { \Theta } } { 8 }$ ，DATA=2B,01,18,05,00,00,00,00 关闭角速度输出
ID=0x608，DATA=2B,01,18,05,05,00,00,00 角速度200Hz输出
$I { \mathsf { D } } { = } { \boldsymbol { \Theta } } { \times } 6 { \boldsymbol { \Theta } } { 8 }$ ，DATA=2B,01,18,05,0A,00,00,00 角速度100Hz输出
$I { \mathsf { D } } { = } { \boldsymbol { \Theta } } { \times } 6 { \boldsymbol { \Theta } } { 8 }$ ，DATA=2B,01,18,05,14,00,00,00 角速度50Hz输出
ID=0x608，DATA=2B,01,18,05,32,00,00,00 角速度20Hz输出
$I { \mathsf { D } } { = } { \boldsymbol { \Theta } } { \times } 6 { \boldsymbol { \Theta } } { 8 }$ ，DATA=2B,01,18,05,64,00,00,00 角速度10Hz输出(最低10Hz)
$I { \mathsf { D } } { = } { \boldsymbol { \Theta } } { \times } 6 { \boldsymbol { \Theta } } { 8 }$ ，DATA=2B,02,18,05,00,00,00,00 关闭欧拉角输出
ID=0x608，DATA=2B,02,18,05,05,00,00,00 欧拉角200Hz输出
$I { \mathsf { D } } { = } { \boldsymbol { \Theta } } { \times } 6 { \boldsymbol { \Theta } } { 8 }$ ，DATA=2B,02,18,05,0A,00,00,00 欧拉角100Hz输出
ID=0x608，DATA=2B,02,18,05,14,00,00,00 欧拉角50Hz输出
$I { \mathsf { D } } { = } { \boldsymbol { \Theta } } { \times } 6 { \boldsymbol { \Theta } } { 8 }$ ，DATA=2B,02,18,05,32,00,00,00 欧拉角20Hz输出
$I { \mathsf { D } } { = } { \boldsymbol { \Theta } } { \times } 6 { \boldsymbol { \Theta } } { 8 }$ ，DATA=2B,02,18,05,64,00,00,00 欧拉角10Hz输出(最低10Hz)
$I { \mathsf { D } } { = } { \boldsymbol { \Theta } } { \times } 6 { \boldsymbol { \Theta } } { 8 }$ ，DATA=2B,03,18,05,00,00,00,00 关闭四元数输出
ID=0x608，DATA=2B,03,18,05,05,00,00,00 四元数200Hz输出
$I { \mathsf { D } } { = } { \boldsymbol { \Theta } } { \times } 6 { \boldsymbol { \Theta } } { 8 }$ ，DATA=2B,03,18,05,0A,00,00,00 四元数100Hz输出
ID=0x608，DATA=2B,03,18,05,14,00,00,00 四元数50Hz输出
$I { \mathsf { D } } { = } { \boldsymbol { \Theta } } { \times } 6 { \boldsymbol { \Theta } } { 8 }$ ，DATA=2B,03,18,05,32,00,00,00 四元数20Hz输出
$I { \mathsf { D } } { = } { \boldsymbol { \Theta } } { \times } 6 { \boldsymbol { \Theta } } { 8 }$ ，DATA=2B,03,18,05,64,00,00,00 四元数10Hz输出(最低10Hz)
$I { \mathsf { D } } { = } { \boldsymbol { \Theta } } { \times } 6 { \boldsymbol { \Theta } } { 8 }$ ，DATA=2B,04,18,05,00,00,00,00 关闭气压输出
$I { \mathsf { D } } { = } { \boldsymbol { \Theta } } { \times } 6 { \boldsymbol { \Theta } } { 8 }$ ，DATA=2B,04,18,05,05,00,00,00 气压200Hz输出
$I { \mathsf { D } } { = } { \boldsymbol { \Theta } } { \times } 6 { \boldsymbol { \Theta } } { 8 }$ ，DATA=2B,04,18,05,0A,00,00,00 气压100Hz输出
$I { \mathsf { D } } { = } { \boldsymbol { \Theta } } { \times } 6 { \boldsymbol { \Theta } } { 8 }$ ，DATA=2B,04,18,05,14,00,00,00 气压50Hz输出
$I { \mathsf { D } } { = } { \boldsymbol { \Theta } } { \times } 6 { \boldsymbol { \Theta } } { 8 }$ ，DATA=2B,04,18,05,32,00,00,00 气压20Hz输出
$I { \mathsf { D } } { = } { \boldsymbol { \Theta } } { \times } 6 { \boldsymbol { \Theta } } { 8 }$ ，DATA=2B,04,18,05,64,00,00,00 气压10Hz输出(最低10Hz)
解释：TPDO0(加速度)输出速率为100Hz(每10ms输出一次)：0x2B为SDO写两个字节指令。 0x00, 0x18为写0x1800索引。0x05为子索引。 $0 \times 0 0 , 0 \times 0 . 4 = ( 0 \times 0 0 < < 8 ) + 0 \times 0 . 4 = 7 0$ (单位为ms)，后面不足补0.
# 8.2.3.6 开启/关闭站点
使用 NMT命令StartRemoteNode和 StopRemoteNode来开启关闭节点:
开启节点: ID:0，DATA:01 08 其中01为开启节点命令，08为节点ID(出厂默认为8)
关闭节点: ID:0，DATA:02 08 其中02为关闭节点命令，08为节点ID(出厂默认为8)
# 8.2.3.7 配置TPDO为同步模式
先关闭所有TPDO(设置TPDO输出速率为0)， 然后发送CANopen同步帧即可：
CANopen 同步帧: ID:80，DATA:空
# 9. 包装
# 9.1 包装盒
CH10X采用15mmX15mmX6mm飞机盒包装，内部为EVA衬底保护，如下图所示
![image](https://cdn-mineru.openxlab.org.cn/result/2026-03-01/b3f3e820-ccd3-430d-85c3-8f0001674093/f83560bc60b3848fc2a7a809bb493aef64e8847e9362ddd2aea69d6c81b628c5.jpg)

# 9.2外箱标签
外箱标签尺寸60X80mm，内容如下：
<table><tr><td>信息</td><td>描述</td></tr><tr><td>供应商</td><td>模组的生产厂家</td></tr><tr><td>订单号码</td><td>订单编号</td></tr><tr><td>型号</td><td>模块的型号</td></tr><tr><td>品名/规格</td><td>模块的名称</td></tr><tr><td>数量</td><td>卷盘包装的模块数量</td></tr><tr><td>生产日期</td><td>出厂日期</td></tr><tr><td>盒数</td><td>盒数</td></tr><tr><td>品管</td><td>品质负责人确认</td></tr></table>
# 9.3 产品标签
产品标签尺寸为10X20mm，分为产品信息标签与合格证标签，内容如下：
Model: CH108M USB 
9 Axis Attitude Sensor 
Interface: USB 
VER:A0 223101 
www.hipnuc.com 
北京超核电子科技有限公司
# 合格证
型号: CH108M USB
检验员：001
检验日期：20220914
10. 附录A - 固件升级与恢复出厂设置
本产品支持升级固件。
固件升级步骤:
连接模块，打开上位机，将模块和上位机波特率都设置为115200.打开固件升级窗口
点击连接按钮，如出现模块连接信息。则说明升级系统准备就绪，点击文件选择器(…)选择拓展名为.hex 的固件，然后点击开始编程。下载完成后会提示编程完成，此时关闭串口，重新给模块上电，模块升级完成。
# CH 固件升级
# Open File
SIZE: 87948 
SP :0X200025C0 
PC:0X08008181 
ADDR:0X08008000 
Connect OK 
PKT SIZE:256 
![image](https://cdn-mineru.openxlab.org.cn/result/2026-03-01/b3f3e820-ccd3-430d-85c3-8f0001674093/667ea4255b846494b78de2beec9b88176e9e62c3361c2da3baabb3494a09cba0.jpg)

7% 
11. 附录B-技术支持
新产品信息及技术支持，请关注超核电子公众号和官方网站
![image](https://cdn-mineru.openxlab.org.cn/result/2026-03-01/b3f3e820-ccd3-430d-85c3-8f0001674093/fedb7014117898bc8c2be48096b98333b25a8829f8c39b8e8e6bfa6a2188b15f.jpg)

![image](https://cdn-mineru.openxlab.org.cn/result/2026-03-01/b3f3e820-ccd3-430d-85c3-8f0001674093/f835379b3b91016e22dbda9400be87469edf6a9ef4f33dbc6354f838a59226b3.jpg)
