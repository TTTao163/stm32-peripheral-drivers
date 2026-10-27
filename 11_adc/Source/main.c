#include "stm32f4xx.h"   // Device header
#include "delay.h"
#include "ADC.h"
#include "usart.h"
u16 ADCvalue = 0;                                                      
float value1=0;  
int main(void)
{
	usart_init(115200);
	Delay_ms(100);
	Adc_Init();
	Delay_ms(100);
    while (1)
    {
		ADCvalue=AdcGet(1);                                         //adc值获取 
		
		value1 =(ADCvalue*(3.3f/4095.f));
		usart_send_float(ADCvalue);
		usart_send_float(value1);
		Delay_ms(1000);
    }
}
