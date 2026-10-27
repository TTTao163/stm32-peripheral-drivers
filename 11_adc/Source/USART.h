#ifndef _USART_H
#define _USART_H

#include "stm32f4xx.h"                  // Device header
#include "string.h"
#include "stdio.h"

#define USART_REC_MAX 256
extern volatile uint8_t usart_rx_done;
extern unsigned char Usart_len;
extern unsigned char USART_RX_BUF[USART_REC_MAX];
void usart_init(unsigned int bound);
int fputc(int ch, FILE *f) ;
void usart_send(unsigned char *s,unsigned char len) ;
void usart_send_float(float f);
void clean_usart(void);

#endif
