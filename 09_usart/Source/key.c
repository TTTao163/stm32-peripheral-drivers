/*********************************************************************************************
* 文件: key.c
* 说明：按键驱动程序
* 功能：按键初始化和扫描（带消抖）
*********************************************************************************************/
#include "key.h"

/**
  * @brief  按键初始化
  * @note   配置PB12为上拉输入，按下为低电平
  * @param  None
  * @retval None
  */
void key_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    // 使能GPIOB时钟
    RCC_AHB1PeriphClockCmd(KEY_CLK, ENABLE);
    
    // 配置GPIO参数
    GPIO_InitStruct.GPIO_Pin = KEY_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;           // 输入模式
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;           // 上拉，按下为低电平
    GPIO_InitStruct.GPIO_Speed = GPIO_High_Speed;
    GPIO_Init(KEY_PORT, &GPIO_InitStruct);
}

/**
  * @brief  按键扫描（带消抖）
  * @note   使用静态变量实现软件消抖
  * @param  None
  * @retval 1=按键按下，0=按键未按下
  */
uint8_t key_scan(void)
{
    static uint8_t key_pressed = 0;  // 静态变量，保持按键状态
    
    // 检测按键按下（低电平）
    if(GPIO_ReadInputDataBit(KEY_PORT, KEY_PIN) == 0)
    {
        if(key_pressed == 0)  // 第一次检测到按下
        {
            key_pressed = 1;
            return 1;  // 按键按下瞬间返回1
        }
    }
    else  // 按键释放（高电平）
    {
        key_pressed = 0;  // 重置状态
    }
    
    return 0;  // 按键未按下或已处理
}
