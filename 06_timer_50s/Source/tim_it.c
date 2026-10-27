/*********************************************************************************************
* 文件: tim_it.c
* 说明：定时器中断服务函数
* 功能：处理TIM3定时器中断，设置时间到达标志
*********************************************************************************************/
#include "tim_it.h"

// 外部变量声明（在 main.c 中定义）
extern uint8_t timer_flag;

/**
  * @brief  TIM3中断服务函数
  * @note   每1秒触发一次，设置标志位通知主函数
  * @param  None
  * @retval None
  */
void TIM3_IRQHandler(void)
{
    // 检查是否是更新中断
    if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        // 设置定时器标志位，通知主函数时间到了
        timer_flag = 1;
        
        // 清除中断标志位（必须！否则会一直触发中断）
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    }
}
