/*********************************************************************************************
* 文件: led.h
* 说明: LED驱动头文件
*       D1->PE0; D2->PE1; D3->PE2; D4->PE3
*********************************************************************************************/
#ifndef _LED_H_
#define _LED_H_

#include "stm32f4xx.h"

/* LED硬件配置 */
#define LED_PORT    GPIOE
#define LED_CLK     RCC_AHB1Periph_GPIOE
#define D1_PIN      GPIO_Pin_0
#define D2_PIN      GPIO_Pin_1
#define D3_PIN      GPIO_Pin_2
#define D4_PIN      GPIO_Pin_3

/* LED编号定义 */
#define D1      0x01
#define D2      0x02
#define D3      0x04
#define D4      0x08

/* 函数声明 */
void led_init(void);
void turn_on(unsigned char Led);
void turn_off(unsigned char Led);

#endif 

