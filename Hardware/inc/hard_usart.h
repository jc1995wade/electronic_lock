#ifndef __HARD_USART1_H
#define	__HARD_USART1_H

#include "stm32f10x.h"

#include <stdio.h>

//串口缓冲区定义
#define  MAX_REC_BUF  1024
	
typedef struct buf{
	uint16_t count;
	uint8_t buf[MAX_REC_BUF];
	
} Buf;



void NVIC_Configuration(void);
void USART1_Config(void);

void uart_send_string(unsigned char *str);
void uart_get_string(unsigned char *str);
void uart_send_data(USART_TypeDef * uart,uint8_t len, uint8_t *str); //发送指定长度数据


void USART2_Config(void);

void uart2_send_string(unsigned char *str);
void uart2_get_string(unsigned char *str);
void NVIC_UART2_Configuration(void);
#endif /* __USART1_H */
