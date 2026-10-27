/*********************************************************************************************
* 文件: iic.h
* 说明: 软件I2C驱动头文件
*********************************************************************************************/
#ifndef _IIC_H_
#define _IIC_H_

#include "stm32f4xx.h"

/* I2C GPIO配置 - 使用PA1(SCL)和PA0(SDA) */
#define I2C_GPIO        GPIOA
#define PIN_SCL         GPIO_Pin_1
#define PIN_SDA         GPIO_Pin_0

/* SDA读取宏 */
#define SDA_R           GPIO_ReadInputDataBit(I2C_GPIO, PIN_SDA)

/* 函数声明 */
void iic_init(void);
void iic_start(void);
void iic_stop(void);
void iic_send_ack(char ack);
unsigned char iic_recv_ack(void);
unsigned char iic_write_byte(unsigned char data);
unsigned char iic_read_byte(unsigned char ack);

#endif
