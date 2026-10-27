/*********************************************************************************************
* 文件: usart.c
* 说明: USART1串口驱动 - PA9(TX), PA10(RX)
*********************************************************************************************/
#include "usart.h"
#include "delay.h"
#include <stdio.h>
#include <string.h>

#define USART_REC_MAX 256
unsigned char Usart_len = 0;                                  
unsigned char USART_RX_BUF[USART_REC_MAX];              

void usart_init(unsigned int bound)
{ 
    GPIO_InitTypeDef GPIO_InitStructure; 
    USART_InitTypeDef USART_InitStructure; 
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);
    
    USART_Cmd(USART1, ENABLE);
}

int fputc(int ch, FILE *f) 
{   
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART_SendData(USART1, (uint8_t)ch);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
    return ch; 
}

void usart_send_byte(unsigned char data)
{
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART_SendData(USART1, data);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
}

void usart_send_string(char *str)
{
    while(*str) {
        usart_send_byte(*str++);
    }
}

void usart_send_number(int num)
{
    char buf[12];
    int i = 0;
    int neg = 0;
    
    if(num < 0) {
        neg = 1;
        num = -num;
    }
    
    if(num == 0) {
        usart_send_byte('0');
        return;
    }
    
    while(num > 0) {
        buf[i++] = '0' + (num % 10);
        num /= 10;
    }
    
    if(neg) usart_send_byte('-');
    
    while(i > 0) {
        usart_send_byte(buf[--i]);
    }
}

void clean_usart(void) 
{ 
    memset(USART_RX_BUF, 0, Usart_len); 
    Usart_len = 0; 
}
