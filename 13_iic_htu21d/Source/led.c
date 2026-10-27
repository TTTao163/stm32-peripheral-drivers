/*********************************************************************************************
* 文件: led.c
* 说明: LED驱动
*       D1->PE0; D2->PE1; D3->PE2; D4->PE3
*********************************************************************************************/
#include "led.h"

/*********************************************************************************************
* 名称：led_init()
* 功能：LED初始化
*********************************************************************************************/
void led_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_AHB1PeriphClockCmd(LED_CLK, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = D1_PIN | D2_PIN | D3_PIN | D4_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(LED_PORT, &GPIO_InitStructure);
    
    /* 初始状态：全部关闭（高电平关闭） */
    GPIO_SetBits(LED_PORT, D1_PIN | D2_PIN | D3_PIN | D4_PIN);
}

/*********************************************************************************************
* 名称：turn_on()
* 功能：打开LED
* 参数：Led - LED编号，可以用 | 组合多个
*********************************************************************************************/
void turn_on(unsigned char Led)
{
    if(Led & D1) GPIO_ResetBits(LED_PORT, D1_PIN);
    if(Led & D2) GPIO_ResetBits(LED_PORT, D2_PIN);
    if(Led & D3) GPIO_ResetBits(LED_PORT, D3_PIN);
    if(Led & D4) GPIO_ResetBits(LED_PORT, D4_PIN);
}

/*********************************************************************************************
* 名称：turn_off()
* 功能：关闭LED
* 参数：Led - LED编号，可以用 | 组合多个
*********************************************************************************************/
void turn_off(unsigned char Led)
{
    if(Led & D1) GPIO_SetBits(LED_PORT, D1_PIN);
    if(Led & D2) GPIO_SetBits(LED_PORT, D2_PIN);
    if(Led & D3) GPIO_SetBits(LED_PORT, D3_PIN);
    if(Led & D4) GPIO_SetBits(LED_PORT, D4_PIN);
}
