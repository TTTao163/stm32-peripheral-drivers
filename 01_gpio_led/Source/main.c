#include "stm32f4xx.h"
#include "LED.h"
int main(void)
{ 
 
   // delay_init();          //初始化延时函数
     led_init();                //初始化LED端口
    while(1)
    {
            GPIO_ResetBits(GPIOE, GPIO_Pin_0);  //LED0对应引脚GPIOB.0拉低，点亮 LED0 等同LED0=0;
            
    }
}
