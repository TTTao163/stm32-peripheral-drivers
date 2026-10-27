#include "stm32f4xx.h"
#include "delay.h"
void Delay_ms(uint32_t ms)
{
    for (uint32_t i = 0; i < ms * 8000; i++);
}
