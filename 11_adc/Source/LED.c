#include "stm32f4xx.h"// Device header
#include "led.h"

void led_init(void) 
{ 
    GPIO_InitTypeDef GPIO_InitStructure; 
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOB , ENABLE);  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOE, &GPIO_InitStructure); 
		GPIO_SetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3); 
	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 ;  
	//GPIO_Init(GPIOB, &GPIO_InitStructure);  
	//GPIO_SetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2);  
}

void turn_on(unsigned char led)
{ 
	 if(led & D1) 
	 GPIO_ResetBits(GPIOE, GPIO_Pin_0);  
	 if(led & D2) 
	 GPIO_ResetBits(GPIOE, GPIO_Pin_1);  
	 if(led & D3) 
	 GPIO_ResetBits(GPIOE, GPIO_Pin_2);  
	 if(led & D4) 
	 GPIO_ResetBits(GPIOE, GPIO_Pin_3);  
	 if(led & LEDR) 
	 GPIO_ResetBits(GPIOB, GPIO_Pin_0);  
	 if(led & LEDG) 
	 GPIO_ResetBits(GPIOB, GPIO_Pin_1);  
	 if(led & LEDB) 
	 GPIO_ResetBits(GPIOB, GPIO_Pin_2);  
}

void turn_off(unsigned char led)
{ 
	 if(led & D1) 
	 GPIO_SetBits(GPIOE, GPIO_Pin_0); 
	 if(led & D2) 
	 GPIO_SetBits(GPIOE, GPIO_Pin_1);  
	 if(led & D3) 
	 GPIO_SetBits(GPIOE, GPIO_Pin_2);  
	 if(led & D4) 
	 GPIO_SetBits(GPIOE, GPIO_Pin_3);
	 if(led & LEDR) 
	 GPIO_SetBits(GPIOB, GPIO_Pin_0); 
	 if(led & LEDG) 
	 GPIO_SetBits(GPIOB, GPIO_Pin_1);  
	 if(led & LEDB) 
	 GPIO_SetBits(GPIOB, GPIO_Pin_2);  
}

