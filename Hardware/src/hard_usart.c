/**
  ******************************************************************************
  * @file    bsp_usart1.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   ����c��printf������usart�˿�
  ******************************************************************************
  */ 
  
#include "hard_usart.h"
#include "stm32f10x_usart.h"
#include "misc.h"

// ���ڻ�����
Buf RecBuf   = { 0, 0};
Buf UART2Buf = { 0, 0};



 /**
  * @brief  USART1 GPIO ����,����ģʽ���á�115200 8-N-1
  * @param  ��
  * @retval ��
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
		
		/* ʹ�ܴ���1�����ж� */
	   // USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
		
		USART_Cmd(USART1, ENABLE);
		
}

/// ����USART1�����ж�
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
		
		/* ʹ�ܴ���2�����ж� */
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
* �������ܣ� ���ڽ����ַ�������
* ��������� ��
* ��������� str �ѽ��յ��ַ���
* ����ֵ ��  �� 
* ��    ע�� ������ \r\n Ϊ�������ַ��������ڹ����蹴ѡ���������С�ѡ��   
******************************************************************************/
void uart_get_string(unsigned char *str)
{
	unsigned char ch;
	/* �ȴ�����1�������� */
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
	ch = USART_ReceiveData(USART1);

	while ((ch != '\r') && (ch != '\n'))  // ����ʱ�Իس����н���
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
	/* �ȴ�����1�������� */
	while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);
	ch = USART_ReceiveData(USART2);

	while ((ch != '\r') && (ch != '\n'))  // ����ʱ�Իس����н���
	{
	 	*str++ = ch;
		while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);
		ch = USART_ReceiveData(USART2);
	}

	*str = '\0';
}



#if 0

///�ض���c�⺯��printf��USART1
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ�USART1 */
		USART_SendData(USART1, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///�ض���c�⺯��scanf��USART1
int fgetc(FILE *f)
{
		/* �ȴ�����1�������� */
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART1);
}

#endif

/*********************************************END OF FILE**********************/
