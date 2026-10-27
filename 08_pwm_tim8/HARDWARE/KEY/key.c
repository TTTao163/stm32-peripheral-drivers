#include "key.h"
#include "delay.h" 

// 按键初始化函数
void KEY_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); // 使能GPIOA时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; // KEY0对应引脚 PA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; // 普通输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // 100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; // 下拉 (PA0/WKUP按键按下为高电平)
	GPIO_Init(GPIOA, &GPIO_InitStructure); // 初始化GPIOA0
} 

// 按键扫描函数
// mode: 0-不支持连续按; 1-支持连续按
// 返回值: 0-无按键按下; 1-KEY0按下
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1; // 按键按松开标志
	
	if(mode) key_up=1;  // 支持连按		  
	
	if(key_up && (KEY0==1))  // PA0/WKUP按键按下为高电平
	{
		delay_ms(10); // 消抖
		key_up=0;
		if(KEY0==1) return KEY0_PRES;
	}
	else if(KEY0==0) key_up=1; 	    
	
	return 0; // 无按键按下
}
