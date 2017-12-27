/**
  ******************************************************************************
  * @file    bsp_usart1.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   重现c库printf函数到usart端口
  ******************************************************************************
  */ 
  
#include "hard_usart.h"
#include "stm32f10x_usart.h"
#include "misc.h"

// 串口缓冲区
Buf RecBuf   = { 0, 0};
Buf UART2Buf = { 0, 0};



 /**
  * @brief  USART1 GPIO 配置,工作模式配置。115200 8-N-1
  * @param  无
  * @retval 无
  */
void USART1_Config(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
	
		/* config USART1 clock */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
		
		/* USART1 GPIO config */
		/* Configure USART1 Tx (PA.09) as alternate function push-pull */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	
		/* Configure USART1 Rx (PA.10) as input floating */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
			
		/* USART1 mode config */
		USART_InitStructure.USART_BaudRate = 115200;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No ;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_Init(USART1, &USART_InitStructure); 
		
		/* 使能串口1接收中断 */
	   // USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
		
		USART_Cmd(USART1, ENABLE);
		
}

/// 配置USART1接收中断
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void NVIC_UART2_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}




void USART2_Config(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		
		/* config USART2 clock */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); 
		/* USART2 GPIO config */
		/* Configure USART1 Tx (PA.02) as alternate function push-pull */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	
		/* Configure USART1 Rx (PA.03) as input floating */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
			
		/* USART2 mode config */
		USART_InitStructure.USART_BaudRate = 115200;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No ;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_Init(USART2, &USART_InitStructure); 
		
		/* 使能串口2接收中断 */
	    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
		
		USART_Cmd(USART2, ENABLE);
}





void uart_send_string(unsigned char *str)
{
	while (*str != '\0')
	{
		//*str = *str<<1;
		//*str = *str>>1;
	 	USART_SendData(USART1, (uint8_t) *str); // 0111 1111
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
		str++;
	}
}


void uart_send_data(USART_TypeDef * uart,uint8_t len, uint8_t *str)
{
	uint8_t i;
	for(i=0; i<len; i++ )
	{
		USART_SendData(uart, (uint8_t) *str); // 0111 1111
		while (USART_GetFlagStatus(uart, USART_FLAG_TXE) == RESET);
		str++;
	}
}


void uart2_send_string(unsigned char *str)
{
	while (*str != '\0')
	{
		//*str = *str<<1;
		//*str = *str>>1;
	 	USART_SendData(USART2, (uint8_t) *str); // 0111 1111
		while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
		str++;
	}
}


/******************************************************************************
* 函数介绍： 串口接收字符串函数
* 输入参数： 无
* 输出参数： str 已接收的字符串
* 返回值 ：  无 
* 备    注： 接收以 \r\n 为结束的字符串，串口工具需勾选“发送新行”选项   
******************************************************************************/
void uart_get_string(unsigned char *str)
{
	unsigned char ch;
	/* 等待串口1输入数据 */
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
	ch = USART_ReceiveData(USART1);

	while ((ch != '\r') && (ch != '\n'))  // 发送时以回车换行结束
	{
	 	*str++ = ch;
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
		ch = USART_ReceiveData(USART1);
	}

	*str = '\0';
}


void uart2_get_string(unsigned char *str)
{
	unsigned char ch;
	/* 等待串口1输入数据 */
	while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);
	ch = USART_ReceiveData(USART2);

	while ((ch != '\r') && (ch != '\n'))  // 发送时以回车换行结束
	{
	 	*str++ = ch;
		while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);
		ch = USART_ReceiveData(USART2);
	}

	*str = '\0';
}



#if 0

///重定向c库函数printf到USART1
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到USART1 */
		USART_SendData(USART1, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///重定向c库函数scanf到USART1
int fgetc(FILE *f)
{
		/* 等待串口1输入数据 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART1);
}

#endif

/*********************************************END OF FILE**********************/
