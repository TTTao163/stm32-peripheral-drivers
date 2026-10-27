#ifndef __ADC_H
#define __ADC_H
#include "stm32f4xx.h" 
void Adc_Init(void);
u16 AdcGet(u8 ch);
#endif 

