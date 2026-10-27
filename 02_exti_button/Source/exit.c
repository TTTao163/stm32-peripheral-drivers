
#include "exit.h"
#include "KEY.h"
#include "delay.h"

extern char led_status;

/**
  * @brief  外部中断15-10处理函数
  * @note   处理 EXTI Line10 到 Line15 的中断
  * @param  None
  * @retval None
  */
void EXTI15_10_IRQHandler(void) 
{ 
  // 检测K1按键（PB12 -> EXTI Line12）
  if(EXTI_GetITStatus(EXTI_Line12) != RESET)
  {
    Delay_ms(10);  // 消抖延时
    
    // 确认按键确实按下
    if(GPIO_ReadInputDataBit(K1_PORT, K1_PIN) == 0)
    {
      // 等待按键释放（避免重复触发）
      while(GPIO_ReadInputDataBit(K1_PORT, K1_PIN) == 0);
      
      // 翻转LED状态：0变1，1变0
      led_status = !led_status;
    }
    
    // 清除中断标志位（必须！）
    EXTI_ClearITPendingBit(EXTI_Line12);
  }
  
  // 检测K2按键（PB13 -> EXTI Line13）
  if(EXTI_GetITStatus(EXTI_Line13) != RESET)
  {
    Delay_ms(10);  // 消抖延时
    
    if(GPIO_ReadInputDataBit(K2_PORT, K2_PIN) == 0)
    {
      while(GPIO_ReadInputDataBit(K2_PORT, K2_PIN) == 0);
      
      // K2的功能可以在这里添加
      // 例如：led_status = 2;
    }
    
    EXTI_ClearITPendingBit(EXTI_Line13);
  }
}
