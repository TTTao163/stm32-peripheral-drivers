#include "pwm.h"
#include "led.h"
#include "usart.h"


//TIM8 PWM部分初始化 - 双色呼吸灯
//PWM输出初始化 - 使用TIM8的CH2和CH3通道控制LED
//arr：自动重装值
//psc：时钟预分频数
void TIM8_PWM_Init(u32 arr,u32 psc)
{		 					 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);  	//TIM8时钟使能 (APB2)
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	//使能PORTB时钟	
	
	// TIM8互补通道引脚配置
	// 配置PB0为TIM8_CH2N - 红色LED (互补输出)
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource0,GPIO_AF_TIM8);
	// 配置PB1为TIM8_CH3N - 绿色LED (互补输出)
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource1,GPIO_AF_TIM8);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; //PB0,PB1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure);              //初始化
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0; //重复计数器(高级定时器特有)
	
	TIM_TimeBaseInit(TIM8,&TIM_TimeBaseStructure);//初始化定时器8
	
	//初始化TIM8 PWM模式 - 使用互补输出通道2N,3N
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //PWM模式1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable; //主输出禁用
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; //互补输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //主输出极性
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low; //互补输出极性低
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset; 
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	
	// 初始化通道2（使用CH2N互补输出到PB0）
	TIM_OC2Init(TIM8, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);
	
	// 初始化通道3（使用CH3N互补输出到PB1）
	TIM_OC3Init(TIM8, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);
 
	TIM_ARRPreloadConfig(TIM8,ENABLE);//ARPE使能 
	
	TIM_Cmd(TIM8, ENABLE);  //使能TIM8
	
	TIM_CtrlPWMOutputs(TIM8,ENABLE); //高级定时器必须使能主输出MOE
 
										  
}  


