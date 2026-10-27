#include "stm32f4xx.h"                  // Device header
#include "delay.h"
#include "key.h"
extern char led_status;  
void key_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    // 配置PB12为输入
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
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE); 
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource12);     //PB14 连接到中断线14 
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource13); 	//使能SYSCFG时钟 
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource14);     //PB14 连接到中断线14 
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource15);     //PB15 连接到中断线15 
  EXTI_InitStructure.EXTI_Line = EXTI_Line14 | EXTI_Line15|EXTI_Line12 | EXTI_Line13;               //LINE14、LINE15 
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;                    //中断事件 
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;                  //下降沿触发  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;                           //使能LINE14、LINE15 
  EXTI_Init(&EXTI_InitStructure);                                       //按上述参数配置 
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;                //外部中断15-10 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;               //抢占优先级0 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;                     //子优先级1 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                   //使能外部中断通道 
  NVIC_Init(&NVIC_InitStructure);                                      //按上述配置初始化 
} 
char get_key_status(void) 
{ 
  char key_status = 0; 
  if(GPIO_ReadInputDataBit(K1_PORT,K1_PIN) == 0)                //判断PB12引脚电平状态 
    key_status |= K1_PREESED;                                  //低电平key_status bit0位置1 
  if(GPIO_ReadInputDataBit(K2_PORT,K2_PIN) == 0)                //判断PB13引脚电平状态 
    key_status |= K2_PREESED;                                  //低电平key_status bit1位置1 
  if(GPIO_ReadInputDataBit(K3_PORT,K3_PIN) == 0)                //判断PB14引脚电平状态 
    key_status |= K3_PREESED;                                  //低电平key_status bit2位置1 
  if(GPIO_ReadInputDataBit(K4_PORT,K4_PIN) == 0)                //判断PB15引脚电平状态 
    key_status |= K4_PREESED;                                  //低电平key_status bit3位置1 
  return key_status; 
}
void EXTI15_10_IRQHandler(void) 
{ 
  if(get_key_status() == K1_PREESED)
  {                            
    Delay_ms(200);                                           
    if(get_key_status() == K1_PREESED)
	{                           
      while(get_key_status() == K1_PREESED);                     //等待按键松开 
      led_status = 1;                                    //翻转led状态标志 
    } 
  } 

  
   if(get_key_status() == K2_PREESED)
  {                        
    Delay_ms(200);                                           
    if(get_key_status() == K2_PREESED)
	{                           
      while(get_key_status() == K2_PREESED);                     //等待按键松开 
      led_status = 2;                                    //翻转led状态标志 
    } 
  } 
  
     if(get_key_status() == K3_PREESED)
	{                            
    Delay_ms(200);                                           
    if(get_key_status() == K3_PREESED)
	{                           
      while(get_key_status() == K3_PREESED);                     //等待按键松开 
      led_status = 3;                                    //翻转led状态标志 
    } 
  } 

     if(get_key_status() == K4_PREESED)
	{                            
    Delay_ms(200);                                           
    if(get_key_status() == K4_PREESED)
	{                           
      while(get_key_status() == K4_PREESED);                     //等待按键松开 
      led_status = 4;                                    //翻转led状态标志 
    } 
	} 
  
  if(EXTI_GetITStatus(EXTI_Line14)!=RESET) 
    EXTI_ClearITPendingBit(EXTI_Line14);                        //清除LINE14上的中断标志位 
  if(EXTI_GetITStatus(EXTI_Line15)!=RESET) 
    EXTI_ClearITPendingBit(EXTI_Line15);  
 if(EXTI_GetITStatus(EXTI_Line12)!=RESET) 
    EXTI_ClearITPendingBit(EXTI_Line12);                        //清除LINE14上的中断标志位 
  if(EXTI_GetITStatus(EXTI_Line13)!=RESET) 
    EXTI_ClearITPendingBit(EXTI_Line13);  
}
