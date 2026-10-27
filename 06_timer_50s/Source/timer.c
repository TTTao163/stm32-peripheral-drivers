#include "timer.h"

/**
  * @brief  TIM3定时器初始化
  * @param  prescaler: 预分频值 (0-65535)
  * @param  period: 自动重装载值 (0-65535)
  * @retval None
  * 
  * @note   定时时间计算公式：
  *         T = ((prescaler + 1) * (period + 1)) / 定时器时钟频率
  *         
  *         STM32F407 的 TIM3 挂在 APB1 上
  *         APB1 时钟频率通常为 84MHz (如果 APB1 预分频器不为1，则定时器时钟自动x2)
  *         
  *         示例：实现1秒定时
  *         prescaler = 8399  (84MHz / (8399+1) = 10000Hz = 10KHz)
  *         period = 9999     (10000Hz / (9999+1) = 1Hz = 1秒)
  */
void TIM3_Init(uint16_t prescaler, uint16_t period)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    // 1. 使能TIM3时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    
    // 2. 配置TIM3基本参数
    TIM_TimeBaseStructure.TIM_Period = period;              // 自动重装载值
    TIM_TimeBaseStructure.TIM_Prescaler = prescaler;        // 预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; // 时钟分频因子
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    
    // 3. 使能TIM3更新中断
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    
    // 4. 配置TIM3中断优先级
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;              // TIM3中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;    // 抢占优先级1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;           // 响应优先级1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;              // 使能中断
    NVIC_Init(&NVIC_InitStructure);
    
    // 5. 启动定时器
    TIM_Cmd(TIM3, ENABLE);
}

// TIM3中断服务函数已移至 tim_it.c 文件
