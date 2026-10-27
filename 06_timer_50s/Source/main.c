#include "stm32f4xx.h"   // Device header
#include "led.h"
#include "timer.h"
#include "key.h"

// 定时器标志位（在中断中设置）
uint8_t timer_flag = 0;

/**
  * @brief  主函数
  * @note   使用TIM3定时器实现50秒定时，按键复位
  *         - 通过预分频系数直接实现50秒定时
  *         - 50秒到达：D1、D2熄灭，D3点亮
  *         - 按下PB12按键：复位计时器，重新开始
  */
int main(void) 
{ 
    // 初始化
    led_init();
    key_init();
    
    // 初始化TIM3定时器（50秒定时）
    // 定时时间计算：
    // TIM3时钟 = 84MHz
    // 目标：50秒定时
    // 
    // 由于period最大值为65535（16位），需要调整参数
    // 
    // 最终方案：prescaler = 63999, period = 65624
    // 计算过程：
    // 步骤1: 84MHz / (63999+1) = 84MHz / 64000 = 1312.5Hz
    // 步骤2: 1312.5Hz / (65624+1) = 1312.5Hz / 65625 = 0.02Hz
    // 步骤3: 1 / 0.02Hz = 50秒 ✓
    // 
    // 验证：(64000 × 65625) / 84000000 = 4200000000 / 84000000 = 50秒
    TIM3_Init(63999, 65624);

    // 初始状态：所有LED灭
    turn_off(D1);
    turn_off(D2);
    turn_off(D3);
    
    // 主循环
    while(1)
    {
        // 检查按键（复位功能）
        if(key_scan() == 1)
        {
            // 复位定时器
            TIM_SetCounter(TIM3, 0);    // 清零计数器
            turn_off(D3);               // 熄灭D3
            timer_flag = 0;             // 清除标志位
        }
        
        // 检查定时器标志位（50秒到达）
        if(timer_flag == 1)
        {
            timer_flag = 0;  // 清除标志位
            
            // 50秒到达
            turn_off(D1);   // 关闭D1
            turn_off(D2);   // 关闭D2
            turn_on(D3);    // 点亮D3（表示时间到）
        }
    }
}
