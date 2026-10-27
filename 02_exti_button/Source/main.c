#include "stm32f4xx.h"   // Device header
#include "KEY.h"
#include "led.h"
#include "delay.h"

char led_status = 0;  // 

int main(void) 
{ 
	led_init();      // 初始化LED
	exti_init();     // 初始化外部中断
	
	while(1)
	{
		if(led_status == 0)
		{
			turn_on(D1);    // LED1亮
			turn_off(D2);   // LED2灭
		} 
		else
		{
			turn_off(D1);   // LED1灭
			turn_on(D2);    // LED2亮
		} 
	} 
}   
