# STM32F407 外设驱动实验合集

基于 **STM32F407VET6**（Cortex-M4F，512KB Flash，192KB SRAM）的裸机外设驱动实验仓库，覆盖 GPIO、外部中断、定时器、PWM、串口、ADC、FSMC 并口屏、IIC 传感器等常用外设。

全部代码为**寄存器级裸机实现 + 标准外设库（StdPeriph V1.8.0）**两套风格，不依赖 HAL，便于理解底层时序。

## 硬件平台

| 项目 | 参数 |
|---|---|
| MCU | STM32F407VET6（Cortex-M4F，168MHz，FPU） |
| 外部晶振 | 12MHz HSE |
| 调试器 | J-Link / ST-Link（SWD） |
| 显示屏 | ST7789 / ILI93xx 控制器 TFT（FSMC 并口驱动） |
| 传感器 | HTU21D 温湿度（IIC 接口） |

## 实验清单

| 目录 | 实验内容 | 关键外设 / 知识点 |
|---|---|---|
| `01_gpio_led` | GPIO 输出，LED 点亮与闪烁 | RCC 时钟使能、GPIO 模式配置（推挽输出）、位带操作 |
| `02_exti_button` | 按键外部中断控制 LED 状态切换 | EXTI 边沿触发、NVIC 优先级分组与使能、中断服务函数 |
| `03_gpio_input` | GPIO 输入读取按键电平 | 上拉/下拉输入模式、软件消抖 |
| `04_gpio_extended` | 多按键组合逻辑与状态机 | GPIO 多路输入、状态机按键处理 |
| `05_timer_base` | TIM3 定时中断实现精确定时 | 预分频 8399（84MHz/8400 = 10kHz）、周期 9999 得到 1Hz 中断 |
| `06_timer_50s` | 定时器长周期定时（50s） | 分频与重装载值组合计算、多级计数扩展定时范围 |
| `07_pwm_tim3` | TIM3 输出 PWM 波，呼吸灯效果 | PWM 模式 1、CCR 占空比调节、GPIO 复用推挽输出 |
| `08_pwm_tim8` | TIM8 高级定时器 PWM 输出 | 高级定时器主输出使能（MOE）、互补通道配置 |
| `09_usart` | 串口收发，printf 重定向 | USART1 配置、波特率计算、`fputc` 重定向、收发中断 |
| `10_usart_extended` | 串口命令解析与协议处理 | 中断接收 + 环形缓冲、字符串命令解析 |
| `11_adc` | ADC 多通道采样 | ADC 规则组转换、采样时间配置、软件触发、结果换算 |
| `12_lcd_st7789` | ST7789 TFT 屏显示中英文 | FSMC 并口时序配置、LCD 初始化序列、字库取模与 GBK 显示 |
| `13_iic_htu21d` | IIC 总线读取 HTU21D 温湿度 | IIC 时序模拟（起始/应答/读写字节）、传感器 CRC 校验与温湿度换算 |

## 目录结构

```
.
├── 01_gpio_led/              # 每个实验为独立可编译的 Keil 工程
│   ├── Libraries/            # CMSIS + StdPeriph V1.8.0
│   ├── Source/               # 用户代码（main.c / 各外设驱动）
│   └── Project/              # Keil MDK 工程（.uvprojx）
├── 07_pwm_tim3/              # 后 7 个实验采用 CORE/FWLIB/HARDWARE/SYSTEM/USER 分层
│   ├── CORE/                 # 内核与启动文件
│   ├── FWLIB/                # 标准外设库
│   ├── HARDWARE/             # 板级外设驱动
│   ├── SYSTEM/               # delay / sys / usart 系统层
│   └── USER/                 # 应用层 main.c 与工程文件
└── 13_iic_htu21d/
```

## 编译与运行

1. 安装 **Keil MDK-ARM 5.x**，并安装 `Keil.STM32F4xx_DFP` 器件支持包
2. 打开任一实验目录下的 `*.uvprojx` 工程文件
3. `Rebuild` 编译，用 J-Link/ST-Link 下载到开发板
4. 串口实验将 PA9/PA10 接 USB-TTL，波特率 115200 查看输出

> 每个实验目录都自带一份完整的 StdPeriph 库，保证单独拷贝任一目录即可独立编译。这是 Keil 相对路径工程的必要做法，代价是仓库体积偏大。

## 说明

- 编译产物（`OBJ/`、`Objects/`、`Listings/`、`*.o`、`*.axf`、`*.hex` 等）已从仓库剔除
- 传感器与时序相关逻辑均有中文注释说明配置依据
- 作者：张涛（物联2311）
