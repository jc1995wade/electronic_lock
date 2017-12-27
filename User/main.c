/**********************窗帘控制模块实验 **************************/
/* Copyright (C) jchedewei@163.com 2017-12-22                   */
/* FILE NAME                : main.c                            */
/* DATE OF FIRST RELEASE    : 2017/12/22                        */
/* DESCRIPTION              : This is a curtain control program */
/****************************************************************/
#include "stdio.h"
#include "stdlib.h"
#include "string.h"


//STM硬件及驱动相关头文件
#include "stm32f10x.h"
#include "hard_led.h"
#include "cmd.h"
#include "hard_usart.h"
#include "hard_finger.h"

extern Buf RecBuf;  // USART1 接收缓冲区
extern Buf UART2Buf;
    

void Delay(__IO u32 nCount); 
/* 硬件初始化 */
void hardware_init(void)
{
    NVIC_Configuration();
	NVIC_UART2_Configuration();
	USART1_Config();
	USART2_Config();
	
	fin_cmd_init();
	uart_send_string((unsigned char *)"Usart init ok!\n");
}



/* 串口输入命令 */
// 输入: cur up/cur stop/sur down
// 实现控制窗帘开启停止和关闭
int main(void)
{
	hardware_init();
	u32 i = 0;
	char cmd[MAX_CMD_NAME_LENGTH] = {NULL};

	//uint8_t str[GINGER_CMD_LEN] =  newFingerCMD(GenImg);  // 初始化一个获取图像命令包
	
	
	while(1)
	{
#if 0
		if(UART2Buf.count > GINGER_CMD_LEN -1)
		{
			uart_send_data(USART2, UART2Buf.count, UART2Buf.buf);
			UART2Buf.count = 0;
		}
        else
        {
          //  uart2_send_string((unsigned char *)"null\n");
        }
		//uart_send_data(USART1, 16, str);
		//uart_send_data(USART2, 16, str);
    if(RecBuf.count > GINGER_CMD_LEN -1)
		{
			uart_send_data(USART1, RecBuf.count, RecBuf.buf);
			RecBuf.count = 0;
		}
        else
        {
            
        }
        
        
#endif
		
#if 1
		//清空cmd内容
		for(i=0; i<MAX_CMD_NAME_LENGTH; i++)
		{
			cmd[i] = '\0'; 
		}
		
		//uart_send_string((unsigned char *)"please input you cmd:\n");
		uart_get_string((unsigned char *)cmd);  // 串口接受数据
        
		/* 输入控制命令式 */
		/* 解析并执行对应命令函数 */
		match_cmd(cmd);
		//uart_send_string((unsigned char *)cmd);   // 命令回显
#endif
	}

}



void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}
/*********************************************END OF FILE**********************/






