#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "pwm.h"
#include "key.h"


//ALIENTEK 探索者STM32F407开发板 实验9
//TIM3 PWM三色呼吸灯实验
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com  
//广州市星翼电子科技有限公司  
//作者：正点原子 @ALIENTEK

int main(void)
{ 
	u16 pwm_val=0;            // 当前PWM值
	u8 dir=1;                 // 渐变方向: 1-递增, 0-递减
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 设置系统中断优先级分组2
	delay_init(168);          // 初始化延时函数
	uart_init(115200);        // 初始化串口波特率为115200
 	TIM3_PWM_Init(500-1,84-1); // 84M/84=1MHz的计数频率,重装值500,所以PWM频率为 1M/500=2KHz
	
	
	while(1) 
	{
		delay_ms(5);	 
		
		if(dir) pwm_val += 2;   
		else pwm_val -= 2;      
		
		if(pwm_val >= 499) 
		{
			pwm_val = 499;
			dir = 0;  
		}
		if(pwm_val <= 1 || pwm_val > 500) 
		{
			pwm_val = 0;
			dir = 1;
		}
		
		// 红绿蓝三个通道同时呼吸
		TIM_SetCompare1(TIM3, pwm_val);   // 红色 - CH1 (PA6)
		TIM_SetCompare2(TIM3, pwm_val);   // 绿色 - CH2 (PA7)
		TIM_SetCompare3(TIM3, pwm_val);   // 蓝色 - CH3 (PB0)
		

	}
}
