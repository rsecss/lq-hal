# 嵌入式模板——HAL

## Introduction

本项目是主要是是为了备战嵌入式蓝桥杯比赛而创建的模板同时不断进行优化，目前官方采用的是 **CT117E-M4** 嵌入式平台，该平台采用的主控芯片是 **STM32G431RBT6**，同时采用 **HAL** 框架进行代码的编写，开发方式采用的是 **STM32CubeMX + MDK + VsCode** 进行开发，官方采用的是 **DAP-Link** 进行调试，同时借助官方提供的 **LCD** 的模板还可以进行打印调试。

## Highlights

- 采用了 **静态任务调度器**，方便进行多任务的编写
- 同时项目集中 `"bsp_system.h"` 聚合头文件管理各种链接关系。

## Directory structure

```plaintxt
|-- Reference/                      # 参考资料
    |-- Individual                  # 个人整理的资料
    |-- Official                    # 官方提供的参考资料
    |-- Lanqiao                     # 蓝桥杯（电子类）历届真题
|-- Template/                       # 项目工程
    |-- Core                        # 核心层
    |-- Drivers                     # 驱动层
    |-- MDK-ARM                     # MDK 工程文件
    |-- MyApplication               # 应用层
    |-- Template-CubeMx             # CubeMX 工程文件
    |-- Keilkill                    # 批处理文件
|-- Tools/                          # 工具软件
    |-- ARMCC                       # AC5 工具链
    |-- STM32G4xx_DFP.1.4.0         # 主控芯片固件包
    |-- USB Driver                  # USB 驱动程序
    |-- Serial Port Assistants      # 串口调试助手

```

## Usage

直接 clone 项目到本地，然后打开 MDK 工程文件进行编译调试即可。

**注意事项：**

1. CubeMX 中这里 SYS 不需要配置是因为开发平台自带调试器，可以不用选择 Debug，而在其他平台则一般选择采用 `Serial Wire Debug`，否则可能无法调试和烧录。
2. CubeMX 中的 NVIC 中配置系统滴答定时器要配置成最高优先级（0），防止频繁被其他任务抢占导致计时精度下降或者系统响应延迟。
3. CubeMX 中的晶振选择 `24MHz`，时钟树选择配置为 `HSI`，同时配置为 `80MHz`。
4. CubeMX 中的 IDE 选择 `MDK-ARM` ，同时工程配置包的选择不能使用最新的，因为官方只给了 1.40 和 1.20 的固件配置包
5. CubeMX 中的 Code Generator（生成选项卡）中勾选 `Add necessary library files as reference in the project` 和 `Generate peripheral initialization as a pair of'.c/.h'files per peripheral`
6. 进入 keil 中 首先取消勾选 `Manage Run-Time Environment` 中的 CMSIS，原因是后面添加文件可能会导致编译失败。
7. 注意查看 Keil 的版本，如果版本过低，可能会无法打开工程。
8. keil 中需要安装主控芯片的固件包 `Keil.STM32G4xx_DFP.1.4.0.pack`，否则编译失败。
9. 如果串口方面出现程序下载卡死或者无法下载，应该首先查看魔术棒下的 target 下的 ==**微库（MicroLIB）**== 是否勾选，如果没有勾选，请勾选，然后重新编译。）是否勾选，如果这里勾选了然后再检查其他的配置是否正确。
10. 需要进入 C/C++ 设置界面下面的 Optimization 选择 `Level 0`，同时在 includepath 栏加入头文件路径 `.\MyApplication`, 否则无法识别头文件。
11. 在 Debug 设置界面，仿真器选择 **`CMSIS-DAP Debugger`**，同时在旁边的设置中选择 Trace 更改时钟频率为 `80MHz`、选择 Port 为 `SW` 和选择 Flash Download 勾选上 `Reset and Run` 按钮，方便后续的下载和操作。
12. `keilkill.bat` 是批处理文件，主要是为了方便分享工程时，可以清除编译产生的中间文件，这个可以视情况和选择

## Contributions

如果上述提供的资料和模板出现问题，欢迎大家 Issue 和 PR ！

最后，感谢广大的贡献者参与贡献！

## Statement

该仓库提供的任何资料或者源码仅供学习参考使用。倘若是用于其他用途出现问题，概不负责！
