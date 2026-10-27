#ifndef _USART_H
#define _USART_H
#include "stm32f4xx.h"                  // Device header
#include "string.h"
#include "stdio.h"
extern unsigned char Usart_len;
void usart_init(unsigned int bound);
int fputc(int ch, FILE *f) ;
void usart_send(unsigned char *s,unsigned char len) ;
void clean_usart(void);
void usart_send_byte(unsigned char data);
void usart_send_string(char *str);
void usart_send_number(int num);
#endif
