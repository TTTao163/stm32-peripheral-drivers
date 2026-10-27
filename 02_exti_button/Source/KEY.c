#include "stm32f4xx.h"                  // Device header
#include "delay.h"
#include "key.h"
extern char led_status;  
void key_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_InitStruct.GPIO_Speed=GPIO_High_Speed;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
}
void exti_init(void) 
{ 
  key_init();                                                          //按键引脚初始化
  NVIC_InitTypeDef   NVIC_InitStructure; 
  EXTI_InitTypeDef   EXTI_InitStructure; 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);        //使能SYSCFG时钟
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource12);     //PB12 连接到中断线12
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource13);     //PB13 连接到中断线13
  EXTI_InitStructure.EXTI_Line = EXTI_Line12 | EXTI_Line13;               //LINE12和LINE13
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;                    //中断事件
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;                  //下降沿触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;                           //使能LINE12和LINE13
  EXTI_Init(&EXTI_InitStructure);                                       //配置外部中断
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;                //外部中断15-10
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;               //抢占优先级0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;                     //响应优先级1
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                   //使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);                                      //中断优先级初始化
} 
char get_key_status(void) 
{ 
  char key_status = 0; 
  if(GPIO_ReadInputDataBit(K1_PORT,K1_PIN) == 0)                
    key_status |= K1_PREESED;                                  
  if(GPIO_ReadInputDataBit(K2_PORT,K2_PIN) == 0)                
    key_status |= K2_PREESED;                                  
  if(GPIO_ReadInputDataBit(K3_PORT,K3_PIN) == 0)                
    key_status |= K3_PREESED;                                  
  if(GPIO_ReadInputDataBit(K4_PORT,K4_PIN) == 0)               
    key_status |= K4_PREESED;                                  
  return key_status; 
}
// 中断服务函数已移至 exit.c 文件
