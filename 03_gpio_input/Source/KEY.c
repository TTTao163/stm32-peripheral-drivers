#include "stm32f4xx.h"
#include "misc.h"
void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
	
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    // ≈‰÷√PB12Œ™ ‰»Î
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed=GPIO_High_Speed;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
}
