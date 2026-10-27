#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"

//按键定义 - 使用PA0 (WKUP按键，STM32F407VET6常见配置)
#define KEY0  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) // PA0

#define KEY0_PRES 1  // KEY0按下

void KEY_Init(void); // 按键初始化
u8 KEY_Scan(u8 mode); // 按键扫描函数

#endif
