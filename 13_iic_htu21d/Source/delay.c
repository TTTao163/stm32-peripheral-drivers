/*********************************************************************************************
* �ļ�: delay.c
* ���ߣ�zonesion 2016.12.22
* ˵������ʱ��غ���  
* �޸ģ�
* ע�ͣ����õ�contiki����ϵͳʱ�轫SYSTEM_SUPPORT_CONTIKI��1����delay.h�ļ���
*********************************************************************************************/
#include "delay.h"
 
static unsigned char  fac_us=0;                                 //us��ʱ������			   
static unsigned short fac_ms=0;                                 //ms��ʱ������
/*********************************************************************************************
* ���ƣ�delay_init
* ���ܣ���ʼ����ʱ����
* ������SYSCLK��ϵͳʱ��Ƶ�ʣ���λMHz��
* ���أ���
* ������ʹ��systick��ʱ����Ϊ��ʱ��ʱ��
*       stm32f407systick��ʱ������Ƶ��ΪHCLK/8
*       HCKL=SYSCLK
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void delay_init(unsigned char SYSCLK)
{
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
  fac_us=SYSCLK/8;	             	
  fac_ms=(unsigned short)fac_us*1000;
}								    


/*********************************************************************************************
* ���ƣ�delay_us
* ���ܣ�us����ʱ����
* ������nus����ʱʱ������λus
* ���أ���
* ������fac_us=SYSCLK/8;���������乤��Ƶ�ʣ���λMHz,SYSCLKȡֵ168MHz��fac_us=21
*       ��ô��ʱ����ʱ����fac_us�Σ���ʱ1΢�롣nus*fac_us�Σ�����ʱnus΢��
*       systick��ʱ��Ϊ24λ������������Ϊ0xFFFFFF = MAXus * fac_us�����Եõ������ʱʱ��MAX = 798915΢��
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void delay_us(unsigned int nus){	
  unsigned int temp;	    	 
  SysTick->LOAD=nus*fac_us;                                     //ʱ�����	  		 
  SysTick->VAL=0x00;                                            //��ռ�����
  SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;                      //��ʼ���� 
  do{
      temp=SysTick->CTRL;                                       //ȡCTRL(Control and Status Register)ֵ
    }
  while((temp&0x01)&&!(temp&(1<<16)));                          //�ȴ�ʱ�䵽��   
  SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;                      //�رռ�����
  SysTick->VAL =0X00;                                           //��ռ�����	 
}

/*********************************************************************************************
* ���ƣ�delay_xms
* ���ܣ�ms����ʱ����
* ������nms����ʱʱ������λms
* ���أ���
* ������fac_ms = 10*fac_us����λMHz,SYSCLKȡֵ168MHz��fac_ms=21000
*       ��ô��ʱ����ʱ����fac_ms�Σ���ʱ1���롣nms*fac_ms�Σ�����ʱnms����
*       systick��ʱ��Ϊ24λ������������Ϊ0xFFFFFF = MAXms * fac_ms�����Եõ������ʱʱ��MAX = 798����(ȡ��)
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void delay_xms(unsigned short nms){	 		  	  
  unsigned int temp;		   
  SysTick->LOAD=(unsigned int)nms*fac_ms;                       //ʱ�����(SysTick->LOADΪ24bit)
  SysTick->VAL =0x00;                                           //��ռ�����
  SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;                      //��ʼ����  
  do{
     temp=SysTick->CTRL;                                        //ȡCTRL(Control and Status Register)ֵ  
    }
  while((temp&0x01)&&!(temp&(1<<16)));                          //�ȴ�ʱ�䵽��   
  SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;                      //�رռ�����
  SysTick->VAL =0X00;                                           //��ռ�����	  	    
} 
/*********************************************************************************************
* ���ƣ�delay_ms
* ���ܣ�ms����ʱ����
* ������nms����ʱʱ������λms
* ���أ���
* ������delay_xms��ʱ����ʱ�޵ģ���stm32F407��Ƶ��248M��ʱ��delay_xmsֻ����ʱ541ms����
*       delay_ms��ʱ��������ʱʱ��ָ�����ɸ�delay_xms�������Դﵽ����ʱʱ����������ʱ����ʱʱ�����65535ms
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
#define XMS_MAX 540                                             //delay_xms����ʱ���ʱ�䣨��λms��
void delay_ms(unsigned short nms){	 	 
   unsigned char repeat=nms/XMS_MAX;	                        //�ָ����    
   unsigned short remain=nms%XMS_MAX;                           //�ָ�ʣ��
   while(repeat)
     {
	delay_xms(XMS_MAX);
	repeat--;
     }
   if(remain)
     delay_xms(remain);	
}
 