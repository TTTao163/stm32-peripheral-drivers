#include "stm32f4xx.h"   // Device header
#include "KEY.h"
#include "led.h"
#include "delay.h"
int main()
{
	LED_Init();
	KEY_Init();
	while(1)
	{
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == Bit_RESET)
        {
            Delay_ms(50); // ¼òµ¥·À¶¶
            if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == Bit_RESET)
            {
				GPIO_ToggleBits(GPIOE, GPIO_Pin_0|GPIO_Pin_1);
                while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == Bit_RESET);  
            }
        }
	}
	
}
