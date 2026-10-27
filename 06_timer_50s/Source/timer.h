#ifndef __TIMER_H
#define __TIMER_H

#include "stm32f4xx.h"

/*********************************************************************************************
* 函数声明
*********************************************************************************************/
void TIM3_Init(uint16_t prescaler, uint16_t period);  // TIM3初始化函数
void TIM3_IRQHandler(void);                           // TIM3中断服务函数

#endif /* __TIMER_H */
