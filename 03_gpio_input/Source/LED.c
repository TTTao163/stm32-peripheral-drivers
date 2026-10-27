#include "stm32f4xx.h"                  // Device header
void LED_Init(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure; 
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);    //使能GPIOG时钟 
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;                        //选中0 号引脚 
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;                 //输出模式 
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                 //推挽输出 
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;               //输出速度 2MHz 
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;              //无上下拉 
	  GPIO_Init(GPIOE, &GPIO_InitStructure); 
	  GPIO_SetBits(GPIOE,GPIO_Pin_0);
	  GPIO_ResetBits(GPIOE,GPIO_Pin_1);
} 
