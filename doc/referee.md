V1.2.0 
A 
机甲大师 ROBOMAST 
第二十五届全国大学生机器人大赛 
ROBOMAST=R 2026 
机甲大师高校系列赛 
通信协议 
ROBOMASTER 

修改日志

<table><tr><td>日期</td><td>版本</td><td>修订记录</td></tr><tr><td>2026.02.09</td><td>V1.2.0</td><td>1.修订命令码 0x0105,0x0204,0x0209,0x020C,0x020D,0x0309,0x03102.删除命令码 0x03043.修订自定义客户端协议中原 RemoteControl(现拆分为KeyboardMouseControl与 CustomControl),TechCoreMotionStateSync, RobotModuleStatus,RadarInfoToClient, DartCommand, GlobalUnitStatus,RobotModuleStatus, AirSupportStatusSync,DartSelectTargetStatusSync, RobotPerformanceSelectionCommand4.自定义客户端新增 CommonCommand 指令5.修复了多处描述或命名错误</td></tr><tr><td>2025.12.18</td><td>V1.1.0</td><td>补充雷达无线链路相关说明</td></tr><tr><td>2025.11.27</td><td>V1.0.0</td><td>首次发布</td></tr></table>
# 前言
本通信协议在 RMUC 与 RMUL 两项赛事中的适用范围如下： 
对于在 RMUC 与 RMUL 之间共用的兵种、机制或场地道具等条目，本协议内容对两项赛事均适用； 
对于仅适用于某一赛项的兵种、机制或场地道具等条目，默认不适用于另一赛项。 
2 
$\circledcirc$ 2026 大疆 版权所有 
ROBOMASTER 
# 目录
修改日志... 
前言 .... 
1. 串口协议... 
1.1 串口协议格式. 4 
1.2 命令码ID和常规链路数据说明. . 6 
1.3 小地图交互数据. .. 35 
1.4 图传链路数据说明. . 40 
1.5 非链路数据说明 . 41 
1.6 雷达无线链路数据说明. . 42 
2. 自定义客户端协议.. . 47 
2.1 指令概览.. . 47 
2.2 详细协议定义. . 50 
附录一：CRC 校验代码示例.. . 78 
附录二：ID 编号说明.. . 84 
附录三：自定义客户端示例通信代码. . 86 
© 2026 大疆 版权所有 
3 
ROBOMASTER 
# 1. 串口协议
# 1.1 串口协议格式
通信方式为串口，配置为：常规链路的波特率为115200，图传链路的波特率为921600，8位数据位，1位 停止位，无硬件流控，无校验位。 

表 1-1 通信协议格式

<table><tr><td>frame_header</td><td>cmd_id</td><td>data</td><td>frameTAIL</td></tr><tr><td>5-byte</td><td>2-byte</td><td>n-byte</td><td>2-byte, CRC16, 整包校验</td></tr></table>

表 1-2 frame_header 格式

<table><tr><td>SOF</td><td>data_length</td><td>seq</td><td>CRC8</td></tr><tr><td>1-byte</td><td>2-byte</td><td>1-byte</td><td>1-byte</td></tr></table>

表 1-3 帧头详细定义

<table><tr><td>域</td><td>偏移位置</td><td>大小(字节)</td><td>详细描述</td></tr><tr><td>SOF</td><td>0</td><td>1</td><td>数据帧起始字节,固定值为0xA5</td></tr><tr><td>data_length</td><td>1</td><td>2</td><td>数据帧中 data 的长度</td></tr><tr><td>seq</td><td>3</td><td>1</td><td>包序号</td></tr><tr><td>CRC8</td><td>4</td><td>1</td><td>帧头 CRC8 校验</td></tr></table>

裁判系统串口数据链路有三种：常规链路、图传链路、雷达无线链路。 

