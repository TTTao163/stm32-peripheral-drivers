#include "stm32f4xx.h"   // Device header
#include "led.h"
#include "timer.h"

// 定时器标志位（在中断中设置）
uint8_t timer_flag = 0;

/**
  * @brief  主函数
  * @note   使用TIM3定时器实现D1和D2交替闪烁
  *         D1亮1秒，D2灭 -> D1灭，D2亮1秒 -> 循环
  */
int main(void) 
{ 
    uint8_t led_state = 0;  // LED状态：0=D1亮D2灭，1=D1灭D2亮
    
    // 初始化LED
    led_init();
    
    // 初始化TIM3定时器
    // 参数说明：
    // prescaler = 8399  -> 84MHz / (8399+1) = 10KHz
    // period = 9999     -> 10KHz / (9999+1) = 1Hz (1秒)
    TIM3_Init(8399, 9999);
    
    // 主循环
    while(1)
    {
        // 检查定时器标志位
        if(timer_flag == 1)
        {
            timer_flag = 0;  // 清除标志位
            
            // 根据状态控制LED交替闪烁
            if(led_state == 0)
            {
                // 状态0: D1亮，D2灭
                turn_on(D1);    // D1点亮
                turn_off(D2);   // D2熄灭
                led_state = 1;  // 切换到状态1
            }
            else
            {
                // 状态1: D1灭，D2亮
                turn_off(D1);   // D1熄灭
                turn_on(D2);    // D2点亮
                led_state = 0;  // 切换到状态0
            }
        }
        
        // 主循环可以做其他事情
    }
}
