# 工创项目结构文档

## 项目概述
本项目是一个基于STM32F103ZE微控制器的工创机器人控制系统，主要实现自动物料搬运和二维码识别功能。

## 目录结构

```
gongchuang-master/
├── .claude/                     # Claude工具配置目录
├── .gitignore                   # Git忽略文件配置
├── keilkilll.bat               # Keil进程终止脚本
├── new-code.c                  # 新代码文件
├── PING_INTRO.txt              # 项目说明文档
├── PROJECT_STRUCTURE.md        # 项目结构文档（本文件）
├── APP/                        # 应用层模块
│   ├── BEEP/                   # 蜂鸣器模块
│   │   ├── bsp_beep.c
│   │   ├── bsp_beep (2).c
│   │   └── bsp_beep.h
│   ├── Elevator_Control/       # 升降机控制模块
│   │   ├── elevator_control.c
│   │   └── elevator_control.h
│   ├── GY-61/                  # 加速度传感器模块
│   │   ├── GY_61.c
│   │   └── GY_61.h
│   ├── HMI_DISPLAY/            # 人机界面显示模块
│   │   ├── Him_Display.c
│   │   └── Him_Display.h
│   ├── LobotServoController/   # 舵机控制器模块
│   │   ├── LobotServoController.c
│   │   ├── LobotServoController.h
│   │   └── bool.h
│   ├── MPU6050/                # MPU6050姿态传感器模块
│   │   ├── eMPL/              # MPU6050运动处理库
│   │   │   ├── inv_mpu.c
│   │   │   ├── inv_mpu.h
│   │   │   ├── inv_mpu_dmp_motion_driver.c
│   │   │   ├── inv_mpu_dmp_motion_driver.h
│   │   │   ├── dmpKey.h
│   │   │   └── dmpmap.h
│   │   ├── inv_mpu.c
│   │   ├── inv_mpu.h
│   │   ├── inv_mpu_dmp_motion_driver.c
│   │   ├── inv_mpu_dmp_motion_driver.h
│   │   ├── mpu.c
│   │   ├── mpu.h
│   │   ├── mpu6050.c
│   │   ├── mpu6050.h
│   │   ├── mpuiic.c
│   │   ├── mpuiic.h
│   │   ├── dmpKey.h
│   │   └── dmpmap.h
│   ├── OLED/                   # OLED显示屏模块
│   │   ├── oled.c
│   │   ├── oled.h
│   │   └── oledfont.h
│   └── Targets/                # 目标检测模块
│       ├── Targets.c
│       └── Targets.h
├── bsp/                        # 板级支持包
│   ├── BSP.c                   # 板级支持包主文件
│   ├── BSP.h
│   ├── bsp_Arm_Steering.c      # 机械臂舵机控制
│   ├── bsp_Arm_Steering.h
│   ├── bsp_Motor_4w.c          # 四轮电机控制
│   ├── bsp_Motor_4w.h
│   ├── bsp_Timer.c             # 定时器配置
│   ├── bsp_Timer.h
│   ├── bsp_usart.c             # 串口通信配置
│   ├── bsp_usart.h
│   ├── pwm.c                   # PWM输出控制
│   └── pwm.h
├── CORE/                       # STM32核心文件
│   ├── core_cm3.c              # Cortex-M3核心功能
│   └── core_cm3.h
├── MOVE/                       # 运动控制模块
│   ├── Di_Pan_Move.c           # 底盘运动控制
│   ├── Di_Pan_Move.h
│   ├── motor.c                 # 电机驱动
│   └── motor.h
├── OBJ/                        # 编译输出文件目录
├── ROUTE/                      # 路径规划模块
│   ├── Route.c
│   └── Route.h
├── STM32F10x_FWLib/            # STM32固件库
│   ├── inc/                    # 固件库头文件
│   └── src/                    # 固件库源文件
├── SYSTEM/                     # 系统模块
│   ├── config/                 # 配置文件
│   │   └── config.h
│   ├── delay/                  # 延时功能
│   │   ├── delay.c
│   │   └── delay.h
│   ├── key/                    # 按键输入
│   │   ├── key.c
│   │   └── key.h
│   ├── LED/                    # LED指示灯
│   │   ├── led.c
│   │   └── led.h
│   ├── sys/                    # 系统配置
│   │   ├── sys.c
│   │   └── sys.h
│   ├── TIM/                    # 定时器配置
│   │   ├── tim.c
│   │   └── tim.h
│   └── USART/                  # 串口通信
│       ├── usart.c
│       └── usart.h
└── USER/                       # 用户代码
    ├── DebugConfig/            # 调试配置
    ├── Listings/               # 编译输出列表
    ├── RTE/                    # 运行时环境
    │   └── Device/
    │       └── STM32F103ZE/   # STM32F103ZE设备配置
    ├── main.c                  # 主程序文件
    ├── stm32f10x.h             # STM32F10x系列头文件
    ├── stm32f10x_conf.h        # STM32F10x配置文件
    ├── stm32f10x_it.c          # 中断服务程序
    ├── stm32f10x_it.h
    ├── system_stm32f10x.c      # 系统时钟配置
    └── system_stm32f10x.h
```

## 主要功能模块

### 1. 底盘运动控制 (MOVE/Di_Pan_Move)
- 实现机器人的前进、后退、左右移动
- 支持精确的距离控制
- 集成视觉反馈的斜率调整功能

### 2. 升降机系统 (APP/Elevator_Control)
- 控制升降机构的高度调节
- 支持多个预设高度位置
- 步进电机脉冲控制

### 3. 传感器系统
- **MPU6050** (APP/MPU6050): 姿态检测，提供欧拉角数据
- **GY-61** (APP/GY-61): 加速度传感器
- **二维码识别**: 通过串口接收视觉系统数据

### 4. 通信系统 (bsp/bsp_usart, SYSTEM/USART)
- 多串口通信支持 (USART1-5)
- 与上位机和视觉系统的数据交换
- 串口接收缓冲区管理

### 5. 机械臂控制 (bsp/bsp_Arm_Steering)
- 机械臂舵机控制
- 物料抓取和放置功能
- 转台控制

### 6. 人机界面 (APP/HMI_DISPLAY)
- OLED显示屏接口
- 蜂鸣器状态提示
- LED状态指示

## 工作流程

1. **系统初始化**: BSP_Init() 初始化硬件外设
2. **二维码识别**: 通过串口接收视觉系统识别的二维码信息
3. **路径规划**: ROUTE模块计算最优移动路径
4. **运动控制**: MOVE模块控制底盘和升降机运动
5. **物料操作**: 机械臂执行抓取和放置任务
6. **循环作业**: 支持多轮自动物料搬运

## 开发环境

- **MCU**: STM32F103ZE
- **开发工具**: Keil MDK
- **固件库**: STM32F10x Firmware Library
- **语言**: C语言
- **编译器**: ARM Compiler

## 配置文件

- **.gitignore**: Git版本控制忽略规则
- **config.h**: 系统全局配置
- **stm32f10x_conf.h**: STM32外设库配置

## 注意事项

1. 项目代码包含大量中文注释，便于理解
2. 硬件相关配置在BSP和SYSTEM目录中
3. 电机速度控制通过PWM实现
4. 串口通信协议需要与上位机配合使用
5. 定时器配置影响系统的实时性能