4 
$\circledcirc$ 2026 大疆 版权所有 
ROBOMASTER 
常规链路由裁判系统服务器和主控模块进行数据转发，从电源管理模块的 User 串口收发数据，示意 图如下： 
![image](https://cdn-mineru.openxlab.org.cn/result/2026-02-28/a72e60af-c3aa-4d9e-8e93-7eb7c3ff3a17/c080e45728a8731e5b35377c035c5ddc4ac0669d47d26a7488e0f55bd4b719c5.jpg)

图传链路由裁判系统选手端和图传模块进行数据转发，从图传模块（发送端）的串口接收数据，示意 图如下： 
![image](https://cdn-mineru.openxlab.org.cn/result/2026-02-28/a72e60af-c3aa-4d9e-8e93-7eb7c3ff3a17/a7c163497245ab75539fbc5a8dd1596311f7ca6de840b41f579d5cb6bf2df76e.jpg)

 雷达无线链路由裁判系统信号发射源进行数据发送，从雷达接收电磁波并解析信息。 
![image](https://cdn-mineru.openxlab.org.cn/result/2026-02-28/a72e60af-c3aa-4d9e-8e93-7eb7c3ff3a17/76e3d5c3da88ba8d178caf8933685744d5eb56ecddac426f216389aefc9f4abe.jpg)

正常工作状态下，裁判系统数据延迟约为130ms，丢包率小于 $1 \%$ ； 
在赛场网络环境较恶劣时，裁判系统数据延迟约为 $2 0 0 \mathrm { m s }$ ，丢包率约为 $3 \%$ ； 
测量数据可能存在误差，数据仅供参考。 
© 2026 大疆 版权所有 
5 
ROBOMASTER 
# 1.2 命令码 ID 和常规链路数据说明

表 1-4 命令码 ID 一览

<table><tr><td>命令码</td><td>数据段长度</td><td>说明</td><td>发送方/接收方</td><td>所属数据链路</td></tr><tr><td>0x0001</td><td>11</td><td>比赛状态数据,固定以1Hz频率发送</td><td>服务器→全体机器人</td><td>常规链路</td></tr><tr><td>0x0002</td><td>1</td><td>比赛结果数据,比赛结束触发发送</td><td>服务器→全体机器人</td><td>常规链路</td></tr><tr><td>0x0003</td><td>16</td><td>机器人血量数据,固定以3Hz频率发送</td><td>服务器→全体机器人</td><td>常规链路</td></tr><tr><td>0x0101</td><td>4</td><td>场地事件数据,固定以1Hz频率发送</td><td>服务器→己方全体机器人</td><td>常规链路</td></tr><tr><td>0x0104</td><td>3</td><td>裁判警告数据,己方判罚/判负时触发发送,其余时间以1Hz频率发送</td><td>服务器→被判罚方全体机器人</td><td>常规链路</td></tr><tr><td>0x0105</td><td>3</td><td>飞镖发射相关数据,固定以1Hz频率发送</td><td>服务器→己方全体机器人</td><td>常规链路</td></tr><tr><td>0x0201</td><td>13</td><td>机器人性能体系数据,固定以10Hz频率发送</td><td>主控模块→对应机器人</td><td>常规链路</td></tr><tr><td>0x0202</td><td>14</td><td>实时底盘缓冲能量和射击热量数据,固定以10Hz频率发送</td><td>主控模块→对应机器人</td><td>常规链路</td></tr><tr><td>0x0203</td><td>16</td><td>机器人位置数据,固定以1Hz频率发送</td><td>主控模块→对应机器人</td><td>常规链路</td></tr><tr><td>0x0204</td><td>8</td><td>机器人增益和底盘能量数据,固定以3Hz频率发送</td><td>服务器→对应机器人</td><td>常规链路</td></tr><tr><td>0x0206</td><td>1</td><td>伤害状态数据,伤害发生后发送</td><td>主控模块→对应机器人</td><td>常规链路</td></tr><tr><td>0x0207</td><td>7</td><td>实时射击数据,弹丸发射后发送</td><td>主控模块→对应机器人</td><td>常规链路</td></tr><tr><td>0x0208</td><td>6</td><td>允许发弹量,固定以10Hz频率发送</td><td>服务器→己方英雄、步兵、哨兵、空中机器人</td><td>常规链路</td></tr><tr><td>0x0209</td><td>5</td><td>机器人RFID模块状态,固定以3Hz频率发送</td><td>服务器→己方装有RFID模块的机器人</td><td>常规链路</td></tr><tr><td>0x020A</td><td>6</td><td>飞镖选手端指令数据,固定以3Hz频率发送</td><td>服务器→己方飞镖机器人</td><td>常规链路</td></tr><tr><td>0x020B</td><td>40</td><td>地面机器人位置数据,固定以1Hz频率发送</td><td>服务器→己方哨兵机器人</td><td>常规链路</td></tr><tr><td>0x020C</td><td>2</td><td>雷达标记进度数据,固定以1Hz频率发送</td><td>服务器→己方雷达机器人</td><td>常规链路</td></tr><tr><td>0x020D</td><td>6</td><td>哨兵自主决策信息同步,固定以1Hz频率发送</td><td>服务器→己方哨兵机器人</td><td>常规链路</td></tr><tr><td>0x020E</td><td>1</td><td>雷达自主决策信息同步,固定以1Hz频率发送</td><td>服务器→己方雷达机器人</td><td>常规链路</td></tr><tr><td>0x0301</td><td>118</td><td>机器人交互数据,发送方触发发送,频率上限为30Hz</td><td>-</td><td>常规链路</td></tr><tr><td>0x0302</td><td>30</td><td>自定义控制器与机器人交互数据,发送方触发发送,频率上限为30Hz</td><td>自定义控制器→选手端图传连接的机器人</td><td>图传链路</td></tr><tr><td>0x0303</td><td>15</td><td>选手端小地图交互数据,选手端触发发送</td><td>选手端点击→服务器→发送方选择的己方机器人</td><td>常规链路</td></tr><tr><td>0x0305</td><td>24</td><td>选手端小地图接收雷达数据,频率上限为5Hz</td><td>雷达→服务器→己方所有选手端</td><td>常规链路</td></tr><tr><td>0x0306</td><td>8</td><td>自定义控制器与选手端交互数据,发送方触发电送,频率上限为30Hz</td><td>自定义控制器→选手端</td><td>-</td></tr><tr><td>0x0307</td><td>103</td><td>选手端小地图接收路径数据,频率上限为1Hz</td><td>哨兵/半自动控制机器人→对应操作手选手端</td><td>常规链路</td></tr><tr><td>0x0308</td><td>34</td><td>选手端小地图接收机器人数据,频率上限为3Hz</td><td>己方机器人→己方选手端</td><td>常规链路</td></tr><tr><td>0x0309</td><td>30</td><td>自定义控制器接收机器人数据,频率上限为10Hz</td><td>己方机器人→对应操作手选手端连接的自定义控制器</td><td>图传链路</td></tr><tr><td>0x0310</td><td>300</td><td>机器人发送给自定义客户端的数据,频率上限为50Hz</td><td>己方机器人→图传链路→对应操作手选手端连接的自定义客户端</td><td>图传链路</td></tr><tr><td>0x0311</td><td>30</td><td>自定义客户端发送给机器人的自定义指令,频率上限为75Hz</td><td>对应操作手选手端连接的自定义客户端→图传链路→己方机器人</td><td>图传链路</td></tr><tr><td>0x0A01</td><td>24</td><td>对方机器人的位置坐标,频率上限为10Hz</td><td>信号发射源→雷达</td><td>雷达无线链路</td></tr><tr><td>0x0A02</td><td>12</td><td>对方机器人的血量信息,频率上限为10Hz</td><td>信号发射源→雷达</td><td>雷达无线链路</td></tr><tr><td>0x0A03</td><td>10</td><td>对方机器人的剩余发弹量信息,频率上限为10Hz</td><td>信号发射源→雷达</td><td>雷达无线链路</td></tr><tr><td>0x0A04</td><td>8</td><td>对方队伍的宏观状态信息,频率上限为10Hz</td><td>信号发射源→雷达</td><td>雷达无线链路</td></tr><tr><td>0x0A05</td><td>36</td><td>对方各机器人当前增益效果,频率上限为10Hz</td><td>信号发射源→雷达</td><td>雷达无线链路</td></tr><tr><td>0x0A06</td><td>6</td><td>对方干扰波密钥，频率上限为10Hz</td><td>信号发射源→雷达</td><td>雷达无线链路</td></tr></table>
6 
$\circledcirc$ 2026 大疆 版权所有 
ROBOMASTER 

© 2026 大疆 版权所有 
7 
ROBOMASTER 

8 
$\circledcirc$ 2026 大疆 版权所有 
ROBOMASTER 


表 1-5 0x0001

<table><tr><td>字节偏移量</td><td>大小</td><td>说明</td></tr><tr><td>0</td><td>1</td><td>bit 0-3: 比赛类型1: RoboMaster 机甲大师超级对抗赛2: RoboMaster 机甲大师高校单项赛3: ICRA RoboMaster 高校人工智能挑战赛4: RoboMaster 机甲大师高校联盟赛 3V3 对抗5: RoboMaster 机甲大师高校联盟赛步兵对抗bit 4-7: 当前比赛阶段0: 未开始比赛1: 准备阶段2: 十五秒裁判系统自检阶段3: 五秒倒计时4: 比赛中5: 比赛结算中</td></tr><tr><td>1</td><td>2</td><td>当前阶段剩余时间,单位: 秒</td></tr><tr><td>3</td><td>8</td><td>UNIX 时间,当机器人正确连接到裁判系统的 NTP 服务器后生效</td></tr></table>
```txt
typedef _packed struct  
{ uint8_t game_type : 4; uint8_t game_progress : 4; uint16_t stage Remain_time; 
```
© 2026 大疆 版权所有 
9 
ROBOMASTER 
```txt
uint64_t SyncTimeStamp;   
}game_status_t; 
```

表 1-6 0x0002

<table><tr><td>字节偏移量</td><td>大小</td><td>说明</td></tr><tr><td>0</td><td>1</td><td>• 0: 平局
• 1: 红方胜利
• 2: 蓝方胜利</td></tr></table>
```txt
typedef _packed struct  
{ uint8_t winner; }game_result_t; 
```

表 1-7 0x0003

<table><tr><td>字节偏移量</td><td>大小</td><td>说明</td></tr><tr><td>0</td><td>2</td><td>己方1号英雄机器人血量，若该机器人未上场或者被罚下，则血量为0，下文同理</td></tr><tr><td>2</td><td>2</td><td>己方2号工程机器人血量</td></tr><tr><td>4</td><td>2</td><td>己方3号步兵机器人血量</td></tr><tr><td>6</td><td>2</td><td>己方4号步兵机器人血量</td></tr><tr><td>8</td><td>2</td><td>保留位</td></tr><tr><td>10</td><td>2</td><td>己方7号哨兵机器人血量</td></tr><tr><td>12</td><td>2</td><td>己方前哨站血量</td></tr><tr><td>14</td><td>2</td><td>己方基地血量</td></tr></table>
```txt
typedef _packed struct  
{  
uint16_t ally_1_robot_HP;  
uint16_t ally_2_robot_HP;  
uint16_t ally_3_robot_HP; 
```
10 
$\circledcirc$ 2026 大疆 版权所有 
ROBOMASTER 
```c
uint16_t ally_4_robot_HP;  
uint16_t reserved;  
uint16_t ally_7_robot_HP;  
uint16_t ally_outpost_HP;  
uint16_t ally_base_HP;  
} game_robot_HP_t; 
```

表 1-8 0x0101

<table><tr><td>字节偏移量</td><td>大小</td><td>说明</td></tr><tr><td>0</td><td>4</td><td>0: 未占领/未激活1: 已占领/已激活● bit 0-2:● bit 0: 己方与资源区区不重叠的补给区占领状态, 1 为已占领● bit 1: 己方与资源区重叠的补给区占领状态, 1 为已占领● bit 2: 己方补给区的占领状态, 1 为已占领(仅 RMUL 适用)● bit 3-6: 己方能量机关状态● bit 3-4: 己方小能量机关的激活状态, 0 为未激活, 1 为已激活,2 为正在激活● bit 5-6: 己方大能量机关的激活状态, 0 为未激活, 1 为已激活,2 为正在激活● bit 7-8: 己方中央高地的占领状态, 1 为被己方占领, 2 为被对方占领● bit 9-10: 己方梯形高地的占领状态, 1 为已占领● bit 11-19: 对方飞镖最后一次击中己方前哨站或基地的时间 (0-420, 开局默认为 0)● bit 20-22: 对方飞镖最后一次击中己方前哨站或基地的具体目标, 开局默认为 0, 1 为击中前哨站, 2 为击中基地固定目标, 3 为击中基地随机固定目标, 4 为击中基地随机移动目标, 5 为击中基地末端移动目标</td></tr><tr><td></td><td></td><td>● bit 23-24: 中心增益点的占领状态,0为未被占领,1为被己方占领,2为被对方占领,3为被双方占领。(仅RMUL适用)● bit 25-26: 己方堡垒增益点的占领状态,0为未被占领,1为被己方占领,2为被对方占领,3为被双方占领● bit 27-28: 己方前哨站增益点的占领状态,0为未被占领,1为被己方占领,2为被对方占领● bit 29: 己方基地增益点的占领状态,1为已占领● bit 30-31: 保留位</td></tr><tr><td colspan="3">typedef packed struct{uint32_t event_data;}event_data_t;</td></tr></table>
© 2026 大疆 版权所有 
11 
ROBOMASTER 


表 1-9 0x0104

<table><tr><td>字节偏移量</td><td>大小</td><td>说明</td></tr><tr><td>0</td><td>1</td><td>己方最后一次受到判罚的等级:1: 双方黄牌2: 黄牌3: 红牌4: 判负</td></tr><tr><td>1</td><td>1</td><td>己方最后一次受到判罚的违规机器人ID。(如红1机器人ID为1,蓝1机器人ID为101)判负和双方黄牌时,该值为0</td></tr><tr><td>2</td><td>1</td><td>己方最后一次受到判罚的违规机器人对应判罚等级的违规次数。(开局默认为0。)</td></tr><tr><td colspan="3">typedef _packed struct{uint8_t level;</td></tr></table>
12 
$\circledcirc$ 2026 大疆 版权所有 
ROBOMASTER 
```txt
uint8_t offending_robot_id;  
uint8_t count;  
}refereewarning_t; 
```

表 1-10 0x0105

<table><tr><td>字节偏移量</td><td>大小</td><td>说明</td></tr><tr><td>0</td><td>1</td><td>己方飞镖发射剩余时间，单位：秒</td></tr><tr><td>1</td><td>2</td><td>● bit 0-2:最近一次己方飞镖击中的目标，开局默认为0，1为击中前哨站，2为击中基地固定目标，3为击中基地随机固定目标，4为击中基地随机移动目标，5为击中基地末端移动目标● bit 3-5:对方最近被击中的目标累计被击中计次数，开局默认为0，至多为4● bit 6-8:飞镖此时选定的击打目标，开局默认或未选定/选定前哨站时为0，选中基地固定目标为1，选中基地随机固定目标为2，选中基地随机移动目标为3，选中基地末端移动目标为4● bit 9-15:保留</td></tr></table>
```txt
typedef _packed struct  
{ uint8_t dart_remaining_time; uint16_t dart_info; }dart_info_t; 
```

表 1-11 0x0201

<table><tr><td>字节偏移量</td><td>大小</td><td>说明</td></tr><tr><td>0</td><td>1</td><td>本机器人ID</td></tr><tr><td>1</td><td>1</td><td>机器人等级</td></tr><tr><td>2</td><td>2</td><td>机器人当前血量</td></tr><tr><td>4</td><td>2</td><td>机器人血量上限</td></tr><tr><td>6</td><td>2</td><td>机器人射击热量每秒冷却值</td></tr><tr><td>8</td><td>2</td><td>机器人射击热量上限</td></tr><tr><td>10</td><td>2</td><td>机器人底盘功率上限</td></tr><tr><td>12</td><td>1</td><td>电源管理模块的输出情况:● bit 0: gimbal口输出, 0为无输出, 1为24V输出● bit 1: chassis口输出, 0为无输出, 1为24V输出● bit 2: shooter口输出, 0为无输出, 1为24V输出</td></tr><tr><td colspan="3">typedef _packed struct{uint8_t robot_id;uint8_t robot_level;uint16_t current_HP;uint16_t maximum_HP;uint16_t shooter_barrel_cooling_value;uint16_t shooter_barrel_heat_limit;uint16_t chassis_power_limit;uint8_t power_management_gimbal_output : 1;uint8_t power_management chassis_output : 1;uint8_t power_management_shooter_output : 1;}robot_status_t;</td></tr></table>
© 2026 大疆 版权所有 
13 
ROBOMASTER 


表 1-12 0x0202

<table><tr><td>字节偏移量</td><td>大小</td><td>说明</td></tr><tr><td>0</td><td>2</td><td>保留位</td></tr><tr><td>2</td><td>2</td><td>保留位</td></tr><tr><td>4</td><td>4</td><td>保留位</td></tr><tr><td>8</td><td>2</td><td>缓冲能量（单位：J）</td></tr><tr><td>10</td><td>2</td><td>17mm发射机构的射击热量</td></tr><tr><td>12</td><td>2</td><td>42mm发射机构的射击热量</td></tr></table>
14 
$\circledcirc$ 2026 大疆 版权所有 
ROBOMASTER 

```c
typedef _packed struct   
{ uint16_t reserved; uint16_t reserved; float reserved; uint16_t buffer_energy; uint16_t shooter_17mm_barrel_heat; uint16_t shooter_42mm_barrel_heat; }power_heat_data_t; 
```

表 1-13 0x0203

<table><tr><td>字节偏移量</td><td>大小</td><td>说明</td></tr><tr><td>0</td><td>4</td><td>本机器人位置x坐标，单位：m</td></tr><tr><td>4</td><td>4</td><td>本机器人位置y坐标，单位：m</td></tr><tr><td>8</td><td>4</td><td>本机器人测速模块的朝向，单位：度。正北为0度</td></tr></table>
```txt
typedef _packed struct  
{ float x; float y; float angle; }robot_pos_t; 
```

表 1-14 0x0204

<table><tr><td>字节偏移量</td><td>大小</td><td>说明</td></tr><tr><td>0</td><td>1</td><td>机器人回血增益（百分比，值为10表示每秒恢复血量上限的10%）</td></tr><tr><td>1</td><td>2</td><td>机器人射击热量冷却增益具体值（直接值，值为x表示热量冷却增加x/s）</td></tr><tr><td>3</td><td>1</td><td>机器人防御增益（百分比，值为50表示50%防御增益）</td></tr><tr><td>4</td><td>1</td><td>机器人负防御增益（百分比，值为30表示-30%防御增益）</td></tr><tr><td>5</td><td>2</td><td>机器人攻击增益（百分比，值为50表示50%攻击增益）</td></tr><tr><td>7</td><td>1</td><td>bit 0-6：机器人剩余能量值反馈，以16进制标识机器人剩余能量值比例，仅在机器人剩余能量小于50%时反馈，其余默认反馈0x80。机器人初始能量视为100%
● bit 0：在剩余能量≥125%时为1，其余情况为0
● bit 1：在剩余能量≥100%时为1，其余情况为0
● bit 2：在剩余能量≥50%时为1，其余情况为0
● bit 3：在剩余能量≥30%时为1，其余情况为0
● bit 4：在剩余能量≥15%时为1，其余情况为0
● bit 5：在剩余能量≥5%时为1，其余情况为0
● bit 6：在剩余能量≥1%时为1，其余情况为0</td></tr></table>
© 2026 大疆 版权所有 
15 
ROBOMASTER 

```c
typedef _packed struct  
{ uint8_t recovery BUFF; uint16_t cooling BUFF; uint8_t defence BUFF; uint8_t vulnerability BUFF; uint16_t attack BUFF; uint8_t remaining_energy; }buff_t; 
```
16 
$\circledcirc$ 2026 大疆 版权所有 
ROBOMASTER 

表 1-15 0x0206

<table><tr><td>字节偏移量</td><td>大小</td><td>说明</td></tr><tr><td>0</td><td>1</td><td>bit 0-3: 当扣血原因为装甲模块被弹丸攻击、受撞击或离线时,该4 bit 组成的数值为装甲模块或测速模块的ID编号;当其他原因导致扣血时,该数值为0bit 4-7: 血量变化类型0: 装甲模块被弹丸攻击导致扣血1: 装甲模块或超级电容管理模块离线导致扣血5: 装甲模块受到撞击导致扣血</td></tr><tr><td colspan="3">typedef _packed struct{uint8_t armor_id : 4;uint8_t HP_deduction_reason : 4;}hurt_data_t;</td></tr></table>
![image](https://cdn-mineru.openxlab.org.cn/result/2026-02-28/a72e60af-c3aa-4d9e-8e93-7eb7c3ff3a17/6bdf7830d34dbeb56d58c3707b296670867edba93f1c2cdeb979417cc03d3cec.jpg)

$0 \mathrm { { x } 0 2 0 6 }$ 的受伤害情况为机器人裁判系统本地判定，即时发送，但实际是否受到对应伤害受规 则条例影响，请以服务器最终判定为准。 

表 1-16 0x0207

<table><tr><td>字节偏移量</td><td>大小</td><td>说明</td></tr><tr><td>0</td><td>1</td><td>弹丸类型:
● bit 1: 17mm 弹丸
● bit 2: 42mm 弹丸</td></tr><tr><td>1</td><td>1</td><td>发射机构 ID:
● 1: 17mm 发射机构
● 2: 保留位
● 3: 42mm 发射机构</td></tr><tr><td>2</td><td>1</td><td>弹丸射速（单位：Hz）</td></tr><tr><td>3</td><td>4</td><td>弹丸初速度（单位：m/s）</td></tr></table>
© 2026 大疆 版权所有 
17 
ROBOMASTER 

```txt
typedef _packed struct   
{ uint8_tbullet_type; uint8_tshooter_number; uint8_tlaunching_freqency; float initial_speed;   
}shoot_data_t; 
```

注：所有 RFID 卡仅在赛内生效。在赛外，即使检测到对应的 RFID 卡，对应值也为 0。 


表 1-17 0x0208

<table><tr><td>字节偏移量</td><td>大小</td><td>说明</td></tr><tr><td>0</td><td>2</td><td>机器人自身拥有的17mm弹丸允许发弹量</td></tr><tr><td>2</td><td>2</td><td>42mm弹丸允许发弹量</td></tr><tr><td>4</td><td>2</td><td>剩余金币数量</td></tr><tr><td>6</td><td>2</td><td>堡垒增益点提供的储备17mm弹丸允许发弹量;该值与机器人是否实际占领堡垒无关</td></tr></table>
```txt
typedef _packed struct  
{ uint16_t projector allowance_17mm; uint16_t projector allowance_42mm; uint16_t remaining_goldcoin; uint16_t projector allowance_fortress; }projectile allowances_t; 
```

表 1-18 0x0209

<table><tr><td>字节偏移量</td><td>大小</td><td>说明</td></tr><tr><td>0</td><td>4</td><td>bit位值为1/0的含义：是否已检测到该增益点RFID卡● bit 0:己方基地增益点</td></tr><tr><td></td><td></td><td>● bit 1: 己方中央高地增益点● bit 2: 对方中央高地增益点● bit 3: 己方梯形高地增益点● bit 4: 对方梯形高地增益点● bit 5: 己方地形跨越增益点 (飞坡) (靠近己方一侧飞坡前)● bit 6: 己方地形跨越增益点 (飞坡) (靠近己方一侧飞坡后)● bit 7: 对方地形跨越增益点 (飞坡) (靠近对方一侧飞坡前)● bit 8: 对方地形跨越增益点 (飞坡) (靠近对方一侧飞坡后)● bit 9: 己方地形跨越增益点 (中央高地下方)● bit 10: 己方地形跨越增益点 (中央高地上方)● bit 11: 对方地形跨越增益点 (中央高地下方)● bit 12: 对方地形跨越增益点 (中央高地上方)● bit 13: 己方地形跨越增益点 (公路下方)● bit 14: 己方地形跨越增益点 (公路上方)● bit15: 对方地形跨越增益点 (公路下方)● bit16: 对方地形跨越增益点 (公路上方)● bit 17: 己方堡垒增益点● bit 18: 己方前哨站增益点● bit 19: 己方与资源区不重叠的补给区/RMUL 补给区● bit 20: 己方与资源区重叠的补给区● bit 21: 己方装配增益点● bit 22: 对方装配增益点● bit 23: 中心增益点 (仅 RMUL 适用)● bit 24: 对方堡垒增益点● bit 25: 对方前哨站增益点</td></tr><tr><td></td><td></td><td>● bit 26: 己方地形跨越增益点 (隧道) (靠近己方一侧公路区下方)● bit 27: 己方地形跨越增益点 (隧道) (靠近己方一侧公路区中间)● bit 28: 己方地形跨越增益点 (隧道) (靠近己方一侧公路区上方)● bit 29: 己方地形跨越增益点 (隧道) (靠近己方梯形高地较低处)● bit 30: 己方地形跨越增益点 (隧道) (靠近己方梯形高地较中间)● bit 31: 己方地形跨越增益点 (隧道) (靠近己方梯形高地较高处)</td></tr><tr><td>4</td><td>1</td><td>● bit 0: 对方地形跨越增益点 (隧道) (靠近对方公路一侧下方)● bit 1: 对方地形跨越增益点 (隧道) (靠近对方公路一侧中间)● bit 2: 对方地形跨越增益点 (隧道) (靠近对方公路一侧上方)● bit 3: 对方地形跨越增益点 (隧道) (靠近对方梯形高地较低处)● bit 4: 对方地形跨越增益点 (隧道) (靠近对方梯形高地较中间)● bit 5: 对方地形跨越增益点 (隧道) (靠近对方梯形高地较高处)</td></tr></table>
18 
$\circledcirc$ 2026 大疆 版权所有 
ROBOMASTER 




© 2026 大疆 版权所有 
19 
ROBOMASTER 




```txt
typedef _packed struct  
{ uint32_t rdid_status; uint8_t rdid_status_2; }rfid_status_t; 
```

表 1-19 0x020A

<table><tr><td>字节偏移量</td><td>大小</td><td>说明</td></tr><tr><td>0</td><td>1</td><td>当前飞镖发射站的状态: 
• 1: 关闭 
• 2: 正在开启或者关闭中 
• 0: 已经开启</td></tr><tr><td>1</td><td>1</td><td>保留位</td></tr><tr><td>2</td><td>2</td><td>切换击打目标时的比赛剩余时间，单位：秒，无/未切换动作，默认为0。</td></tr><tr><td>4</td><td>2</td><td>最后一次操作手确定发射指令时的比赛剩余时间，单位：秒，初始值为0。</td></tr></table>
20 
$\circledcirc$ 2026 大疆 版权所有 
ROBOMASTER 

```txt
typedef _packed struct   
{ uint8_t dart-launch_opening_status; uint8_t reserved; uint16_t target_change_time; uint16_t latest_launch_cmd_time; }dart_client_cmd_t; 
```

表 1-20 0x020B

<table><tr><td>字节偏移量</td><td>大小</td><td>说明</td></tr><tr><td>0</td><td>4</td><td>己方英雄机器人位置x轴坐标，单位：m</td></tr><tr><td>4</td><td>4</td><td>己方英雄机器人位置y轴坐标，单位：m</td></tr><tr><td>8</td><td>4</td><td>己方工程机器人位置x轴坐标，单位：m</td></tr><tr><td>12</td><td>4</td><td>己方工程机器人位置y轴坐标，单位：m</td></tr><tr><td>16</td><td>4</td><td>己方3号步兵机器人位置x轴坐标，单位：m</td></tr><tr><td>20</td><td>4</td><td>己方3号步兵机器人位置y轴坐标，单位：m</td></tr><tr><td>24</td><td>4</td><td>己方4号步兵机器人位置x轴坐标，单位：m</td></tr><tr><td>28</td><td>4</td><td>己方4号步兵机器人位置y轴坐标，单位：m</td></tr><tr><td>32</td><td>4</td><td>保留位</td></tr><tr><td>36</td><td>4</td><td>保留位</td></tr></table>
© 2026 大疆 版权所有 
ROBOMASTER 
![image](https://cdn-mineru.openxlab.org.cn/result/2026-02-28/a72e60af-c3aa-4d9e-8e93-7eb7c3ff3a17/3568b34587d1373915f4a778993c55c864cb7811f23749d00ee3cd9cdf9d726c.jpg)

场地围挡在红方补给站附近的交点为坐标原点，沿场地长边向蓝方为 X轴正方向，沿场地短边 向红方停机坪为 Y 轴正方向。 
```txt
typedef _packed struct   
{ float hero_x; float hero_y; float engineer_x; float engineer_y; float standard_3_x; float standard_3_y; float standard_4_x; float standard_4_y; float reserved; float reserved;   
}ground_robot_position_t; 
```

表 1-21 0x020C

<table><tr><td>字节偏移量</td><td>大小</td><td>说明</td><td>备注</td></tr><tr><td>0</td><td>2</td><td>● bit 0: 对方 1 号英雄机器人易伤情况● bit 1: 对方 2 号工程机器人易伤情况● bit 2: 对方 3 号步兵机器人易伤情况● bit 3: 对方 4 号步兵机器人易伤情况● bit 4: 对方空中机器人特殊标识情况● bit 5: 对方哨兵机器人易伤情况● bit 6: 己方 1 号英雄机器人特殊标识情况● bit 7: 己方 2 号工程机器人特殊标识情况● bit 8: 己方 3 号步兵机器人特殊标识情况</td><td>●对方机器人: 在对应机器人被标记进度≥100时发送1, 被标记进度&lt;100时发送0。●己方机器人: 在对应机器人被标记进度≥50时发送1, 被标记进度&lt;50时发送0。</td></tr><tr><td></td><td></td><td>● bit9: 己方 4 号步兵机器人特殊标识情况● bit10: 己方空中机器人特殊标识情况● bit11: 己方哨兵机器人特殊标识情况● bit12-15: 保留位</td><td></td></tr></table>
22 
$\circledcirc$ 2026 大疆 版权所有 
ROBOMASTER 

```txt
typedef _packed struct  
{ uint16_t mark progress; }radar_mark_data_t; 
```

表 1-22 0x020D

<table><tr><td>字节偏移量</td><td>大小</td><td>说明</td></tr><tr><td>0</td><td>4</td><td>● bit 0-10: 除远程兑换外,哨兵机器人成功兑换的允许发弹量,开局为0,在哨兵机器人成功兑换一定允许发弹量后,该值将变为哨兵机器人成功兑换的允许发弹量值● bit 11-14: 喊兵机器人成功远程兑换允许发弹量的次数,开局为0,在哨兵机器人成功远程兑换允许发弹量后,该值将变为哨兵机器人成功远程兑换允许发弹量的次数● bit 15-18: 喊兵机器人成功远程兑换血量的次数,开局为0,在哨兵机器人成功远程兑换血量后,该值将变为哨兵机器人成功远程兑换血量的次数● bit 19: 喊兵机器人当前是否可以确认免费复活,可以确认免费复活时值为1,否则为0● bit 20: 喊兵机器人当前是否可以兑换立即复活,可以兑换立即复活时值为1,否则为0● bit 21-30: 喊兵机器人当前若兑换立即复活需要花费的金币数。● bit 31: 保留</td></tr><tr><td>4</td><td>2</td><td>● bit 0: 喊兵当前是否处于脱战状态,处于脱战状态时为1,否则为0</td></tr><tr><td></td><td></td><td>● bit 1-11: 队伍 17mm 允许发弹量的剩余可兑换数
● bit 12-13: 喊兵当前姿态, 1 为进攻姿态, 2 为防御姿态, 3 为移动姿态
● bit 14: 己方能量机关是否能够进入正在激活状态, 1 为当前可激活
● bit 15: 保留位</td></tr><tr><td colspan="3">typedef _packed struct
{
    uint32_t sentry_info;
    uint16_t sentry_info_2;
} sentry_info_t;</td></tr></table>
© 2026 大疆 版权所有 
23 
ROBOMASTER 


表 1-23 0x020E

<table><tr><td>字节偏移量</td><td>大小</td><td>说明</td></tr><tr><td>0</td><td>1</td><td>● bit0-1: 雷达是否拥有触发双倍易伤的机会, 开局为 0 , 数值为雷达拥有触发双倍易伤的机会, 至多为 2● bit2: 对方是否正在被触发双倍易伤\( \blacktriangleright \) 0: 对方未被触发双倍易伤\( \blacktriangleright \) 1: 对方正在被触发双倍易伤● bit3-4: 己方加密等级 (即对方干扰波难度等级), 开局为 1 , 最高为 3● bit5: 当前是否可以修改密钥, 1 为可修改● bit6-7: 保留位</td></tr><tr><td colspan="3">typedef packed struct\{uint8_t radar_info;\} radar_info_t;</td></tr></table>
机器人交互数据通过常规链路发送，其数据段包含一个统一的数据段头结构。数据段头结构包括内容 ID、 发送者和接收者的ID、内容数据段。机器人交互数据包的总长不超过 127个字节，减去frame_header、 cmd_id 和 frame_tail 的 9 个字节以及数据段头结构的 6 个字节，故机器人交互数据的内容数据段最大 为112个字节。 
24 
$\circledcirc$ 2026 大疆 版权所有 
ROBOMASTER 
每1000毫秒，英雄、工程、步兵、空中机器人、飞镖能够接收数据的上限为3720字节，雷达和哨兵机器 人能够接收数据的上限为5120字节。 
由于存在多个内容 ID，但整个 cmd_id上行频率最大为 $3 0 \mathrm { H z }$ ，请合理安排带宽。 

表 1-24 0x0301

<table><tr><td>字节偏移量</td><td>大小</td><td>说明</td><td>备注</td></tr><tr><td>0</td><td>2</td><td>子内容ID</td><td>需为开放的子内容ID</td></tr><tr><td>2</td><td>2</td><td>发送者ID</td><td>需与自身ID匹配，ID编号详见附录</td></tr><tr><td>4</td><td>2</td><td>接收者ID</td><td>●仅限己方通信●需为规则允许的多机通讯接收者●若接收者为选手端，则仅可发送至发送者对应的选手端●ID编号详见附录</td></tr><tr><td>6</td><td>x</td><td>内容数据段</td><td>x最大为112</td></tr></table>
```c
typedef _packed struct  
{ uint16_t data_cmd_id; uint16_t sender_id; uint16_t receiver_id; uint8_t user_data[x]; }robot_interaction_data_t; 
```
<table><tr><td>子内容ID</td><td>内容数据段长度</td><td>功能说明</td></tr><tr><td>0x0200~0x02FF</td><td>x≤112</td><td>机器人之间通信</td></tr><tr><td>0x0100</td><td>2</td><td>选手端删除图层</td></tr><tr><td>0x0101</td><td>15</td><td>选手端绘制一个图形</td></tr><tr><td>0x0102</td><td>30</td><td>选手端绘制两个图形</td></tr><tr><td>0x0103</td><td>75</td><td>选手端绘制五个图形</td></tr><tr><td>0x0104</td><td>105</td><td>选手端绘制七个图形</td></tr><tr><td>0x0110</td><td>45</td><td>选手端绘制字符图形</td></tr><tr><td>0x0120</td><td>4</td><td>哨兵自主决策指令</td></tr><tr><td>0x0121</td><td>1</td><td>雷达自主决策指令</td></tr></table>
© 2026 大疆 版权所有 
25 
ROBOMASTER 


表 1-25 子内容 ID：0x0100

<table><tr><td>字节偏移量</td><td>大小</td><td>说明</td><td>备注</td></tr><tr><td>0</td><td>1</td><td>删除操作</td><td>• 0: 空操作
• 1: 删除图层
• 2: 删除所有</td></tr><tr><td>1</td><td>1</td><td>图层数</td><td>图层数: 0~9</td></tr></table>
```txt
typedef _packed struct  
{ uint8_t delete_type; uint8_t layer; }interaction_layer_delete_t; 
```

表 1-26 子内容 ID：0x0101

<table><tr><td>字节偏移量</td><td>大小</td><td>说明</td><td>备注</td></tr><tr><td>0</td><td>3</td><td>图形名</td><td>在图形删除、修改等操作中，作为索引</td></tr><tr><td>3</td><td>4</td><td>图形配置1</td><td>bit 0-2: 图形操作
● 0: 空操作
● 1: 增加
● 2: 修改
● 3: 删除</td></tr><tr><td></td><td></td><td></td><td>bit 3-5: 图形类型0: 直线1: 矩形2: 正圆3: 椭圆4: 圆弧5: 浮点数6: 整型数7: 字符bit 6-9: 图层数 (0~9)bit 10-13: 颜色0: 红/蓝 (己方颜色)1: 黄色2: 绿色3: 橙色4: 紫红色5: 粉色6: 青色7: 黑色8: 白色bit 14-31: 根据绘制的图形不同,含义不同,详见“表 1-27 图形细节参数说明”</td></tr><tr><td>7</td><td>4</td><td>图形配置2</td><td>bit 0-9: 线宽,建议字体大小与线宽比例为 10: 1bit 10-20: 起点/圆心 x 坐标bit 21-31: 起点/圆心 y 坐标</td></tr><tr><td>11</td><td>4</td><td>图形配置3</td><td>根据绘制的图形不同，含义不同，详见“表1-27 图形细节参数说明”</td></tr></table>
26 
$\circledcirc$ 2026 大疆 版权所有 
ROBOMASTER 

© 2026 大疆 版权所有 
27 
ROBOMASTER 

```txt
typedef _packed struct  
{ uint8_t figure_name[3]; uint32_t operate_tpye:3; uint32_t figure_tpye:3; uint32_t layer:4; uint32_t color:4; uint32_t details_a:9; uint32_t details_b:9; uint32_t width:10; uint32_t start_x:11; uint32_t start_y:11; uint32_t details_c:10; uint32_t details_d:11; uint32_t details_e:11; }interaction_figure_t; 
```

表 1-27 图形细节参数说明

<table><tr><td>类型</td><td>details_a</td><td>details_b</td><td>details_c</td><td>details_d</td><td>details_e</td></tr><tr><td>直线</td><td>-</td><td>-</td><td>-</td><td>终点x坐标</td><td>终点y坐标</td></tr><tr><td>矩形</td><td>-</td><td>-</td><td>-</td><td>对角顶点x坐标</td><td>对角顶点y坐标</td></tr><tr><td>正圆</td><td>-</td><td>-</td><td>半径</td><td>-</td><td>-</td></tr><tr><td>椭圆</td><td>-</td><td>-</td><td>-</td><td>x半轴长度</td><td>y半轴长度</td></tr><tr><td>圆弧</td><td>起始角度</td><td>终止角度</td><td>-</td><td>x半轴长度</td><td>y半轴长度</td></tr><tr><td>浮点数</td><td>字体大小</td><td>无作用</td><td colspan="3">该值除以1000即实际显示值</td></tr><tr><td>整型数</td><td>字体大小</td><td>-</td><td colspan="3">32位整型数，int32_t</td></tr><tr><td>字符</td><td>字体大小</td><td>字符长度</td><td>-</td><td>-</td><td>-</td></tr></table>
28 
$\circledcirc$ 2026 大疆 版权所有 
ROBOMASTER 

角度值含义为： $0 ^ { \circ }$ 指12点钟方向，顺时针绘制； 
屏幕位置：（0,0）为屏幕左下角（1920，1080）为屏幕右上角； 
![image](https://cdn-mineru.openxlab.org.cn/result/2026-02-28/a72e60af-c3aa-4d9e-8e93-7eb7c3ff3a17/3b9eb23f381e8eaaa3279264551e1dbd3cff8fda0aaa48b13be6f51c60536b2d.jpg)

浮点数：整型数均为32位，对于浮点数，实际显示的值为输入的值/1000，如在details_c、 details_d、details_e 对应的字节输入 1234，选手端实际显示的值将为 1.234。 
即使发送的数值超过对应数据类型的限制，图形仍有可能显示，但此时不保证显示的效 果。 

表 1-28 子内容 ID：0x0102

<table><tr><td>字节偏移量</td><td>大小</td><td>说明</td><td>备注</td></tr><tr><td>0</td><td>15</td><td>图形1</td><td>与0x0101的数据段相同</td></tr><tr><td>15</td><td>15</td><td>图形2</td><td>与0x0101的数据段相同</td></tr></table>
```txt
typedef _packed struct   
{ interaction_figure_t interaction_figure[2]; }interaction_figure_2_t; 
```
© 2026 大疆 版权所有 
29 
ROBOMASTER 

表 1-29 子内容 ID：0x0103

<table><tr><td>字节偏移量</td><td>大小</td><td>说明</td><td>备注</td></tr><tr><td>0</td><td>15</td><td>图形1</td><td>与0x0101的数据段相同</td></tr><tr><td>15</td><td>15</td><td>图形2</td><td>与0x0101的数据段相同</td></tr><tr><td>30</td><td>15</td><td>图形3</td><td>与0x0101的数据段相同</td></tr><tr><td>45</td><td>15</td><td>图形4</td><td>与0x0101的数据段相同</td></tr><tr><td>60</td><td>15</td><td>图形5</td><td>与0x0101的数据段相同</td></tr></table>
```txt
typedef _packed struct   
{ interaction_figure_t interaction_figure[5]; }interaction_figure_3_t; 
```

表 1-30 子内容 ID：0x0104

<table><tr><td>字节偏移量</td><td>大小</td><td>说明</td><td>备注</td></tr><tr><td>0</td><td>15</td><td>图形1</td><td>与0x0101的数据段相同</td></tr><tr><td>15</td><td>15</td><td>图形2</td><td>与0x0101的数据段相同</td></tr><tr><td>30</td><td>15</td><td>图形3</td><td>与0x0101的数据段相同</td></tr><tr><td>45</td><td>15</td><td>图形4</td><td>与0x0101的数据段相同</td></tr><tr><td>60</td><td>15</td><td>图形5</td><td>与0x0101的数据段相同</td></tr><tr><td>75</td><td>15</td><td>图形6</td><td>与0x0101的数据段相同</td></tr><tr><td>90</td><td>15</td><td>图形7</td><td>与0x0101的数据段相同</td></tr></table>
```c
typedef _packed struct   
{ interaction_figure_t interaction_figure[7]; 
```
30 
$\circledcirc$ 2026 大疆 版权所有 
ROBOMASTER 
}interaction_figure_4_t; 

表 1-31 子内容 ID：0x0110

<table><tr><td>字节偏移量</td><td>大小</td><td>说明</td><td>备注</td></tr><tr><td>0</td><td>2</td><td>数据的内容ID</td><td>0x0110</td></tr><tr><td>2</td><td>2</td><td>发送者的ID</td><td>需要校验发送者的ID正确性</td></tr><tr><td>4</td><td>2</td><td>接收者的ID</td><td>需要校验接收者的ID正确性，仅支持发送机器人对应的选手端</td></tr><tr><td>6</td><td>15</td><td>字符配置</td><td>详见图形数据介绍</td></tr><tr><td>21</td><td>30</td><td>字符</td><td>-</td></tr></table>
```txt
typedef _packed struct   
{ graphic_data_struct_t grapic_data_struct; uint8_t data[30];   
}ext_client(custom_character_t; 
```

表 1-32 哨兵自主决策指令：0x0120

<table><tr><td>字节偏移量</td><td>大小</td><td>说明</td><td>备注</td></tr><tr><td>0</td><td>4</td><td>哨兵自主决策相关指令</td><td>● bit 0: 喊兵机器人是否确认复活
► 0 表示哨兵机器人确认不复活，即使此时哨兵的复活读条已经完成
► 1 表示哨兵机器人确认复活，若复活读条完成将立即复活
● bit 1: 喊兵机器人是否确认兑换立即复活
► 0 表示哨兵机器人确认不兑换立即复活;</td></tr><tr><td></td><td></td><td></td><td>&gt;1表示哨兵机器人确认兑换立即复活,若此时哨兵机器人符合兑换立即复活的规则要求,则会立即消耗金币兑换立即复活●bit 2-12:哨兵将要兑换的发弹量值,开局为0,修改此值后,哨兵在补血点即可兑换允许发弹量此值的变化需要单调递增,否则视为不合法。示例:此值开局仅能为0,此后哨兵可将其从0修改至X,则消耗X金币成功兑换X允许发弹量。此后哨兵可将其从X修改至X+Y,以此类推。●bit 13-16:哨兵远程兑换发弹量的请求次数,开局为0,修改此值即可请求远程兑换发弹量此值的变化需要单调递增且每次仅能增加1,否则视为不合法。示例:此值开局仅能为0,此后哨兵可将其从0修改至1,则消耗金币远程兑换允许发弹量。此后哨兵可将其从1修改至2,以此类推。●bit 17-20:哨兵远程兑换血量的请求次数,开局为0,修改此值即可请求远程兑换血量此值的变化需要单调递增且每次仅能增加1,否则视为不合法。示例:此值开局仅能为0,此后哨兵可将其从0修改至1,则消耗金币远程兑换血量。此后哨兵可将其从1修改至2,以此类推。在哨兵发送该子命令时,服务器将按照从相对低位到相对高位的原则依次处理这些指令,直至全部成功或不能处理为止。示例:若队伍金币数为0,此时哨兵战亡,“是否确认复活”的值为1,“是否确认兑换立即复活”的值为1,“确认兑换的允许发弹量值”为100。(假定之前哨兵未兑换过允许发弹量)由于此时队伍金币数不足以使哨兵兑换立即复活,则服务器将会忽视后续指令,等待哨兵发送的下一组指令。</td></tr><tr><td></td><td></td><td></td><td>● bit 21-22: 喊兵修改当前姿态指令, 1为进攻姿态, 2为防御姿态, 3为移动姿态, 默认为3; 修改此值即可改变哨兵姿态。● bit 23: 喊兵机器人是否确认使能量机关进入正在激活状态, 1为确认。默认为0。● bit 24-31: 保留位。</td></tr><tr><td colspan="4">typedef _packed struct{uint32_t sentry_cmd;} sentry_cmd_t;</td></tr></table>
© 2026 大疆 版权所有 
31 
ROBOMASTER 

32 
$\circledcirc$ 2026 大疆 版权所有 
ROBOMASTER 


表 1-33 雷达自主决策指令：0x0121

<table><tr><td>字节偏移量</td><td>大小</td><td>说明</td><td>备注</td></tr><tr><td>0</td><td>1</td><td>雷达是否确认触发双倍易伤</td><td>开局为0，修改此值即可请求触发双倍易伤，若此时雷达拥有触发双倍易伤的机会，则可触发。此值的变化需要单调递增且每次仅能增加1，否则视为不合法。示例：此值开局仅能为0，此后雷达可将其从0修改至1，若雷达拥有触发双倍易伤的机会，则触发双倍易伤。此后雷达可将其从1修改至2，以此类推。若雷达请求双倍易伤时，双倍易伤正在生效，则第二次双倍易伤将在第一次双倍易伤结束后生效。</td></tr><tr><td>1</td><td>7</td><td>密钥更新或验证指令</td><td>每个字节均为ASCII码编码的字母或数字。开局为随机值。byte1为指令类型，byte2-7为密钥值。当byte1值为1时，修改此值即可更新己方加密密钥；当byte1值为2时，修改此值即可将雷达破解的对方密钥传输给服务器以验证是否正确破解。注意:</td></tr><tr><td></td><td></td><td></td><td>● 仅开局和每次对方破解成功使得加密等级（己方干扰波难度）提高时可以修改密钥，其余时间修改无效。
● 当byte1值为2时，每次更新验证密钥后的10秒内，再次更新无效。</td></tr></table>
© 2026 大疆 版权所有 
33 
ROBOMASTER 

```txt
typedef _packed struct  
{ uint8_t radar_cmd; uint8_t password_cmd; uint8_t password_1; uint8_t password_2; uint8_t password_3; uint8_t password_4; uint8_t password_5; uint8_t password_6; } radar_cmd_t; 
```
34 
$\circledcirc$ 2026 大疆 版权所有 
ROBOMASTER 
# 1.3 小地图交互数据
# 1.3.1 选手端下发数据
云台手可通过选手端大地图向机器人发送固定数据。 
命令码为 0x0303，触发时发送，两次发送间隔不得低于 0.5 秒。 
# 发送方式一：
$\textcircled{1}$ 点击己方机器人头像； 
$\textcircled{2}$ （可选）按下一个键盘按键或点击对方机器人头像； 
$\textcircled{3}$ 点击小地图任意位置。该方式向己方选定的机器人发送地图坐标数据，若点击对方机器人头像，则以目 标机器人 ID 代替坐标数据。 
# 发送方式二：
$\textcircled{1}$ （可选）按下一个键盘按键或点击对方机器人头像； 
$\textcircled{2}$ 点击小地图任意位置。该方式向己方所有机器人发送地图坐标数据，若点击对方机器人头像，则以目标 机器人 ID代替坐标数据。 
 半自动控制方式的机器人对应的操作手可通过选手端大地图向机器人发送固定数据。 
命令码为 0x0303，触发时发送，两次发送间隔不得低于3秒。 
# 发送方式：
$\textcircled{1}$ （可选）按下一个键盘按键或点击对方机器人头像； 
$\textcircled{2}$ 点击小地图任意位置。该方式向操作手对应的机器人发送地图坐标数据，若点击对方机器人头像，则以 目标机器人ID代替坐标数据。 
一台半自动控制方式的机器人既可以接收云台手发送的信息，也可以接收对应操作手的信息。两种信息的 来源将在下表中“信息来源”中进行区别。 
![image](https://cdn-mineru.openxlab.org.cn/result/2026-02-28/a72e60af-c3aa-4d9e-8e93-7eb7c3ff3a17/41869013c8fabcd976e29572f1ea9ba9c39e00b6d2bf7c892a62e541d55f2228.jpg)

为降低机器人串口接收设备的偶发不稳定性对通信的影响， $0 \mathrm { { x } 0 3 0 3 }$ 协议的发送机制有所特殊 处理，具体如下：选手端触发 1 次发送后，服务器将以 100ms 的间隔向机器人额外发送 4 次，共 5 次。此后，直到下一次选手端触发发送前，服务器都将以1Hz的频率持续定频发送 最近一次的包。触发时的连续发送和 1Hz 定频发送计时相互独立。队伍需关注多次收到重复 协议内容的处理方式。 
© 2026 大疆 版权所有 
35 
ROBOMASTER 

表 1-34 命令码 ID：0x0303

<table><tr><td>字节偏移量</td><td>大小</td><td>说明</td><td>备注</td></tr><tr><td>0</td><td>4</td><td>目标位置x轴坐标，单位m</td><td>当发送目标机器人ID时，该值为0</td></tr><tr><td>4</td><td>4</td><td>目标位置y轴坐标，单位m</td><td>当发送目标机器人ID时，该值为0</td></tr><tr><td>8</td><td>1</td><td>云台手按下的键盘按键通用键值</td><td>无按键按下，则为0</td></tr><tr><td>9</td><td>1</td><td>对方机器人ID</td><td>当发送坐标数据时，该值为0</td></tr><tr><td>10</td><td>2</td><td>信息来源ID</td><td>信息来源的ID，ID对应关系详见附录</td></tr></table>
```sql
typedef _packed struct   
{ float target_position_x; float target_position_y; uint8_t cmdkeyboard; uint8_t target_robot_id; uint16_t cmd_source; }map_command_t; 
```
# 1.3.2 选手端接收数据
选手端小地图可接收机器人数据。 
雷达可通过常规链路向己方所有选手端发送对方机器人的坐标数据，该位置会在己方选手端小地图显示。 

表 1-35 命令码 ID：0x0305

<table><tr><td>字节偏移量</td><td>大小</td><td>说明</td><td>备注</td></tr><tr><td>0</td><td>2</td><td>英雄机器人x位置坐标，单位：cm</td><td rowspan="4">当x、y超出边界时显示在对应边缘处，当x、y均为0时，视为未发送此机器人坐标。</td></tr><tr><td>2</td><td>2</td><td>英雄机器人y位置坐标，单位：cm</td></tr><tr><td>4</td><td>2</td><td>工程机器人x位置坐标，单位：cm</td></tr><tr><td>6</td><td>2</td><td>工程机器人y位置坐标，单位：cm</td></tr><tr><td>8</td><td>2</td><td>3号步兵机器人x位置坐标，单位：cm</td><td rowspan="8"></td></tr><tr><td>10</td><td>2</td><td>3号步兵机器人y位置坐标，单位：cm</td></tr><tr><td>12</td><td>2</td><td>4号步兵机器人x位置坐标，单位：cm</td></tr><tr><td>14</td><td>2</td><td>4号步兵机器人y位置坐标，单位：cm</td></tr><tr><td>16</td><td>2</td><td>保留位</td></tr><tr><td>18</td><td>2</td><td>保留位</td></tr><tr><td>20</td><td>2</td><td>哨兵机器人x位置坐标，单位：cm</td></tr><tr><td>22</td><td>2</td><td>哨兵机器人y位置坐标，单位：cm</td></tr></table>
36 
$\circledcirc$ 2026 大疆 版权所有 
ROBOMASTER 

typedef _packed struct 
{ 
uint16_t hero_position_x; 
uint16_t hero_position_y; 
uint16_t engineer_position_x; 
uint16_t engineer_position_y; 
uint16_t infantry_3_position_x; 
uint16_t infantry_3_position_y; 
uint16_t infantry_4_position_x; 
uint16_t infantry_4_position_y; 
uint16_t reserved; 
uint16_t reserved; 
uint16_t sentry_position_x; 
uint16_t sentry_position_y; 
} map_robot_data_t; 
哨兵机器人或半自动控制方式的机器人可通过常规链路向对应的操作手选手端发送路径坐标数据，该路径 
$\circledcirc$ 2026 大疆 版权所有 
37 
ROBOMASTER 
会在小地图上显示。 

表 1-36 命令码 ID：0x0307

<table><tr><td>字节偏移量</td><td>大小</td><td>说明</td><td>备注</td></tr><tr><td>0</td><td>1</td><td>1:到目标点攻击2:到目标点防守3:移动到目标点</td><td>-</td></tr><tr><td>1</td><td>2</td><td>路径起点x轴坐标,单位:dm</td><td rowspan="2">小地图左下角为坐标原点,水平向右为X轴正方向,竖直向上为Y轴正方向。显示位置将按照场地尺寸与小地图尺寸等比缩放,超出边界的位置将在边界处显示</td></tr><tr><td>3</td><td>2</td><td>路径起点y轴坐标,单位:dm</td></tr><tr><td>5</td><td>49</td><td>路径点x轴增量数组,单位:dm</td><td rowspan="2">增量相较于上一个点位进行计算,共49个新点位,X与Y轴增量对应组成点位</td></tr><tr><td>54</td><td>49</td><td>路径点y轴增量数组,单位:dm</td></tr><tr><td>103</td><td>2</td><td>发送者ID</td><td>需与自身ID匹配,ID编号详见附录</td></tr></table>
```c
typedef _packed struct   
{ uint8_t intention; uint16_t start_position_x; uint16_t start_position_y; int8_t delta_x[49]; int8_t delta_y[49]; uint16_t sender_id; }map_data_t; 
```
己方机器人可通过常规链路向己方任意选手端发送自定义的消息，该消息会在己方选手端特定位置显示。 
38 
$\circledcirc$ 2026 大疆 版权所有 
ROBOMASTER 

表 1-37 命令码 ID：0x0308

<table><tr><td>字节偏移量</td><td>大小</td><td>说明</td><td>备注</td></tr><tr><td>0</td><td>2</td><td>发送者的 ID</td><td>需要校验发送者的 ID 正确性</td></tr><tr><td>2</td><td>2</td><td>接收者的 ID</td><td>需要校验接收者的 ID 正确性，仅支持发送己方选手端</td></tr><tr><td>4</td><td>30</td><td>字符</td><td>以 utf-16 格式编码发送，支持显示中文。编码发送时请注意数据的大小端问题</td></tr></table>
```c
typedef _packed struct   
{ uint16_t sender_id; uint16_t receiver_id; uint8_t user_data[30]; } custom_info_t; 
```
© 2026 大疆 版权所有 
39 
ROBOMASTER 
# 1.4 图传链路数据说明
# 1.4.1 自定义控制器与机器人交互数据说明
操作手可使用自定义控制器通过图传链路向对应的机器人发送数据。 

表 1-38 命令码 ID：0x0302

<table><tr><td>字节偏移量</td><td>大小</td><td>说明</td></tr><tr><td>0</td><td>30</td><td>自定义数据</td></tr></table>
```txt
typedef _packed struct { uint8_t data[x]; }custom_robot_data_t; 
```
机器人可通过图传链路向对应的操作手选手端连接的自定义控制器发送数据。 

表 1-39 命令码 ID：0x0309

<table><tr><td>字节偏移量</td><td>大小</td><td>说明</td></tr><tr><td>0</td><td>30</td><td>自定义数据</td></tr></table>
```txt
typedef _packed struct { uint8_t data[x]; }robot(custom_data_t; 
```
# 1.4.2 自定义客户端与机器人交互数据说明
机器人可以通过图传链路向自定义客户端发送自定义信息以及接受自定义客户端的自定义指令。 
注：由于数据量较大，该协议无重传机制。 

表 1-40 命令码 ID：0x0310

<table><tr><td>字节偏移量</td><td>大小</td><td>说明</td></tr><tr><td>0</td><td>300</td><td>自定义数据</td></tr></table>
40 
$\circledcirc$ 2026 大疆 版权所有 
ROBOMASTER 
```txt
typedef _packed struct { uint8_t data[x]; }robot(custom_data_2_t; 
```

表 1-41 命令码 ID：0x0311

<table><tr><td>字节偏移量</td><td>大小</td><td>说明</td></tr><tr><td>0</td><td>30</td><td>自定义客户端发送给机器人的自定义指令</td></tr></table>
```txt
typedef _packed struct { uint8_t data[x]; }robot(custom_data_3_t; 
```
# 1.5 非链路数据说明
操作手可使用自定义控制器模拟键鼠操作选手端。 

表 1-42 命令码 ID：0x0306

<table><tr><td>字节偏移量</td><td>大小</td><td>说明</td><td>备注</td></tr><tr><td>0</td><td>2</td><td>键盘键值:● bit 0-7: 按键1键值● bit 8-15: 按键2键值</td><td>● 仅响应选手端开放的按键● 使用通用键值, 支持2键无冲, 键值顺序变更不会改变按下状态, 若无新的按键信息, 将保持上一帧数据的按下状态</td></tr><tr><td>2</td><td>2</td><td>● bit 0-11: 鼠标X轴像素位置● bit 12-15: 鼠标左键状态</td><td rowspan="2">● 位置信息使用绝对像素点值(赛事客户端使用的分辨率为1920×1080,屏幕左上角为(0,0))</td></tr><tr><td>4</td><td>2</td><td>● bit 0-11: 鼠标Y轴像素位置● bit 12-15: 鼠标右键状态</td></tr><tr><td></td><td></td><td></td><td>● 鼠标按键状态1为按下,其他值为未按下,仅在出现鼠标图标后响应该信息,若无新的鼠标信息,选手端将保持上一帧数据的鼠标信息,当鼠标图标消失后该数据不再保持</td></tr><tr><td>6</td><td>2</td><td>保留位</td><td>-</td></tr></table>
© 2026 大疆 版权所有 
41 
ROBOMASTER 

![image](https://cdn-mineru.openxlab.org.cn/result/2026-02-28/a72e60af-c3aa-4d9e-8e93-7eb7c3ff3a17/6085e24b9b41aef787a1a1021f4ead70ec257b94dbaf2e830cfb846dc567069b.jpg)

一次鼠标移动点击需要先发送鼠标未按下及指定位置的数据帧，再发送保持该位置时按下鼠标 的数据帧，最后发送保持该位置时鼠标未按下的数据帧 
```c
typedef _packed struct  
{ uint16_t key_value; uint16_t x_position:12; uint16_t mouse_left:4; uint16_t y_position:12; uint16_t mouse_right:4; uint16_t reserved; }custom_client_data_t; 
```
# 1.6 雷达无线链路数据说明

表 1-43 命令码 ID：0x0A01

<table><tr><td>字节偏移量</td><td>大小</td><td>说明</td></tr><tr><td>0</td><td>2</td><td>对方英雄机器人位置x轴坐标，单位：cm</td></tr><tr><td>2</td><td>2</td><td>对方英雄机器人位置y轴坐标，单位：cm</td></tr><tr><td>4</td><td>2</td><td>对方工程机器人位置x轴坐标，单位：cm</td></tr><tr><td>6</td><td>2</td><td>对方工程机器人位置y轴坐标，单位：cm</td></tr><tr><td>8</td><td>2</td><td>对方3号步兵机器人位置x轴坐标，单位：cm</td></tr><tr><td>10</td><td>2</td><td>对方3号步兵机器人位置y轴坐标，单位：cm</td></tr><tr><td>12</td><td>2</td><td>对方4号步兵机器人位置x轴坐标，单位：cm</td></tr><tr><td>14</td><td>2</td><td>对方4号步兵机器人位置y轴坐标，单位：cm</td></tr><tr><td>16</td><td>2</td><td>对方空中机器人位置x轴坐标，单位：cm</td></tr><tr><td>18</td><td>2</td><td>对方空中机器人位置y轴坐标，单位：cm</td></tr><tr><td>20</td><td>2</td><td>对方哨兵机器人位置x轴坐标，单位：cm</td></tr><tr><td>22</td><td>2</td><td>对方哨兵机器人位置y轴坐标，单位：cm</td></tr></table>
42 
$\circledcirc$ 2026 大疆 版权所有 
ROBOMASTER 


表 1-44 命令码 ID：0x0A02

<table><tr><td>字节偏移量</td><td>大小</td><td>说明</td></tr><tr><td>0</td><td>2</td><td>对方1号英雄机器人血量，若该机器人未上场或者被罚下，则血量为0，下文同理</td></tr><tr><td>2</td><td>2</td><td>对方2号工程机器人血量</td></tr><tr><td>4</td><td>2</td><td>对方3号步兵机器人血量</td></tr><tr><td>6</td><td>2</td><td>对方4号步兵机器人血量</td></tr><tr><td>8</td><td>2</td><td>保留位</td></tr><tr><td>10</td><td>2</td><td>对方7号哨兵机器人血量</td></tr></table>

表 1-45 命令码 ID：0x0A03

<table><tr><td>字节偏移量</td><td>大小</td><td>说明</td></tr><tr><td>0</td><td>2</td><td>对方1号英雄机器人允许发弹量</td></tr><tr><td>2</td><td>2</td><td>对方3号步兵机器人允许发弹量（含堡垒提供的储备允许发弹量，下同）</td></tr><tr><td>4</td><td>2</td><td>对方4号步兵机器人允许发弹量</td></tr><tr><td>6</td><td>2</td><td>对方6号空中机器人允许发弹量</td></tr><tr><td>8</td><td>2</td><td>对方7号哨兵机器人允许发弹量</td></tr></table>
© 2026 大疆 版权所有 
43 
ROBOMASTER 


表 1-46 命令码 ID：0x0A04

<table><tr><td>字节偏移量</td><td>大小</td><td>说明</td></tr><tr><td>0</td><td>2</td><td>对方剩余金币数</td></tr><tr><td>2</td><td>2</td><td>对方累计总金币数</td></tr><tr><td>4</td><td>4</td><td>● bit 0: 对方补给区占领状态● bit 1-2: 对方中央高地的占领状态, 1 为被对方占领, 2 为被己方占领● bit 3: 对方梯形高地的占领状态, 1 为已占领● bit 4-5: 对方堡垒增益点的占领状态, 0 为未被占领, 1 为被对方占领, 2 为被己方占领, 3 为被双方占领● bit 6-7: 对方前哨站增益点的占领状态, 0 为未被占领, 1 为被对方占领, 2 为被己方占领● bit 8: 对方基地增益点的占领状态, 1 为已占领● bit 9: 靠近对方一侧飞坡前地形跨越增益点(隧道)中心处场地交互模块卡的状态, 1 为被对方占领● bit 10: 靠近对方一侧飞坡后地形跨越增益点(隧道)中心处场地交互模块卡的状态, 1 为被对方占领● bit 11: 靠近己方一侧飞坡前地形跨越增益点(隧道)中心处场地交互模块卡的状态, 1 为被对方占领● bit 12: 靠近己方一侧飞坡后地形跨越增益点(隧道)中心处场地交互模块卡的状态, 1 为被对方占领● bit 13: 对方地形跨越增益点(高地)上部场地交互模块卡的状态, 1 为被对方占领</td></tr><tr><td></td><td></td><td>● bit 14: 对方地形跨越增益点（飞坡）上部场地交互模块卡的状态，1为被对方占领
● bit 15: 对方地形跨越增益点（公路）上部场地交互模块卡的状态，1为被对方占领</td></tr></table>
44 
$\circledcirc$ 2026 大疆 版权所有 
ROBOMASTER 


表 1-47 命令码 ID：0x0A05

<table><tr><td>字节偏移量</td><td>大小</td><td>说明</td></tr><tr><td>0</td><td>1</td><td>对方英雄机器人回血增益(百分比,值为10表示每秒恢复血量上限的10%,下同)</td></tr><tr><td>1</td><td>2</td><td>对方英雄机器人射击热量冷却增益具体值(直接值,值为x表示热量冷却增加x/s,下同)</td></tr><tr><td>3</td><td>1</td><td>对方英雄机器人防御增益(百分比,值为50表示50%防御增益,下同)</td></tr><tr><td>4</td><td>1</td><td>对方英雄机器人负防御增益(百分比,值为30表示-30%防御增益,下同)</td></tr><tr><td>5</td><td>2</td><td>对方英雄机器人攻击增益(百分比,值为50表示50%攻击增益,下同)</td></tr><tr><td>7</td><td>1</td><td>对方工程机器人回血增益</td></tr><tr><td>8</td><td>2</td><td>对方工程机器人射击热量冷却增益具体值</td></tr><tr><td>10</td><td>1</td><td>对方工程机器人防御增益</td></tr><tr><td>11</td><td>1</td><td>对方工程机器人负防御增益</td></tr><tr><td>12</td><td>2</td><td>对方工程机器人攻击增益</td></tr><tr><td>14</td><td>1</td><td>对方3号步兵机器人回血增益</td></tr><tr><td>15</td><td>2</td><td>对方3号步兵机器人射击热量冷却增益具体值</td></tr><tr><td>17</td><td>1</td><td>对方3号步兵机器人防御增益</td></tr><tr><td>18</td><td>1</td><td>对方3号步兵机器人负防御增益</td></tr><tr><td>19</td><td>2</td><td>对方3号步兵机器人攻击增益</td></tr><tr><td>21</td><td>1</td><td>对方4号步兵机器人回血增益</td></tr><tr><td>22</td><td>2</td><td>对方4号步兵机器人射击热量冷却增益具体值</td></tr><tr><td>24</td><td>1</td><td>对方4号步兵机器人防御增益</td></tr><tr><td>25</td><td>1</td><td>对方4号步兵机器人负防御增益</td></tr><tr><td>26</td><td>2</td><td>对方4号步兵机器人攻击增益</td></tr><tr><td>28</td><td>1</td><td>对方哨兵机器人回血增益</td></tr><tr><td>29</td><td>2</td><td>对方哨兵机器人射击热量冷却增益具体值</td></tr><tr><td>31</td><td>1</td><td>对方哨兵机器人防御增益</td></tr><tr><td>32</td><td>1</td><td>对方哨兵机器人负防御增益</td></tr><tr><td>33</td><td>2</td><td>对方哨兵机器人攻击增益</td></tr><tr><td>35</td><td>1</td><td>对方哨兵机器人当前姿态</td></tr></table>
© 2026 大疆 版权所有 
45 
ROBOMASTER 


表 1-48 命令码 ID：0x0A06

<table><tr><td>字节偏移量</td><td>大小</td><td>说明</td></tr><tr><td>0</td><td>6</td><td>每个字节均为 ASCII 码编码的字母或数字。</td></tr></table>
46 
$\circledcirc$ 2026 大疆 版权所有 
ROBOMASTER 
# 2. 自定义客户端协议
自定义客户端的数据流格式为 Protobuf v3 （Protocol Buffers）, 采用 MQTT 作为发布/订阅消息传输 协议。MQTT直接传输Protobuf 序列化的二进制流。topic为对应的指令名。服务器IP 地址为固定IP： 192.168.12.1，端口：3333。自定义客户端 IP 地址为 DHCP 自动分配。 
# 通信流程如下：
1. 结合文档中的 Protobuf 消息格式编写 proto 文件，用 protoc 生成不同语言的类代码 
2. 在发布者端，将对象序列化成二进制 
3. 通过 MQTT publish 把二进制发送到对应 Topic 
4. 在订阅者端，接收消息并用 Protobuf 反序列化 
示例代码，详见“附录三：自定义客户端示例通信代码”，其中 MyMqttClient 的 clientID 需填入所连 接机器人对应的 ID 编号。 
此外，自定义客户端可通过 udp监听3334端口获取图传码流数据。编码格式为hevc，每个 udp包的码 流数据的前8个字节固定为： 
帧编号（递增）：2 byte 
当前帧内分片序号：2 byte 
当前帧总字节数：4 byte 
# 2.1 指令概览

表 2-1指令概览

<table><tr><td>指令(message)名</td><td>指令用途</td><td>发送方/接收方</td><td>最高qos等级</td><td>频率</td></tr><tr><td>KeyboardMouseControl</td><td>传输鼠标键盘输入</td><td>自定义客户端→图传链路→机器人</td><td>1</td><td>75Hz</td></tr><tr><td>CustomControl</td><td>最大30字节的自定义数据</td><td>自定义客户端→图传链路→机器人</td><td>1</td><td>75Hz</td></tr><tr><td>GameStatus</td><td>同步比赛全局状态信息</td><td>服务器→自定义客户端</td><td>1</td><td>5Hz</td></tr><tr><td>GlobalUnitStatus</td><td>同步基地、前哨站和所有机器人状态</td><td>服务器→自定义客户端</td><td>1</td><td>1Hz</td></tr><tr><td>GlobalLogisticsStatus</td><td>同步全局后勤信息</td><td>服务器→自定义客户端</td><td>1</td><td>1Hz</td></tr><tr><td>GlobalSpecialMechanism</td><td>同步正在生效的全局特殊机制</td><td>服务器→自定义客户端</td><td>1</td><td>1Hz</td></tr><tr><td>Event</td><td>全局事件通知</td><td>服务器→自定义客户端</td><td>1</td><td>触发式发送</td></tr><tr><td>RobotInjuryStat</td><td>机器人一次存活期间累计受伤统计</td><td>服务器→自定义客户端</td><td>1</td><td>1Hz</td></tr><tr><td>RobotRespawnStatus</td><td>机器人复活状态同步</td><td>服务器→自定义客户端</td><td>1</td><td>1Hz</td></tr><tr><td>RobotStaticStatus</td><td>机器人固定属性和配置</td><td>服务器→自定义客户端</td><td>1</td><td>1Hz</td></tr><tr><td>RobotDynamicStatus</td><td>机器人实时数据</td><td>服务器→自定义客户端</td><td>1</td><td>10Hz</td></tr><tr><td>RobotModuleStatus</td><td>机器人各模块运行状态</td><td>服务器→自定义客户端</td><td>1</td><td>1Hz</td></tr><tr><td>RobotPosition</td><td>机器人空间坐标和朝向</td><td>服务器→自定义客户端</td><td>1</td><td>1Hz</td></tr><tr><td>Buff</td><td>Buff效果信息</td><td>服务器→自定义客户端</td><td>1</td><td>获得增益时触发发送,此后1Hz定频发送直到失去增益</td></tr><tr><td>PenaltyInfo</td><td>判罚信息同步</td><td>服务器→自定义客户端</td><td>1</td><td>触发式发送</td></tr><tr><td>RobotPathPlanInfo</td><td>哨兵轨迹规划信息</td><td>服务器→自定义客户端</td><td>1</td><td>1Hz</td></tr><tr><td>MapClickInfoNotify</td><td>云台手地图点击标记</td><td>自定义客户端→服务器</td><td>1</td><td>触发式发送频率限制与1.3.1选手端下发数据小节一致</td></tr><tr><td>RadarInfoToClient</td><td>雷达发送的机器人位置信息</td><td>服务器→自定义客户端</td><td>1</td><td>1Hz</td></tr><tr><td>CustomByteBlock</td><td>机器人自定义上传数据流(机器人端对应0x0310)</td><td>机器人→图传链路→自定义客户端</td><td>1</td><td>50Hz</td></tr><tr><td>AssemblyCommand</td><td>工程装配指令</td><td>自定义客户端→服务器</td><td>1</td><td>1Hz</td></tr><tr><td>TechCoreMotionStateSync</td><td>科技核心运动状态同步</td><td>服务器→自定义客户端</td><td>1</td><td>1Hz</td></tr><tr><td>RobotPerformanceSelectionCommand</td><td>地面机器人选择性能体系或控制方式</td><td>自定义客户端→服务器</td><td>1</td><td>1Hz</td></tr><tr><td>RobotPerformanceSelectionSync</td><td>步兵/英雄性能体系状态同步</td><td>服务器→自定义客户端</td><td>1</td><td>1Hz</td></tr><tr><td>CommonCommand</td><td>机器人多种常用指令</td><td>自定义客户端→服务器</td><td>1</td><td>触发式发送,发送频率最高为10Hz</td></tr><tr><td>HeroDeployModeEventCommand</td><td>英雄部署模式相关指令</td><td>自定义客户端→服务器</td><td>1</td><td>1Hz</td></tr><tr><td>DeployModeStatusSync</td><td>英雄部署模式状态同步</td><td>服务器→自定义客户端</td><td>1</td><td>1Hz</td></tr><tr><td>RuneActivateCommand</td><td>能量机关激活指令</td><td>自定义客户端→服务器</td><td>1</td><td>1Hz</td></tr><tr><td>RuneStatusSync</td><td>能量机关状态同步</td><td>服务器→自定义客户端</td><td>1</td><td>1Hz</td></tr><tr><td>SentryStatusSync</td><td>哨兵姿态相关信息同步</td><td>服务器→自定义客户端</td><td>1</td><td>1Hz</td></tr><tr><td>DartCommand</td><td>飞镖控制指令</td><td>自定义客户端→服务器</td><td>1</td><td>1Hz</td></tr><tr><td>DartSelectTargetStatusSync</td><td>飞镖目标选择状态同步</td><td>服务器→自定义客户端</td><td>1</td><td>1Hz</td></tr><tr><td>SentryCtrlCommand</td><td>哨兵控制指令请求</td><td>自定义客户端→服务器</td><td>1</td><td>1Hz</td></tr><tr><td>SentryCtrlResult</td><td>哨兵控制指令结果反馈</td><td>服务器→自定义客户端</td><td>1</td><td>1Hz</td></tr><tr><td>AirSupportCommand</td><td>空中支援指令</td><td>自定义客户端→服务器</td><td>1</td><td>1Hz</td></tr><tr><td>AirSupportStatusSync</td><td>空中支援状态反馈</td><td>服务器→自定义客户端</td><td>1</td><td>1Hz</td></tr></table>
© 2026 大疆 版权所有 
47 
ROBOMASTER 

48 
$\circledcirc$ 2026 大疆 版权所有 
ROBOMASTER 

© 2026 大疆 版权所有 
49 
ROBOMASTER 

# 2.2 详细协议定义
# 2.2.1 KeyboardMouseControl
用途：传输鼠标键盘输入 
该数据将通过相机图传模块（接收端）发送给机器人。实际比赛时，操作手需提前选择键鼠控制指令来 源为官方选手端或自定义客户端，同时仅能通过一个来源生成发送给机器人的键鼠指令。 
<table><tr><td>数据编号</td><td>数据类型</td><td>数据用途</td></tr><tr><td>1</td><td>int32</td><td>鼠标x轴移动速度，负值标识向左移动</td></tr><tr><td>2</td><td>int32</td><td>鼠标y轴移动速度，负值标识向下移动</td></tr><tr><td>3</td><td>int32</td><td>鼠标滚轮移动速度，负值标识向后滚动</td></tr><tr><td>4</td><td>bool</td><td>左键是否按下（false=抬起, true=按下）</td></tr><tr><td>5</td><td>bool</td><td>右键是否按下（false=抬起, true=按下）</td></tr><tr><td>6</td><td>uint32</td><td>键盘按键位掩码</td></tr><tr><td>7</td><td>bool</td><td>中键是否按下（false=抬起, true=按下）</td></tr></table>
键盘按键位掩码：注释：数据类型虽为uint32，但实际仅需发送bit0到 bit15范围的数据即可。 
对应关系： 
每个 bit 对应一个按键，0 为未按下，1 为按下： 
bit 0：W 键 
bit 1：S 键 
bit 2：A 键 
bit 3：D 键 
bit 4:Shift 键 
50 
$\circledcirc$ 2026 大疆 版权所有 
ROBOMASTER 
bit 5：Ctrl 键 
bit 6：Q 键 
bit 7：E 键 
bit 8：R 键 
bit 9：F 键 
bit 10：G 键 
bit 11：Z 键 
bit 12：X 键 
bit 13：C 键 
bit 14：V 键 
bit 15：B 键 
message KeyboardMouseControl{ int32 mouse_x = 1; int32 mouse_y = 2; int32 mouse_z = 3; bool left_button_down $= 4$ . bool right_button_down $= 5$ uint32 keyboard_value $= 6$ bool mid_button_down $= 7$ .   
} 
# 2.2.2 CustomControl
用途：最大 30字节的自定义数据 
说明：该信息将通过常规链路以 0x0311命令字发送给机器人 
<table><tr><td>数据编号</td><td>数据类型</td><td>数据用途</td></tr><tr><td>1</td><td>bytes</td><td>最大 30 字节的自定义数据</td></tr></table>
```txt
messageCustomControl{ 
```
© 2026 大疆 版权所有 
51 
ROBOMASTER 
bytes data $= 0$ .   
} 
# 2.2.3 GameStatus
用途：同步比赛全局状态信息 
<table><tr><td>数据编号</td><td>数据类型</td><td>数据用途</td></tr><tr><td>1</td><td>uint32</td><td>当前局号（从1开始）</td></tr><tr><td>2</td><td>uint32</td><td>总局数</td></tr><tr><td>3</td><td>uint32</td><td>红方得分</td></tr><tr><td>4</td><td>uint32</td><td>蓝方得分</td></tr><tr><td>5</td><td>uint32</td><td>当前阶段</td></tr><tr><td>6</td><td>int32</td><td>当前阶段剩余时间（秒）</td></tr><tr><td>7</td><td>int32</td><td>当前阶段已过时间（秒）</td></tr><tr><td>8</td><td>bool</td><td>是否暂停</td></tr></table>
current_stage 枚举值： 
0：未开始比赛 
1：准备阶段 
2：十五秒裁判系统自检阶段 
3：五秒倒计时 
4：比赛中 
5：比赛结算中 
```txt
message GameStatus {
    uint32 current_round = 1;
    uint32 total_rounds = 2;
    uint32 red_score = 3;
    uint32 blue_score = 4; 
```
52 
$\circledcirc$ 2026 大疆 版权所有 
ROBOMASTER 
uint32 current stage $= 5$ int32 stage_countdown_sec $= 6$ int32 stage_elapsed(sec $= 7$ bool is_paused $= 8$ 
# 2.2.4 GlobalUnitStatus
用途：同步基地、前哨站和所有机器人状态 
<table><tr><td>数据编号</td><td>数据类型</td><td>数据用途</td></tr><tr><td>1</td><td>uint32</td><td>己方基地当前血量(0=已摧毁)</td></tr><tr><td>2</td><td>uint32</td><td>己方基地状态</td></tr><tr><td>3</td><td>uint32</td><td>己方基地当前护盾值(0=无护盾)</td></tr><tr><td>4</td><td>uint32</td><td>己方前哨站当前血量(0=已摧毁)</td></tr><tr><td>5</td><td>uint32</td><td>己方前哨站状态</td></tr><tr><td>6</td><td>uint32</td><td>对方基地当前血量(0=已摧毁)</td></tr><tr><td>7</td><td>uint32</td><td>对方基地状态</td></tr><tr><td>8</td><td>uint32</td><td>对方基地当前护盾值(0=无护盾)</td></tr><tr><td>9</td><td>uint32</td><td>对方前哨站当前血量(0=已摧毁)</td></tr><tr><td>10</td><td>uint32</td><td>对方前哨站状态</td></tr><tr><td>11</td><td>repeated uint32</td><td>所有机器人血量(先己方后对方)</td></tr><tr><td>12</td><td>repeated int32</td><td>己方机器人剩余累计发弹量</td></tr><tr><td>13</td><td>uint32</td><td>己方累计总伤害</td></tr><tr><td>14</td><td>uint32</td><td>对方累计总伤害</td></tr></table>
base_status 枚举值： 
0：无敌 
© 2026 大疆 版权所有 
53 
ROBOMASTER 
1：解除无敌，护甲未展开 
2：解除无敌，护甲展开 
outpost_status 枚举值： 
0：无敌 
1：存活，解除无敌，中部装甲旋转 
2：存活，解除无敌，中部装甲停转 
3：被击毁，不可重建 
4：被击毁，可重建 
5：被击毁，重建中 
```proto
message GlobalUnitStatus {
    uint32 base_health = 1;
    uint32 base_status = 2;
    uint32 base_shield = 3;
    uint32 outpost_health = 4;
    uint32 outpost_status = 5;
    uint32 enemy_base_health = 6;
    uint32 enemy_base_status = 7;
    uint32 enemy_base_shield = 8;
    uint32 enemy_outpost_health = 9;
    uint32 enemy_outpost_status = 10;
    repeated uint32 robot_health = 11;
    repeated int32 robot_bullets = 12;
    uint32 total_damage_ally = 13;
    uint32 total_damage_enemy = 14;
} 
```
# 2.2.5 GlobalLogisticsStatus
用途：同步全局后勤信息 
54 
$\circledcirc$ 2026 大疆 版权所有 
ROBOMASTER 
<table><tr><td>数据编号</td><td>数据类型</td><td>数据用途</td></tr><tr><td>1</td><td>uint32</td><td>己方当前经济</td></tr><tr><td>2</td><td>uint64</td><td>己方累计总经济</td></tr><tr><td>3</td><td>uint32</td><td>己方科技等级（工程机器人曾装配最高难度）</td></tr><tr><td>4</td><td>uint32</td><td>己方加密等级（雷达解析信息波机制）</td></tr></table>
```proto
message GlobalLogisticsStatus {
    uint32 remaining_economy = 1;
    uint64 total_economy Obtained = 2;
    uint32 tech_level = 3;
    uint32 encryption_level = 4;
} 
```
# 2.2.6 GlobalSpecialMechanism
用途：同步正在生效的全局特殊机制 
<table><tr><td>数据编号</td><td>数据类型</td><td>数据用途</td></tr><tr><td>1</td><td>repeated uint32</td><td>正在生效的机制 ID 列表</td></tr><tr><td>2</td><td>repeated int32</td><td>对应的时间参数（秒）</td></tr></table>
mechanism_id 枚举值： 
1：己方堡垒被对方占领计时 
2：对方堡垒被己方占领计时 
```txt
message GlobalSpecialMechanism {
    repeated uint32 mechanism_id = 1;
    repeated int32 mechanism_time(sec = 2); 
```
© 2026 大疆 版权所有 
55 
ROBOMASTER 
# 2.2.7 Event
用途：全局事件通知 
<table><tr><td>数据编号</td><td>数据类型</td><td>数据用途</td></tr><tr><td>1</td><td>int32</td><td>事件编号</td></tr><tr><td>2</td><td>string</td><td>事件参数（含义随 event_id 变化）</td></tr></table>
event_id 枚举值： 
1：击杀事件（参数为击杀者id+被击毁机器人id的拼接） 
 2：基地、前哨站被摧毁事件（参数值为被击毁的目标id，如蓝方前哨站为111） 
3：能量机关可激活次数变化（参数值为变化后可激活次数） 
4：能量机关当前可进入正在激活状态（无参数值） 
5：当前能量机关被成功激活的灯臂数量、平均环数（参数值为激活成功臂数 $+$ 平均环数的 
6：能量机关被激活（含激活类型） 
7：己方英雄进入部署模式（无参数值） 
8：己方英雄造成狙击伤害（参数值为累计造成狙击伤害数量） 
9：对方英雄造成狙击伤害（参数值为累计造成狙击伤害数量） 
10：己方呼叫空中支援（无参数值） 
11：己方空中支援被打断（参数值为对方还剩余的可打断次数） 
12：对方呼叫空中支援（无参数值） 
13：对方空中支援被打断（参数值为己方还剩余的可打断次数） 
 14：飞镖命中（参数值为命中目标，1 为击中前哨站，2 为击中基地固定目标，3 为击中基地随 机固定目标，4 为击中基地随机移动目标，5为击中基地末端移动目标） 
15：双方飞镖闸门开启（参数值1为己方开启，2为对方开启） 
16：己方基地遭到攻击（无参数值，每次触发存在5s内置冷却） 
17：双方前哨站停转（参数值1为己方前哨，2为对方前哨） 
18：双方基地护甲展开（参数值1为己方基地，2为对方基地） 
message Event { int32 event_id $= 1$ string param $= 2$ 
56 
$\circledcirc$ 2026 大疆 版权所有 
ROBOMASTER 
# 2.2.8 RobotInjuryStat
用途：机器人一次存活期间累计受伤统计 
<table><tr><td>数据编号</td><td>数据类型</td><td>数据用途</td></tr><tr><td>1</td><td>uint32</td><td>该次存活累计受伤总计</td></tr><tr><td>2</td><td>uint32</td><td>撞击伤害</td></tr><tr><td>3</td><td>uint32</td><td>17mm 弹丸伤害</td></tr><tr><td>4</td><td>uint32</td><td>42mm 弹丸伤害</td></tr><tr><td>5</td><td>uint32</td><td>飞镖溅射伤害</td></tr><tr><td>6</td><td>uint32</td><td>模块离线扣血</td></tr><tr><td>7</td><td>uint32</td><td>异常离线扣血</td></tr><tr><td>8</td><td>uint32</td><td>判罚扣血</td></tr><tr><td>9</td><td>uint32</td><td>服务器强制使其战亡扣血</td></tr><tr><td>10</td><td>uint32</td><td>击杀者 ID (若未检测到击杀者,值为 0)</td></tr><tr><td colspan="3">message RobotInjuryStat {
uint32 total_damage = 1;
uint32 collision_damage = 2;
uint32 small Projectile_damage = 3;
uint32 large Projectile_damage = 4;
uint32 dart_splash_damage = 5;
uint32 module_offline_damage = 6;
uint32 offline_damage = 7;
uint32 penalty_damage = 8;
uint32 server_kill_damage = 9;
uint32 killer_id = 10;
}</td></tr></table>
© 2026 大疆 版权所有 
57 
ROBOMASTER 
# 2.2.9 RobotRespawnStatus
用途：机器人复活状态同步 
<table><tr><td>数据编号</td><td>数据类型</td><td>数据用途</td></tr><tr><td>1</td><td>bool</td><td>是否处于待复活状态</td></tr><tr><td>2</td><td>uint32</td><td>复活所需总读条</td></tr><tr><td>3</td><td>uint32</td><td>当前复活读条进度</td></tr><tr><td>4</td><td>bool</td><td>是否可以免费复活</td></tr><tr><td>5</td><td>uint32</td><td>花费金币复活所需金币数</td></tr><tr><td>6</td><td>bool</td><td>是否允许花费金币复活</td></tr></table>
```proto
message RobotRespawnStatus {
    bool is_pending_respawn = 1;
    uint32 total_respawn_progress = 2;
    uint32 current_respawn_progress = 3;
    bool can_free_respawn = 4;
    uint32 gold_cost_for_respawn = 5;
    bool can_pay_for_respawn = 6;
} 
```
# 2.2.10 RobotStaticStatus
用途：机器人固定属性和配置 
<table><tr><td>数据编号</td><td>数据类型</td><td>数据用途</td></tr><tr><td>1</td><td>uint32</td><td>连接状态（0=未连接，1=已连接）</td></tr><tr><td>2</td><td>uint32</td><td>上场状态（0=已上场，1=未上场）</td></tr><tr><td>3</td><td>uint32</td><td>存活状态（0=未知，1=存活，2=战亡）</td></tr><tr><td>4</td><td>uint32</td><td>机器人编号</td></tr><tr><td>5</td><td>uint32</td><td>机器人类型</td></tr><tr><td>6</td><td>uint32</td><td>性能体系-发射机构</td></tr><tr><td>7</td><td>uint32</td><td>性能体系-底盘</td></tr><tr><td>8</td><td>uint32</td><td>当前等级</td></tr><tr><td>9</td><td>uint32</td><td>最大血量</td></tr><tr><td>10</td><td>uint32</td><td>最大热量</td></tr><tr><td>11</td><td>float</td><td>热量冷却速率（每秒）</td></tr><tr><td>12</td><td>uint32</td><td>最大功率</td></tr><tr><td>13</td><td>uint32</td><td>最大缓冲能量</td></tr><tr><td>14</td><td>uint32</td><td>最大底盘能量</td></tr></table>
58 
$\circledcirc$ 2026 大疆 版权所有 
ROBOMASTER 

performance_system_shooter 枚举值： 
1：冷却优先 
2：爆发优先 
3：英雄近战优先 
4：英雄远程优先 
performance_system_chassis 枚举值： 
1：血量优先 
2：功率优先 
3：英雄近战优先 
4：英雄远程优先 
```txt
message RobotStaticStatus {
    uint32 connection_state = 1;
    uint32 field_state = 2;
    uint32 alive_state = 3;
    uint32 robot_id = 4; 
```
© 2026 大疆 版权所有 
59 
ROBOMASTER 
```txt
uint32 robot_type = 5;  
uint32 performance_system_shooter = 6;  
uint32 performance_system_chassis = 7;  
uint32 level = 8;  
uint32 max_health = 9;  
uint32 max_heat = 10;  
float heat(cooldown_rate = 11;  
uint32 max_power = 12;  
uint32 max_buffer_energy = 13;  
uint32 max_chassis_energy = 14; 
```
# 2.2.11 RobotDynamicStatus
用途：机器人实时数据 
<table><tr><td>数据编号</td><td>数据类型</td><td>数据用途</td></tr><tr><td>1</td><td>uint32</td><td>当前血量</td></tr><tr><td>2</td><td>float</td><td>当前热量</td></tr><tr><td>3</td><td>float</td><td>上一次弹丸射速</td></tr><tr><td>4</td><td>uint32</td><td>当前剩余底盘能量</td></tr><tr><td>5</td><td>uint32</td><td>当前缓冲能量</td></tr><tr><td>6</td><td>uint32</td><td>当前经验值</td></tr><tr><td>7</td><td>uint32</td><td>距离下一次升级仍需获得的经验</td></tr><tr><td>8</td><td>uint32</td><td>累计已发弹量</td></tr><tr><td>9</td><td>uint32</td><td>剩余允许发弹量</td></tr><tr><td>10</td><td>bool</td><td>是否处于脱战状态</td></tr><tr><td>11</td><td>uint32</td><td>脱战状态倒计时</td></tr><tr><td>12</td><td>bool</td><td>是否可以远程补血</td></tr><tr><td>13</td><td>bool</td><td>是否可以远程补弹</td></tr></table>
60 
$\circledcirc$ 2026 大疆 版权所有 
ROBOMASTER 

```proto
message RobotDynamicStatus {
    uint32 current_health = 1;
    float current_heat = 2;
    float last_projectile-fire_rate = 3;
    uint32 current_chassis_energy = 4;
    uint32 current_buffer_energy = 5;
    uint32 current_experience = 6;
    uint32 experience_for_upgrade = 7;
    uint32 total_projectiles_fired = 8;
    uint32 remaining_ammo = 9;
    bool is_out_of_combat = 10;
    uint32 out_of_combat_countdown = 11;
    bool can_remote_heal = 12;
    bool can_remote_ammo = 13;
} 
```
# 2.2.12 RobotModuleStatus

用途：机器人各模块运行状态

<table><tr><td>数据编号</td><td>数据类型</td><td>数据用途</td></tr><tr><td>1</td><td>uint32</td><td>电源管理模块状态（0=离线，1=在线，2=因安装不规范被视为离线）</td></tr><tr><td>2</td><td>uint32</td><td>RFID模块状态（0=离线，1=在线，2=因安装不规范被视为离线）</td></tr><tr><td>3</td><td>uint32</td><td>灯条模块状态(0=离线,1=在线,2=因安装不规范被视为离线)</td></tr><tr><td>4</td><td>uint32</td><td>17mm发射机构状态(0=离线,1=在线,2=因安装不规范被视为离线)</td></tr><tr><td>5</td><td>uint32</td><td>42mm发射机构状态(0=离线,1=在线,2=因安装不规范被视为离线)</td></tr><tr><td>6</td><td>uint32</td><td>定位模块状态(0=离线,1=在线,2=因安装不规范被视为离线)</td></tr><tr><td>7</td><td>uint32</td><td>装甲模块状态(0=离线,1=在线,2=因安装不规范被视为离线)</td></tr><tr><td>8</td><td>uint32</td><td>图传模块状态(0=离线,1=在线,2=因安装不规范被视为离线)</td></tr><tr><td>9</td><td>uint32</td><td>电容模块状态(0=离线,1=在线,2=因安装不规范被视为离线)</td></tr><tr><td>10</td><td>uint32</td><td>主控状态(0=离线,1=在线,2=因安装不规范被视为离线)</td></tr><tr><td>11</td><td>uint32</td><td>激光检测模块状态(0=离线,1=在线,2=因安装不规范被视为离线)</td></tr></table>
© 2026 大疆 版权所有 
61 
ROBOMASTER 

```txt
message RobotModuleStatus {
    uint32 powermanager = 1;
    uint32 rfid = 2;
    uint32 light strip = 3;
    uint32 small_shooter = 4;
    uint32 big_shooter = 5;
    uint32 uwb = 6;
    uint32 armor = 7;
    uint32 video_transmission = 8;
    uint32 capacitor = 9; 
```
62 
$\circledcirc$ 2026 大疆 版权所有 
ROBOMASTER 
uint32 main_controller $= 10$ uint32 laserDetectionModule $= 11$ 
# 2.2.13 RobotPosition
用途：机器人空间坐标和朝向 
数据编号 数据类型 数据用途  
1 float 世界坐标X轴  
2 float 世界坐标Y轴  
3 float 世界坐标Z轴  
4 float 本机器人测速模块的朝向，单位：度，正北为0度  
message RobotPosition{float $\mathbf{x} = 1$ floaty $= 2$ floatz $= 3$ floatyaw $= 4$ } 
# 2.2.14 Buff
用途：Buff效果信息 
<table><tr><td>数据编号</td><td>数据类型</td><td>数据用途</td></tr><tr><td>1</td><td>uint32</td><td>机器人ID</td></tr><tr><td>2</td><td>uint32</td><td>Buff 类型</td></tr><tr><td>3</td><td>int32</td><td>Buff 增益值</td></tr><tr><td>4</td><td>uint32</td><td>Buff 最大剩余时间</td></tr></table>
© 2026 大疆 版权所有 
63 

ROBOMASTER

<table><tr><td>数据编号</td><td>数据类型</td><td>数据用途</td></tr><tr><td>5</td><td>uint32</td><td>Buff剩余时间</td></tr></table>
buff 类型枚举值： 
1：攻击增益 
2：防御增益 
3：射击热量冷却增益 
4：底盘功率增益 
5：回血增益 
6：可兑换允许发弹量 
 7：地形跨越增益（预备）（指诸如检测到飞坡前半部分的场地交互模块，但未获取实际地形跨越增 益（飞坡）的情况） 
message Buff{ uint32 robot_id $= 1$ . uint32 buff_type $= 2$ int32 buffer_level $= 3$ uint32buff_max_time $= 4$ uint32buffer_left_time $= 5$ 
# 2.2.15 PenaltyInfo

用途：判罚信息同步

<table><tr><td>数据编号</td><td>数据类型</td><td>数据用途</td></tr><tr><td>1</td><td>uint32</td><td>当前受罚类型</td></tr><tr><td>2</td><td>uint32</td><td>当前受罚效果时长（秒）</td></tr><tr><td>3</td><td>uint32</td><td>当前判罚数量</td></tr></table>
64 
$\circledcirc$ 2026 大疆 版权所有 
ROBOMASTER 
penalty_type 枚举值： 
1：黄牌 
2：双方黄牌 
3：红牌 
4：超功率 
5：超热量 
6：超射速 
```proto
message PenaltyInfo {
    uint32 penalty_type = 1;
    uint32 penalty_effects_sec = 2;
    uint32 total_penalty_num = 3;
} 
```
# 2.2.16 RobotPathPlanInfo
用途：哨兵轨迹规划信息 
<table><tr><td>数据编号</td><td>数据类型</td><td>数据用途</td></tr><tr><td>1</td><td>uint32</td><td>哨兵意图（1=攻击，2=防守，3=移动）</td></tr><tr><td>2</td><td>uint32</td><td>起始点X坐标（分米）</td></tr><tr><td>3</td><td>uint32</td><td>起始点Y坐标（分米）</td></tr><tr><td>4</td><td>repeated int32</td><td>相对起始点X增量数组（-128~+127，长度49）</td></tr><tr><td>5</td><td>repeated int32</td><td>相对起始点Y增量数组（-128~+127，长度49）</td></tr><tr><td>6</td><td>uint32</td><td>发送者ID</td></tr></table>
```txt
message RobotPathPlanInfo {
    uint32 intention = 1;
    uint32 start_pos_x = 2;
    uint32 start_pos_y = 3; 
```
© 2026 大疆 版权所有 
65 
ROBOMASTER 
```txt
repeated int32 offset_x = 4 [packed = true];  
repeated int32 offset_y = 5 [packed = true];  
uint32 sender_id = 6; 
```
# 2.2.17 MapClickInfoNotify
用途：云台手地图点击标记 
<table><tr><td>数据编号</td><td>数据类型</td><td>数据用途</td></tr><tr><td>1</td><td>uint32</td><td>发送范围(0=指定客户端,1=除哨兵,2=包含哨兵)</td></tr><tr><td>2</td><td>bytes</td><td>目标机器人ID列表(固定7字节,如发送给己方英雄和工程机器人,红方需要填入1,2;蓝方需要填入101,102,后续填0)</td></tr><tr><td>3</td><td>uint32</td><td>标记类型(1=攻击,2=防御,3=警戒,4=自定义)</td></tr><tr><td>4</td><td>uint32</td><td>标定的对方ID</td></tr><tr><td>5</td><td>uint32</td><td>自定义图标ASCII码</td></tr><tr><td>6</td><td>uint32</td><td>标记模式(1=地图,2=对方机器人)</td></tr><tr><td>7</td><td>uint32</td><td>屏幕坐标X(像素)</td></tr><tr><td>8</td><td>uint32</td><td>屏幕坐标Y(像素)</td></tr><tr><td>9</td><td>float</td><td>地图坐标X</td></tr><tr><td>10</td><td>float</td><td>地图坐标Y</td></tr></table>
```txt
message MapClickInfoNotify {
    uint32 is_send_all = 1;
    bytes robot_id = 2;
    uint32 mode = 3;
    uint32 enemy_id = 4;
    uint32ascii = 5; 
```
66 
$\circledcirc$ 2026 大疆 版权所有 
ROBOMASTER 
uint32 type $= 6$ .   
uint32 screen_x $= 7$ .   
uint32 screen_y $= 8$ float map_x $= 9$ float map_y $= 10$ 
# 2.2.18 RadarInfoToClient
用途：雷达发送的机器人位置信息 
<table><tr><td>数据编号</td><td>数据类型</td><td>数据用途</td></tr><tr><td>1</td><td>uint32</td><td>目标机器人ID</td></tr><tr><td>2</td><td>float</td><td>目标位置X（米）</td></tr><tr><td>3</td><td>float</td><td>目标位置Y（米）</td></tr><tr><td>4</td><td>float</td><td>朝向角度</td></tr><tr><td>5</td><td>uint32</td><td>是否特殊标识（0=否；1=是；2=是，但目标机器人此时定位模块为离线状态）</td></tr></table>
message RadarInfoToClient{ uint32 target_robot_id $= 1$ float target_pos_x $= 2$ float target_pos_y $= 3$ float toward_angle $= 4$ uint32 is_high_light $= 5$ } 
© 2026 大疆 版权所有 
67 
ROBOMASTER 
# 2.2.19 CustomByteBlock
用途：自定义数据流 
<table><tr><td>数据编号</td><td>数据类型</td><td>数据用途</td></tr><tr><td>1</td><td>bytes</td><td>最大为2.4kbit的自定义数据包</td></tr><tr><td colspan="3">message CustomByteBlock {
    bytes data = 1;
}</td></tr></table>
# 2.2.20 AssemblyCommand
用途：工程装配指令 
<table><tr><td>数据编号</td><td>数据类型</td><td>数据用途</td></tr><tr><td>1</td><td>uint32</td><td>装配操作类型（1=确认装配，2=取消装配）</td></tr><tr><td>2</td><td>uint32</td><td>选中的装配难度</td></tr><tr><td colspan="3">message AssemblyCommand {
    uint32 operation = 1;
    uint32 difficulty = 2;
}</td></tr></table>
# 2.2.21 TechCoreMotionStateSync
用途：科技核心运动状态同步 
<table><tr><td>数据编号</td><td>数据类型</td><td>数据用途</td></tr><tr><td>1</td><td>uint32</td><td>当前可选择的最高装配难度等级</td></tr><tr><td>2</td><td>uint32</td><td>科技核心状态</td></tr><tr><td>3</td><td>uint32</td><td>对方科技核心状态（仅己方四级装配可用时有效，0为对方没有正在装配，1为对方正在装配非四级难度，2为对方正在装配四级难度）</td></tr><tr><td>4</td><td>uint32</td><td>己方装配总剩余时长（仅己方正在四级装配时有效）</td></tr><tr><td>5</td><td>uint32</td><td>己方单个步骤装配间隔剩余时长（仅己方正在四级装配时有效）</td></tr></table>
68 
$\circledcirc$ 2026 大疆 版权所有 
ROBOMASTER 

status 枚举值： 
1：未进入装配状态 
2：已选择装配难度，科技核心移动中 
3：科技核心移动完成，可进行首个装配步骤 
4：上一个装配步骤已完成，可进行下一个装配步骤 
5：装配步骤已全部完成 
6：已确认装配，科技核心移动中 
```proto
message TechCoreMotionStateSync {
    uint32 maximum_difficulty_level = 1;
    uint32 status = 2;
    uint32 enemy_core_status = 3;
    uint32 remain_time_all = 4;
    uint32 remain_time_step = 5;
} 
```
# 2.2.22 RobotPerformanceSelectionCommand
用途：地面机器人选择性能体系或控制方式 
<table><tr><td>数据编号</td><td>数据类型</td><td>数据用途</td></tr><tr><td>1</td><td>uint32</td><td>发射机构性能体系</td></tr><tr><td>2</td><td>uint32</td><td>底盘性能体系</td></tr><tr><td>3</td><td>uint32</td><td>哨兵控制方式选择</td></tr></table>
© 2026 大疆 版权所有 
69 
ROBOMASTER 

性能体系枚举值可参考 RobotStaticStatus 中描述 
sentry_control 枚举值： 
$0 =$ 哨兵自动控制 
$1 =$ 哨兵半自动控制 
```proto
message RobotPerformanceSelectionCommand {
    uint32 shooter = 1;
    uint32 chassisis = 2;
    unit32 sentry_control = 3;
} 
```
# 2.2.23 RobotPerformanceSelectionSync
用途：步兵/英雄性能体系状态同步 
<table><tr><td>数据编号</td><td>数据类型</td><td>数据用途</td></tr><tr><td>1</td><td>uint32</td><td>发射机构性能体系</td></tr><tr><td>2</td><td>uint32</td><td>底盘性能体系</td></tr><tr><td>3</td><td>uint32</td><td>哨兵控制方式选择</td></tr></table>
```proto
message RobotPerformanceSelectionSync {
    uint32 shooter = 1;
    uint32 chassisis = 2;
    unit32 sentry_control = 3;
} 
```
# 2.2.24 CommonCommand
用途：机器人多种常用指令 
70 
$\circledcirc$ 2026 大疆 版权所有 
ROBOMASTER 
<table><tr><td>数据编号</td><td>数据类型</td><td>数据用途</td></tr><tr><td>1</td><td>uint32</td><td>命令类型</td></tr><tr><td>2</td><td>uint32</td><td>命令类型对应的参数</td></tr></table>
cmd_type 枚举值： 
 1：兑换 $1 7 \mathrm { m m }$ 发弹量（仅剩余金币足以兑换时有效；该命令类型对应的参数值必须为10的倍数） 
2：兑换 $4 2 \mathrm { m m }$ 发弹量（仅剩余金币足以兑换时有效） 
3：确认复活（若此时复活读条完成将立即复活） 
 4：兑换立即复活（若此时符合兑换立即复活的规则要求，则会立即消耗金币兑换立即复活） 
5：远程兑换允许发弹量（若此时符合规则要求，则会立即消耗金币远程兑换允许发弹量） 
6：远程兑换血量（若此时符合规则要求，则会立即消耗金币远程兑换血量） 
message CommonCommand{ uint32 cmd_type $= 1$ uint32 param $= 2$ 
# 2.2.25 HeroDeployModeEventCommand
用途：英雄部署模式指令 
<table><tr><td>数据编号</td><td>数据类型</td><td>数据用途</td></tr><tr><td>1</td><td>uint32</td><td>模式 (0=退出, 1=进入)</td></tr><tr><td colspan="3">message HeroDeployModeEventCommand {
    uint32 mode = 1;
}</td></tr></table>
# 2.2.26 DeployModeStatusSync
用途：英雄部署模式状态同步 
© 2026 大疆 版权所有 
71 
ROBOMASTER 
<table><tr><td>数据编号</td><td>数据类型</td><td>数据用途</td></tr><tr><td>1</td><td>uint32</td><td>当前部署模式状态（0为未部署，1为已部署）</td></tr></table>
message DeployModeStatusSync{ uint32 status $= 1$ .   
} 
# 2.2.27 RuneActivateCommand
用途：能量机关激活指令 
<table><tr><td>数据编号</td><td>数据类型</td><td>数据用途</td></tr><tr><td>1</td><td>uint32</td><td>激活（1=开启）</td></tr><tr><td colspan="3">message RuneActivateCommand {
    uint32 activate = 1;
}</td></tr></table>
# 2.2.28 RuneStatusSync
用途：能量机关状态同步 
<table><tr><td>数据编号</td><td>数据类型</td><td>数据用途</td></tr><tr><td>1</td><td>uint32</td><td>当前能量机关状态枚举</td></tr><tr><td>2</td><td>uint32</td><td>当前已激活的灯臂数量</td></tr><tr><td>3</td><td>uint32</td><td>总环数</td></tr></table>
rune_status 枚举值： 
1：未激活 
2：正在激活 
3：已激活 
```txt
message RuneStatusSync { 
```
72 
$\circledcirc$ 2026 大疆 版权所有 
ROBOMASTER 
```txt
uint32 Rune_status = 1;  
uint32 activated Arms = 2;  
uint32 average_rings = 3; 
```
# 2.2.29 SentryStatusSync
用途：哨兵姿态和弱化状态 
数据编号 数据类型 数据用途  
1 uint32 姿态ID（1为进攻姿态，2为防御姿态，3为移动姿态）  
2 bool 是否弱化  
message SentryStatusSync{uint32posture_id $= 1$ boolis_WEakened $= 2$ ·} 
# 2.2.30 DartCommand
用途：飞镖控制指令 
```txt
数据编号 数据类型 数据用途  
1 uint32 目标ID（1为前哨站，2为基地固定目标，3为基地随机固定目标，4为基地随机移动目标，5为基地末端移动目标）  
2 bool 闸门开关  
3 bool 是否确认发射（默认为0，1为确认发射） 
```
```txt
message DartCommand {
    uint32 target_id = 1;
    bool open = 2; 
```
© 2026 大疆 版权所有 
73 
ROBOMASTER 
bool launch_confirm $= 3$ 1 
# 2.2.31 DartSelectTargetStatusSync
用途：飞镖目标选择状态同步 
<table><tr><td>数据编号</td><td>数据类型</td><td>数据用途</td></tr><tr><td>1</td><td>uint32</td><td>目标 ID</td></tr><tr><td>2</td><td>uint32</td><td>闸门状态（0：关闭，1：开启中，2：已开启）</td></tr></table>
```proto
message DartSelectTargetStatusSync {
    uint32 target_id = 1;
    uint32 open = 2;
} 
```
# 2.2.32 SentryCtrlCommand
用途：哨兵控制指令请求 
<table><tr><td>数据编号</td><td>数据类型</td><td>数据用途</td></tr><tr><td>1</td><td>uint32</td><td>指令编号（0=无效）</td></tr></table>
指令编号枚举： 
1: 补血点补弹 
2: 补给站实体补弹 
3: 远程补弹 
4:远程回血 
5: 确认复活 
6：确认花费金币复活 
7: 地图标点 
8：切换为进攻姿态 
9: 切换为防御姿态 
74 
$\circledcirc$ 2026 大疆 版权所有 
ROBOMASTER 
10：切换为移动姿态 
```proto
message SentryCtrlCommand { uint32 command_id = 1; } 
```
# 2.2.33 SentryCtrlResult
用途：哨兵控制指令结果反馈 
<table><tr><td>数据编号</td><td>数据类型</td><td>数据用途</td></tr><tr><td>1</td><td>uint32</td><td>对应的指令编号</td></tr><tr><td>2</td><td>uint32</td><td>执行结果码</td></tr></table>
result_code 枚举值： 
0：成功 
其他：失败 
```proto
message SentryCtrlResult {
    uint32 command_id = 1;
    uint32 result_code = 2;
} 
```
# 2.2.34 AirSupportCommand
用途：空中支援指令 
<table><tr><td>数据编号</td><td>数据类型</td><td>数据用途</td></tr><tr><td>1</td><td>uint32</td><td>指令类型</td></tr></table>
command_id 枚举值： 
1：免费呼叫空中支援 
© 2026 大疆 版权所有 
ROBOMASTER 
2：花费金币呼叫空中支援（仍优先使用免费时长） 
3：中断空中支援 
message AirSupportCommand{ uint32 command_id $= 1$ .   
} 
# 2.2.35 AirSupportStatusSync

用途：空中支援状态反馈

<table><tr><td>数据编号</td><td>数据类型</td><td>数据用途</td></tr><tr><td>1</td><td>uint32</td><td>空中支援状态</td></tr><tr><td>2</td><td>uint32</td><td>免费空中支援剩余时间</td></tr><tr><td>3</td><td>uint32</td><td>付费空中支援已花费金币</td></tr><tr><td>4</td><td>uint32</td><td>当前激光检测模块是否正在检测到被照射（0为未被照射，1为被照射）</td></tr><tr><td>5</td><td>uint32</td><td>空中机器人被反制状态</td></tr></table>
airsupport_status 枚举值： 
0：未进行空中支援 
1：正在空中支援 
shooter_status 枚举值: 
0：发射机构因雷达反制而被锁定 
1：发射机构正常，未被反制 
```txt
message AirSupportStatusSync {
    uint32 airesupport_status = 1;
    uint32 left_time = 2;
    uint32 cost Coins = 3; 
```
76 
$\circledcirc$ 2026 大疆 版权所有 
ROBOMASTER 
```txt
uint32 is_being_targeted = 4;  
uint32 shooter_status = 5; 
```
© 2026 大疆 版权所有 
77 
ROBOMASTER 
# 附录一：CRC 校验代码示例
//crc8 generator polynomial:G(x)=x8+x5+x4+1   
const unsigned char CRC8_INIT = 0xff;   
const unsigned char CRC8_TAB[256] =   
{   
0x00, 0x5e, 0xcc, 0xe2, 0x61, 0x3f, 0xDD, 0x83, 0xc2, 0x9c, 0x7e, 0x20, 0xa3, 0xffd, 0x1f, 0x41,   
0x9d, 0xc3, 0x21, 0x7f, 0xfc, 0xa2, 0x40, 0x1e, 0x5f, 0x01, 0xe3, 0xbd, 0x3e, 0x60, 0x82, 0xdc,   
0x23, 0x7d, 0x9f, 0xc1, 0x42, 0xc1c, 0xFE, 0xa0, 0xe1, 0xbf, 0x5d, 0x03, 0x80, 0xde, 0x3c, 0x62,   
0xae, 0xe0, 0x02, 0x5c, 0xdbf, 0x81, 0x63, 0x3d, 0x7c, 0x22, 0xc0, 0x9e, 0xd1d, 0x43, 0xa1, 0xfff,   
0x46, 0x18, 0xaa, 0xa4, 0x27, 0x79, 0xb5, 0xc5, 0x84, 0xda, 0x38, 0xe5, 0xbbb, 0x59, 0x07,   
0xdbb, 0x85, 0x67, 0x39, 0xa,b, 0xe4, 0x66, 0x58, 0x19, 0x47, 0xa5, 0xdbb, 0x78, 0x26, 0xc4, 0x9a,   
0x65, 0x3b, 0xd9, 0x87, 0x04, 0xa5a, 0xb8, 0xe6, 0xa7, 0xf9, 0xb1b, 0x45, 0xc6, 0x98, 0xa7a, 0x24,   
0xf8, 0xa6, 0x44, 0xa1a, 0x99, 0xc7, 0x25, 0xb7b, 0xa3a, 0x64b, 0xd8b, 0xb5b, 0xe7b6b99   
ox8c, oxd2, ox3o3o3o6e oxed oxb3 oxd3 oxb5 oxb4 oxb6 oxb7 oxb8 oxb9   
ox11. ox4f. oxad. oxf3. ox7o. ox2e. oxcc. ox92. ox3d3. ox8d. ox6f. ox31. xxb2. xec. oxo e. ox5o   
oxaf. oxf1. oxl3. oxl4d. oxcce. o9o. o72. oxtc. o6d. oxb3. oxb1. oxbf. oxtc. oxtc2. oxbb. oee   
ox32. ox6c. o8e. oxdo. oxb5. oxbd. oxfef. xfb1. xfbf. xfae. x4c. x12. x91. xcf. x2d. x73   
oxca. ox94. ox76. ox28. oxab. xbf5. ox17. ox49. ox8o. ox56. xxb4. xxea. ox69. ox37. xxd5. x8b b   
ox57. oxo9. oxbv. oxb5. oxb36. oxb68. oxb4. oxb4. oxbc. o29. o77. xof4. xaaa. o48. o16   
oxe9. xob7. ox55. oxobv. oxb8. oxb6. oxb4a. oxb75. oxb97. xoc9. x4a. o14. xof6. xaa8   
ox74,o2a,Oxc8,O96,Oxl5,Oxb4b,Oxa9,Oxf7,Oxb6,Oxe8,Oxoa,O54,Oxd7,O89,Oxb6b,O35   
};   
unsigned char Get_CRC8_Check_Sum(unsigned char *pchMessage,unsigned int dwLength,unsigned char ucCRC8)   
{   
unsigned char ucIndex;   
while (dwLength--)   
{   
ucIndex $\equiv$ ucCRC8^(*pchMessage++);   
ucCRC8 $=$ CRC8_TAB[ucIndex]; 
78 
$\circledcirc$ 2026 大疆 版权所有 
ROBOMASTER 
}   
return(ucCRC8);   
}   
/\*   
\*\* Descriptions: CRC8 Verify function   
\*\* Input: Data to Verify,Stream length $=$ Data $^+$ checksum   
\*\* Output: True or False (CRC Verify Result)   
\*/   
unsigned int Verify_CRC8_Check_Sum(unsigned char \*pchMessage, unsigned int dwLength)   
{   
unsigned char ucExpected $= 0$ .   
if ((pchMessage $= = 0$ ) || (dwLength <= 2)) return 0;   
ucExpected $=$ Get_CRC8_Check_Sum(pchMessage, dwLength-1, CRC8_INIT);   
return ( ucExpected $= =$ pchMessage[dwLength-1]);   
}   
/\*   
\*\* Descriptions: append CRC8 to the end of data   
\*\* Input: Data to CRC and append,Stream length $=$ Data $^+$ checksum   
\*\* Output: True or False (CRC Verify Result)   
\*/   
void Append_CRC8_Check_Sum(unsigned char \*pchMessage, unsigned int dwLength)   
{   
unsigned char ucCRC $= 0$ .   
if ((pchMessage $= = 0$ ) || (dwLength <= 2)) return;   
ucCRC $=$ Get_CRC8_Check_Sum(( unsigned char \*)pchMessage, dwLength-1, CRC8_INIT);   
pchMessage[dwLength-1] $=$ ucCRC; 
© 2026 大疆 版权所有 
79 
ROBOMASTER 
```csv
uint16_t CRC_INIT = 0xffff;  
const uint16_t wCRC_Table[256] =  
{  
0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf, 0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xea6c, 0xdbe5, 0xe97e, 0xf8f7, 0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e, 0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdbed, 0xc64, 0xf9ff, 0xe876, 0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd, 0xd4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xffae7, 0xc87c, 0xd9f5, 0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c, 0xbdbc, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974, 0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb, 0xcc4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3, 0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a, 0xdcd, 0xfc44, 0xfddbf, 0ecx56, 0x98e9, 0x8960, 0xbbfbf, 0xa72, 0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9, 0xEF4e, 0xfec7, 0xcc5c, 0xDD5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1, 0x7387, 0x62o e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738, 0xffcf, 0xee46, 0xdcdbd, 0xCD54, 0xb9eb, 0xa862, 0x9af9, 0x8b7o, 0x84o8, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xfob7, 0x84o4o, 0x19c9, 0x2b52, 0x3adbh, 0x4e64, 0x5fed, 0xD76o x7cff.  
OX9489,OX85Oo,OXB79b,Oxa612,Oxd2ad,Oxc324,Oxf1bf,OxeO36.  
OX18c1,OXo948,OXB3bd3,Oxa2a5a,OXSee5,Oxf4f6c,O7df7,OXC67e.  
Oxa5o a,Oxb483,OXB618,OX9791,OXE32e,Oxf2a7,OxcO3c,Oxd1b5.  
OX2942,OXB38cb,Oxa5o,OXB1bd9,Oxbf66,O7eef,Oxbc74,Oxb5fdf.  
Oxb58b,Oxa4o2,Oxb4o2,Oxb4o2,Oxb4o2,Oxb4o2,Oxb4o2,Oxb4o2,Oxb4o2,Oxb4o2,Oxb4o2,Oxb4o2,Oxb4o2,Oxb4o2,Oxb4o2,Oxb4o2,Oxb4o2,Oxb4o2,Oxb4o2,Oxb4o2,Oxb4o2,Oxb4o2,O xb58b,Oxa4o2,Oxb4o2,Oxb4o2,Oxb4o2,Oxb4o2,Oxb4o2,Oxb4o2,Oxb4o2,Oxb4o2,Oxb4o2,Oxb4o2,Oxb4o2,Oxb4o2,Oxb4o2,Oxb4o2,Oxb4o2,Oxb4o2,Oxb4o2,Oxb4o2,O xb58b,O xa4o2,Oxb4o2,Oxb4o2,Oxb4o2,Oxb4o2,Oxb4o2,Oxb4o2,Oxb4o2,Oxb4o2,Oxb4o2,Oxb4o2,Oxb4o2,Oxb4o2,Oxb4o2,Oxb4o2,Oxb4o2,Oxb4o2,Oxb4o2,Oxb4o2,Oxb4o2,Oxtalb5. 
```
80 
$\circledcirc$ 2026 大疆 版权所有 
ROBOMASTER 
```c
0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,   
0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,   
0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,   
0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,   
0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,   
0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,   
0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78   
};   
/\*   
\*\* Descriptions: CRC16 checksum function   
\*\* Input: Data to check,Stream length, initialized checksum   
\*\* Output: CRC checksum   
\*/   
uint16_t Get_CRC16_Check_Sum( uint8_t *pchMessage,int32_t dwLength,int16_t wCRC)   
{   
Uint8_t chData;   
if (pchMessage == NULL)   
{   
return 0xFFFF;   
}   
while(dwLength--)   
{   
chData = \*pchMessage++;   
(wCRC) = ((uint16_t)(wCRC) >> 8) ^ {wCRC}_Table[((uint16_t)(wCRC) ^ {(\texttt{uint16_t})(\texttt{chData}))} &   
0x00ff];   
}   
return wCRC; 
```
© 2026 大疆 版权所有 
81 
ROBOMASTER 
/\*   
\*\* Descriptions: CRC16 Verify function   
\*\* Input: Data to Verify,Stream length $=$ Data $^+$ checksum   
\*\* Output: True or False (CRC Verify Result)   
\*/   
uint32_t Verify_CRC16_Check_Sum uint8_t \*pchMessage, uint32_t dwLength)   
{   
uint16_t wExpected $= 0$ .   
if ((pchMessage $= =$ NULL) || (dwLength <= 2))   
{   
return \_\_FALSE;   
}   
wExpected $=$ Get_CRC16_Check_Sum ( pchMessage, dwLength - 2, CRC_INIT);   
return ((wExpected & 0xff) $= =$ pchMessage[dwLength - 2] && ((wExpected >> 8) & 0xff) $= =$ pchMessage[dwLength - 1]);   
}   
/\*   
\*\* Descriptions: append CRC16 to the end of data   
\*\* Input: Data to CRC and append,Stream length $=$ Data $^+$ checksum   
\*\* Output: True or False (CRC Verify Result)   
\*/   
void Append_CRC16_Check_Sum uint8_t \* pchMessage, uint32_t dwLength)   
{   
uint16_t wCRC $= 0$ .   
if ((pchMessage $= =$ NULL) || (dwLength <= 2))   
{   
return; 
82 
$\circledcirc$ 2026 大疆 版权所有 
ROBOMASTER 
```c
}   
wCRC = Get_CRC16_Check_Sum ( (U8 *)pchMessage, dwLength-2, CRC_INIT );   
pchMessage[dwLength-2] = (U8)(wCRC & 0x00ff);   
pchMessage[dwLength-1] = (U8)((wCRC >> 8)& 0x00ff); 
```
© 2026 大疆 版权所有 
83 
ROBOMASTER 
# 附录二：ID 编号说明
机器人 ID编号如下所示： 
1：红方英雄机器人 
2：红方工程机器人 
3/4/5：红方步兵机器人（与机器人 ID $3 { \sim } 5$ 对应） 
6：红方空中机器人 
7：红方哨兵机器人 
8：红方飞镖 
9：红方雷达 
10：红方前哨站 
11：红方基地 
101：蓝方英雄机器人 
102：蓝方工程机器人 
103/104/105：蓝方步兵机器人（与机器人 ID 3~5 对应） 
106：蓝方空中机器人 
107：蓝方哨兵机器人 
108：蓝方飞镖 
109：蓝方雷达 
110：蓝方前哨站 
111：蓝方基地 
选手端 ID如下所示： 
0x0101：红方英雄机器人选手端 
0x0102：红方工程机器人选手端 
0x0103/0x0104/0x0105：红方步兵机器人选手端（与机器人 ID $3 { \sim } 5$ 对应） 
0x0106：红方空中机器人选手端 
0x016A：蓝方空中机器人选手端 
0x0165：蓝方英雄机器人选手端 
84 
$\circledcirc$ 2026 大疆 版权所有 
ROBOMASTER 
0x0166：蓝方工程机器人选手端 
0x0167/0x0168/0x0169：蓝方步兵机器人选手端（与机器人 ID 3~5 对应） 
0x8080：裁判系统服务器（用于哨兵和雷达自主决策指令） 
© 2026 大疆 版权所有 
85 
ROBOMASTER 
# 附录三：自定义客户端示例通信代码
using MQTTnet;   
using MQTTnet(Client;   
using MQTTnet.Diagnostics;   
using MQTTnet.Proteol;   
using MQTTnet.Server;   
using System;   
using System.Collections;   
using System.Collections.Generic;   
using System.Text;   
using System.Threading.Tasks;   
using UnityEngine;   
/// <summary>   
/// MQTT客户端   
/// </summary>   
public class MyMqttClient { private IMqttClient m_MqttClient = null; private string m客户的ID; /// <summary>   
/// 创建客户端并连接服务器   
/// </summary>   
/// <param name $=$ "clientID></param>   
/// <param name $=$ "ip></param>   
/// <param name $=$ "port></param>   
public MyMQttClient(string clientID, string ip $=$ "127.0.0.1", int port $= 3333$ ）{ m(ClientID $\equiv$ clientID; //客户端选项生成器 var options $\equiv$ new MqttOptionsBuilder() .WithClientId(m(ClientID) .WithTcpServer(ip, port) 
86 
$\circledcirc$ 2026 大疆 版权所有 
ROBOMASTER 
```cs
. Build(); //创建客户端 m_MqttClient = new MqttFactory().CreateMqttClient(); //监测客户端 连接/断开连接 完成 m_MqttClient ConnectedAsync += ClientConnected; m_M MQTTClient.DisconnectedAsync += ClientDisConnected; //客户端接收到消息 m_M MQTTClient.ApplicationMessageReceivedAsync += ReceiveMsg; //连接服务器 m_M MQTTClient.ConnectAsync(options); } // <summary> // 接收到消息 </summary> // <param name="args"></param> // <returns></returns> private Task ReceiveMsg(MqttApplicationMessageReceivedEventArgs args) { Debugger.Log(DebugLogEnum.CustomClient, "\(recv topic: {args.ApplicationMessage.Topic}"); //消息 post 出现，以免出现消息堵塞 MqttSubProtoEvent ev = new MqttSubProtoEvent(); ev.topic = args.ApplicationMessage.Topic; ev.data = args.ApplicationMessage.Payload; ThreadPool.CustomClientLoop.PostEvent(ev); return TaskCompletedTask; } // <summary> // 断开连接完成 </summary> // <param name="args"></param> // <returns></returns> private Task ClientDisconnected(MqttClientDisconnectedEventArgs args) 
```
$\circledcirc$ 2026 大疆 版权所有 
87 
ROBOMASTER 
{ return Task_COMPLETEDTask; } // <summary> // 连接完成 // </summary> // <param name $=$ "args></param> // <returns></returns>ReceiveMsg private Task ClientConnected(MqttClientConnectedEventArgs args) { Subscribe("KeyboardMouseControl"); Subscribe("CustomControl"); return Task_COMPLETEDTask; } // <summary> // 发布消息 // </summary> public void PublishMsg(string topic, string message, MqttQualityOf服务水平 level = MqttQualityOf服务水平 ExactlyOnce, bool isRetain $=$ false) { long UNIXs $\equiv$ DateTime.UtcNow.Ticks / 10; // 微秒级 m_MqttClient.PublishStringAsynctopic, message, level, isRetain); } // <summary> // 发布消息 // </summary> public void PublishBytesMsg(string topic, byte[] message, MqttQualityOf服务水平 level $=$ MqttQualityOf服务水平 ExactlyOnce, bool isRetain $=$ false) { long UNIXs $\equiv$ DateTime.UtcNow.Ticks / 10; // 微秒级 m_MqttClient.PublishBinaryAsynctopic, message, level, isRetain); } 
88 
$\circledcirc$ 2026 大疆 版权所有 
ROBOMASTER 
```cs
// <summary>
// 订阅主题
// </summary>
public void Subscribe(string topic)
{
    m_MqttClient.SubSCRIBEAsync(new MqttFilterBuilder().WithTopictopic).Build());
} 
```
© 2026 大疆 版权所有 
89 
![image](https://cdn-mineru.openxlab.org.cn/result/2026-02-28/a72e60af-c3aa-4d9e-8e93-7eb7c3ff3a17/73f687717bde318450959568dc7d8546988a2448c935c763f2e036f4eb050e6a.jpg)

邮箱：robomaster@dj.com 
论坛：https://bbs.robomaster.com 
官网：https://www.robomaster.com 
电话：+860755-84357613（周一至周五10:30-19:30） 
地址：广东省深圳市南山区西丽街道仙茶路与兴科路交叉口大疆天空之城T222F 