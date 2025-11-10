# 工创项目业务逻辑文档

## 项目概述
本项目是一个智能工创机器人系统，主要实现基于二维码识别的自动物料搬运、分类和码垛功能。机器人通过视觉系统识别物料，使用机械臂进行抓取和放置操作，并在不同工作区域间完成物料转运。

## 核心业务逻辑

### 1. 系统工作区域定义

```
工作区域布局：
┌─────────────┐
│   转盘区     │ ← 物料初始存放区
├─────────────┤
│   Area 0    │ ← 粗加工区/原料缓冲区
├─────────────┤
│   Area 1    │ ← 半成品区/码垛区
└─────────────┘
```

### 2. 主要业务流程

#### 2.1 系统初始化流程
```c
// main.c:70-82
BSP_Init();           // 硬件外设初始化
PWM_Init();           // PWM输出初始化
NVIC_Configuration(); // 中断配置
Key_Init();          // 按键初始化
Elevator_Height_Init(); // 升降机高度初始化
Move_group(1,200);   // 机械臂初始位置
```

#### 2.2 完整作业流程 (KeyNum==1)

**第一轮作业：**
1. **定位移动**
   ```c
   Di_Pan_Go('L',0.75,3);  // 向左移动0.75圈
   Di_Pan_Go('F',2.05,3);  // 向前移动2.05圈
   ```

2. **二维码识别**
   ```c
   // 串口接收视觉系统数据
   // QR_code_buffer1 = 三位数，分别代表红绿蓝物料编号
   ```

3. **精确调整**
   ```c
   Di_Pan_Go('F',3.83,3);     // 前进到工作位置
   Move_group(3, 100);        // 机械臂朝外
   Elevator_Move_Plate();     // 升降机到平台高度
   Di_Pan_Go('R',0.275,3);    // 微调位置
   ```

4. **转盘物料抓取**
   ```c
   As_QR_code_Catch_Body_TurnTable1(QR_code_buffer1);
   // 按二维码顺序抓取三种颜色物料到车上
   ```

5. **转运处理**
   ```c
   Turntable_To_Area0();                    // 转盘→粗加工区
   As_QR_code_Put_To_Area(QR_code_buffer1,0); // 放置到粗加工区
   As_QR_code_Put_To_Car(QR_code_buffer1);   // 装载到车
   Area0_To_Area1(QR_code_buffer1,0);        // 粗加工区→半成品区
   As_QR_code_Put_To_Area(QR_code_buffer1,0); // 放置到半成品区
   Area1_To_Turntable(QR_code_buffer1);       // 返回转盘区
   ```

**第二轮作业：**
- 重复上述流程，处理第二批物料 (QR_code_buffer2)
- 实现码垛功能：`Area0_To_Area1_Stack(QR_code_buffer2,0)`

### 3. 核心业务函数详解

#### 3.1 物料抓取函数

**As_QR_code_Catch_Body_TurnTable1()** - 转盘物料抓取
```c
// Targets.c:306
void As_QR_code_Catch_Body_TurnTable1(u16 QR_code)
{
    u8 Colors[3] = {QR_code / 100, QR_code / 10 % 10, QR_code % 10};
    // 解析二维码：百位=红色，十位=绿色，个位=蓝色

    // 视觉识别循环
    for each Color in Colors:
        USART_SendData(UART4, 'a');           // 请求颜色识别
        wait for vision response;
        if(识别颜色 == 目标颜色):
            Catch_TurnTable(目标颜色);         // 执行抓取动作
            Move_group(6, 200);               // 机械臂复位
```

**基础抓取动作 - Catch_TurnTable()**
```c
// Targets.c:17
void Catch_TurnTable(u8 Color)
{
    Elevator_Move_Plate();           // 升降到平台高度
    Move_group(Color + 10, 500);     // 转盘转到目标颜色+爪子闭合
    delay_ms(300);
    Elevator_Height_Init();          // 升降机回到初始高度
    Move_group(2, 130);              // 底盘朝内
    Elevator_Move_Mid();             // 降到物料盘高度
    Move_group(5, 500);              // 松开爪子
    Elevator_Height_Init();          // 回到初始高度
}
```

#### 3.2 区域转运函数

**Area0_To_Area1()** - 粗加工区到半成品区
```c
// Targets.c:120
void Area0_To_Area1(u16 QR_code, u8 Ma_Duo)
{
    // 根据二维码信息确定转运路径
    // Ma_Duo = 0: 单层放置, 1: 码垛放置
    // 实现精确的路径规划和定位
}
```

**Turntable_To_Area0()** - 转盘区到粗加工区
```c
// 预定义移动路径和距离参数
// 确保物料安全转运
```

