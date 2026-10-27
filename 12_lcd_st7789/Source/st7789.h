#ifndef _ST7789_H_
#define _ST7789_H_

#include "stdio.h"
#include <stdint.h>
#include "stm32f4xx.h"
#include "fsmc.h"
#include "delay.h"


/*********************************************************************************************
* �궨��
*********************************************************************************************/
//lcd ʹ��PD7�� NE1���ܻ���ַΪ0x60000000,����A17 ��Ϊrsѡ��
#define RS_PIN          17
#define ST7789_REG      (*((volatile uint16_t *)(0x60000000)))
#define ST7789_DAT      (*((volatile uint16_t *)(0x60000000 | (1<<(RS_PIN+1)))))

#define write_command(x)   (ST7789_REG=(x))
#define write_data(x)      (ST7789_DAT=(x))
#define read_data()        ST7789_DAT

#define LCD_WIDE        240
#define LCD_HIGH        320


typedef struct  
{										    
	short wide;			  //����
	short high;			  //�߶�
	uint16_t id;        //ID
	uint8_t dir;			  //���������������ƣ�0��������1��������	
	uint8_t wramcmd;		//��ʼдgramָ��	
	uint8_t rramcmd;		//��ʼ��gramָ��
	uint8_t setxcmd;		//����x����ָ��
	uint8_t setycmd;		//����y����ָ��	 
}screen_dev_t; 	  

extern screen_dev_t screen_dev;

void st7789_PrepareWrite(void);
void st7789_SetCursorPos(short x,short y);
void st7789_PrepareFill(short x1, short y1, short x2, short y2);

uint8_t st7789_init(void);
void st7789_DrawPoint(short x,short y,uint32_t color);
uint32_t LCD_ReadPoint(short x,short y);
void st7789_FillColor(short x1,short y1,short x2,short y2,uint16_t color);
void st7789_FillData(short x1,short y1,short x2,short y2,unsigned short* dat);

#endif
