/*********************************************************************************************
* 文件: led.c
* 作者：liutong 2015.8.19
* 说明：LED驱动程序
*       D1->PE0;D2->PE1;D3->PE2;D4->PE3;R->PB0;G->PB1;B->PB2
* 修改：chennian 2017.11.02 修改初始化引脚
*       chennian 2017.11.22 增加注释
        zhoucj   2017.12.18 增加注释
* 注释：
*********************************************************************************************/
#include "LED.h"
/*********************************************************************************************
* 名称：led_init
* 功能：初始化LED对应的GPIO
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void led_init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;                    
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE | 
                         RCC_AHB1Periph_GPIOB , ENABLE);        //使能GPIO时钟
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 |
                                GPIO_Pin_2 | GPIO_Pin_3;        //选中引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;                 //输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;              //输出速度2MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;              //无上下拉
  GPIO_Init(GPIOE, &GPIO_InitStructure);                        //根据上述参数配置GPIOE0、GPIOE1、GPIOE2、GPIOE3
  GPIO_SetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_1 |
                      GPIO_Pin_2 | GPIO_Pin_3);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | 
                                GPIO_Pin_1 |
                                GPIO_Pin_2 ;                    //选中0、1、2号引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;                 //输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;              //输出速度2MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;              //无上下拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);                        //根据上述参数配置GPIOB0、GPIOB1、GPIOB2
  GPIO_SetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2);
}