#### 3.3 精确定位功能

**视觉反馈调整**
```c
// Di_Pan_Move.h:23-26
u8 Di_Pan_Vision_static_dist_Adjust(u8 catch_or_put);  // 静态距离调整
u8 Di_Pan_Vision_slope_Adjust(u8 ma_or_Di);           // 斜率调整
u8 Di_Pan_Vision_Stack_dist_Adjust(u8 catch_or_put);   // 码垛距离调整
```

### 4. 数据结构和变量

#### 4.1 关键全局变量
```c
// main.c
u16 QR_code_buffer1, QR_code_buffer2;    // 二维码数据缓冲
float pitch, roll, yaw;                  // 欧拉角 (MPU6050)
u8 opencv_receive[8];                    // 视觉系统数据接收缓冲
u16 elevator_pulse;                      // 升降机脉冲计数
unsigned int target_pulse;               // 目标脉冲数
```

#### 4.2 串口通信协议
- **UART4**: 与树莓派视觉系统通信
- **USART1**: 其他设备通信
- **数据格式**: 8字节数据包，以0x5d结束

### 5. 机械臂控制逻辑

#### 5.1 动作组定义 (Move_group)
```c
// 动作编号对照表:
// 1: 初始位置
// 2: 底盘朝内
// 3: 底盘朝外
// 4: 爪子闭合
// 5: 爪子张开
// 6: 底盘朝外+物料盘复位
// 10+颜色: 转盘转到指定颜色+爪子闭合
// 20+颜色: 转盘转到指定颜色+底盘朝内
```

#### 5.2 升降机高度控制
```c
// elevator_control.h
void Elevator_Height_Init(void);    // 初始高度
void Elevator_Move_Bottom(void);    // 地面高度
void Elevator_Move_StackHeight(void); // 码垛高度
void Elevator_Move_Plate(void);     // 平台高度
void Elevator_Move_Mid(void);       // 中间高度
```

### 6. 视觉系统集成

#### 6.1 识别流程
1. **补光控制**: `Light_on_shang` / `Light_off_shang`
2. **识别请求**: 发送字符'a'给树莓派
3. **数据接收**: 等待8字节数据包 (以0x5d结束)
4. **颜色匹配**: 比较识别结果与目标颜色

#### 6.2 二维码解析
```c
// 二维码格式: ABC (三位数)
// A: 红色物料编号 (1-9)
// B: 绿色物料编号 (1-9)
// C: 蓝色物料编号 (1-9)
u8 Colors[3] = {QR_code / 100, QR_code / 10 % 10, QR_code % 10};
```

### 7. 错误处理和容错机制

#### 7.1 串口通信错误处理
```c
while(opencv_receive[7] != 0x5d) {
    USART_SendData(UART4, 'a');    // 重发请求
    delay_ms(500);
}
```

#### 7.2 位置确认
- 使用MPU6050和GY-61传感器进行姿态控制
- 视觉反馈进行精确位置调整
- 多重确认确保操作安全

### 8. 工作模式

#### 8.1 测试模式
- **模式1**: 完整作业流程演示
- **模式2**: 单步调试功能
- **测试函数**: `Test_Ma_Duo()`, `Final()`

#### 8.2 运行模式
- **自动模式**: 按键1触发完整作业流程
- **手动模式**: 单独功能测试
- **调试模式**: 参数调整和功能验证

### 9. 性能参数

#### 9.1 运动参数
```c
// Di_Pan_Move.h:7-9
#define speed_Tiao_Zheng  400      // 调整速度
#define speed_MIN         150      // 最小速度
#define speed_MAX         15000    // 最大速度
```

#### 9.2 距离参数
```c
// Targets.h:20-21
#define Green_To_Turntable     3.8   // 绿区到转盘距离
#define Green_To_Red_or_Blue   0.6   // 颜色区间距离
```

### 10. 系统特点

1. **智能识别**: 基于二维码和颜色识别的物料分类
2. **精确定位**: 多传感器融合的定位系统
3. **灵活作业**: 支持不同物料类型的抓取和放置
4. **自动码垛**: 智能码垛算法实现多层堆叠
5. **容错机制**: 完善的错误检测和恢复功能
6. **模块化设计**: 清晰的功能模块划分，便于维护

### 11. 扩展性

系统采用模块化设计，支持：
- 新增工作区域
- 不同类型物料处理
- 复杂码垛模式
- 多机器人协作
- 远程监控和控制

该业务逻辑文档详细描述了工创项目的核心功能、工作流程和技术实现，为系统维护和功能扩展提供了完整的技术参考。
