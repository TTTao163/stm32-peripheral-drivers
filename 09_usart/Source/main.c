#include "stm32f4xx.h"   // Device header
#include "KEY.h"
#include "led.h"
#include "usart.h"
#include "delay.h"
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	usart_init(115200);
	led_init(); 
	Delay_ms(100);
	usart_send("你好，南工！\r\n",strlen("你好，南工！\r\n"));
	usart_send("ZT is ready!\r\n",strlen("ZT is ready!\r\n"));
	while(1)
	{
		
	}
}
