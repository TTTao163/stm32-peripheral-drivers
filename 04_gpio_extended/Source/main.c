#include "stm32f4xx.h"   // Device header
#include "KEY.h"
#include "led.h"
#include "delay.h"
char led_status = 0;   
uint8_t flag=0;
int main(void) 
{ 
	led_init();                                                  
	exti_init();                                                  
	while(1)
	{                                                  
		if((led_status == 1)&&(flag==0))
		{           
			turn_on(D1);                                            
			turn_off(D2);	
			turn_off(D3);
			turn_off(D4);	
			flag=1;			
		} 
		else if((led_status ==2) &&(flag==0))
		{                                                     
			turn_on(D2);                                             
			 turn_off(D1);	
			turn_off(D3);
			turn_off(D4);
				flag=1;				
		} 
		else if((led_status == 3)&&(flag==0))
		{                                                      
				turn_on(D3);                                             
				turn_off(D1);	
				turn_off(D2);
				turn_off(D4);
				flag=1;				
		} 
		 if(led_status == 4)
		{		turn_off(D3);
				turn_off(D1);	
				turn_off(D2);
				turn_off(D4);
				flag=0;	
		} 
	} 
}
