#include "hard_finger.h"
#include "hard_usart.h"
#include <stdbool.h>
#include "stm32f10x_usart.h"
#include "cmd.h"
#include "stdlib.h"
extern Buf RecBuf;
extern Buf UART2Buf;

static uint8_t PageID[2] = {0x00, 0x00 };

static void Delaym(__IO uint32_t nCount);

//��ȡϵͳ����
uint8_t ReadSysPara[12] = { 0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, \
                            0x01, 0x00, 0x03, 0x0F, 0x00, 0x13};
//��ȡ��Чģ����
uint8_t templatenum[12] = { 0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, \
                            0x01, 0x00, 0x03, 0x1D, 0x00, 0x21};
// ���ģ��
uint8_t empty[12]       = { 0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, \
                            0x01, 0x00, 0x03, 0x0D, 0x00, 0x11};
// ָ�������Ա�
uint8_t search[17]      = { 0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, \
                            0x01, 0x00, 0x08, 0x04, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x0F};
// ��ȡָ��ͼ��
uint8_t genimg[12]      = { 0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, \
                            0x01, 0x00, 0x03, 0x01, 0x00, 0x05};
// ���������� buf1
uint8_t img2tzbuf1[13]  = { 0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, \
                            0x01, 0x00, 0x04, 0x02, 0x01, 0x00, 0x08};
// ���������� buf2
uint8_t img2tzbuf2[13]  = { 0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, \
                            0x01, 0x00, 0x04, 0x02, 0x02, 0x00, 0x09};
// �����ϳ�
uint8_t regmodel[12]    = { 0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, \
                            0x01, 0x00, 0x03, 0x05, 0x00, 0x09}; 
// �洢ģ��
uint8_t store[15]       = { 0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, \
                            0x01, 0x00, 0x06, 0x06, 0x01, 0x00, 0x00, 0x00, 0x0E};
// ģ��Ա�
uint8_t match[12]       = { 0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, \
                            0x01, 0x00, 0x03, 0x03, 0x00, 0x07};
  
                            

/*
** ��ȡͼ��
*/
static bool FinEmpty(void)
{
    uart_send_data(USART2, sizeof(empty), empty);
    while(1)
	{  
		if(UART2Buf.count > GINGER_ACK_LEN -1)
		{
            UART2Buf.count = 0;
            if(UART2Buf.buf[9] == 0x00)
            { 
                return true;
            }
            else
            { 
                return false;
            }                
		}  
	}  
}


/*
** ��ȡͼ��
*/
static bool FinGenImg(void)
{
    uart_send_data(USART2, sizeof(genimg), genimg);
    while(1)
	{  
        
		if(UART2Buf.count > GINGER_ACK_LEN -1)
		{
            Delaym(0x00000FFF);
            UART2Buf.count = 0;
            if(UART2Buf.buf[9] == 0x00)
            { 
                return true;
            }
            else
            { 
                return false;
            }                
		}  
	}  
}



/*
** �����ϳ�
*/
static bool FinImg2Tz(uint8_t * buf, uint8_t len)
{
    uart_send_data(USART2, len, buf);
    while(1)
	{  
		if(UART2Buf.count > GINGER_ACK_LEN -1)
		{
             Delaym(0x00000FFF);
            UART2Buf.count = 0;
            if(UART2Buf.buf[9] == 0x00)
            { 
                return true;
            }
            else
            { 
                return false;
            }
		}
	} 
}


/* 
** �����Ա�
*/
static bool FinMatch(void)
{
    uart_send_data(USART2, sizeof(match), match);
    while(1)
	{  
		if(UART2Buf.count > GINGER_ACK_LEN -1)
		{
            UART2Buf.count = 0;
            if(UART2Buf.buf[9] == 0x00)
            { 
                return true;
            }
            else
            { 
                return false;
            }                
		}
	}
}


/*
** �����ϳ�ģ��
*/
static bool FinRegModel(void)
{
    uart_send_data(USART2, sizeof(regmodel), regmodel);
    while(1)
	{  
		if(UART2Buf.count > GINGER_ACK_LEN -1)
		{
             Delaym(0x00000FFF);
            UART2Buf.count = 0;
            if(UART2Buf.buf[9] == 0x00)
            { 
                return true;
            }
            else
            { 
                return false;
            }                
		}
	}
}

/*
** �洢ģ���ļ�
*/
static bool FinStore(void)
{
    uart_send_data(USART2, sizeof(store), store);
    while(1)
	{  
		if(UART2Buf.count > GINGER_ACK_LEN -1)
		{
            Delaym(0x00000FFF);
            UART2Buf.count = 0;
            if(UART2Buf.buf[9] == 0x00)
            { 
                
                return true;
            }
            else
            { 
                return false;
            }                
		}
	}
}


/*
** ��ȡָ��ģ�����浽PageID��
*/
static bool FinTemplateNum(void)
{
    uart_send_data(USART2, sizeof(templatenum), templatenum);
    while(1)
	{  
		if(UART2Buf.count > GINGER_ACK_LEN -1)
		{
            Delaym(0x00000FFF);
            if(UART2Buf.buf[9] == 0x00)
            {
                
                UART2Buf.count = 0;
                PageID[0] = UART2Buf.buf[10];  // ��
                PageID[1] = UART2Buf.buf[11];  // ��
                return true;
            }
            else
            {
                return false;
            }                    
		}
	}
}

/*
** ָ�ƶԱ�
*/
static bool Search(void)
{
    uart_send_data(USART2, sizeof(search), search);
    while(1)
	{  
		if(UART2Buf.count > GINGER_ACK_LEN -1)
		{
            UART2Buf.count = 0;
            if(UART2Buf.buf[9] == 0x00)
            {  
                return true;
            }
            else
            { 
                return false;
            }                
		}
	}
}



/*
** ��ȡ��ģ�����,��ʾ������
*/
static void finger_num(void)
{
    uint8_t tmp[2];
	// 7. ��ȡ��ʹ��ģ�����,���ô洢λ�ú�
    if(FinTemplateNum() == true)
    {
        tmp[0] = PageID[0] + 0x30;   // 16����ת char
        tmp[1] = PageID[1] + 0x30;
        uart_send_data(USART1, 2, tmp);
    }
    else
    {
        uart_send_data(USART1, 11, (uint8_t *)"getNum err\n");
    }
    
    
}
/*
** ���´洢λ��
*/
static void UpdateStoreCMD(void)
{
    // д��ָ��
    store[11] = PageID[0];   
    store[12] = PageID[1];
    // У���  13  14
    store[13] = (uint8_t)(((0x000E + PageID[0] + PageID[1])&0xFF00)>>8);
    store[14] = (uint8_t)(((0x000E + PageID[0] + PageID[1])&0x00FF));
    // uart_send_data(USART2, sizeof(store), store);  
}


/*
** ¼��ָ��
*/
static void finger_in(void)
{   
RGET_IMG_1:
    // 1. ��ȡͼ��  0x00
    uart_send_data(USART1, 7, (uint8_t *)"Genimg\n");
    while(FinGenImg() == false);
    
    // 2. ��������  0x00
    if(FinImg2Tz(img2tzbuf1, 13) == false)
    {
        uart_send_data(USART1, 7, (uint8_t *)"Tz Err\n");
        goto RGET_IMG_1;
    }
    else
    {
         uart_send_data(USART1, 7, (uint8_t *)"Tz  Ok\n");
    }
   
    
RGET_IMG_2:
    // 3. ��ȡͼ��  0x00 
    uart_send_data(USART1, 7, (uint8_t *)"Genimg\n");
    while(FinGenImg() == false);
    
    // 4. ��������  0x00
    if(FinImg2Tz(img2tzbuf2, 13) == false)
    {
        uart_send_data(USART1, 7, (uint8_t *)"Tz Err\n");
        goto RGET_IMG_2;
    }
    else 
    {
         uart_send_data(USART1, 7, (uint8_t *)"Tz  Ok\n");
    }
    
    // 5. �����Ա�  0x00
    if(FinMatch() == false)
    {
       uart_send_data(USART1, 9, (uint8_t *)"Match Ok\n");
    }
    else
    {
        uart_send_data(USART1, 10, (uint8_t *)"Match Err\n");
    }
    
    // 6. �����ϳ�ģ�� 0x00 
    if(FinRegModel() == true)   
    {       
        uart_send_data(USART1, 12, (uint8_t *)"RegModel Ok\n");
    }
    else
    {
        uart_send_data(USART1, 13, (uint8_t *)"RegModel Err\n");
    }
    
    // 7. �鿴����ģ���� 0x00
     if(FinTemplateNum() == true)
    {
        uart_send_data(USART1, 10, (uint8_t *)"GetNum Ok\n");
    }
    else
    {
        uart_send_data(USART1, 11, (uint8_t *)"GetNum Err\n");
    }
    
    // 8. ���´洢ָ��
    UpdateStoreCMD();
    
    
    // 9. �洢¼���ģ�� 0x00
    if(FinStore() == true)
    {
       uart_send_data(USART1, 10, (uint8_t *)"Store Ok\n");
    }
    else
    {
         uart_send_data(USART1, 10, (uint8_t *)"Store Err\n");
    } 
}


/*
** ɾ��ָ��
*/
static void finger_empty(void)
{
	if(FinEmpty() == true)
    {
        uart_send_data(USART1, 9, (uint8_t *)"Empty Ok\n");
    }
    else
    {
        uart_send_data(USART1, 10, (uint8_t *)"Empty Err\n");
    }
}

/*
** ����ָ�ƶԱ�ָ��
*/
static void UpdateSearchCMD(void)
{
    // �޸ĶԱȸ��� 13 14
    search[13] = PageID[0];
    search[14] = PageID[1];
    // У���  15 16
    search[15] = (uint8_t)(((0x000E + PageID[0] + PageID[1])&0xFF00)>>8);
    search[16] = (uint8_t)(((0x000E + PageID[0] + PageID[1])&0x00FF));
    //uart_send_data(USART1, sizeof(search), search);
    
}


/*
** ָ�ƶԱ� 
*/
static void finger_confirm(void)
{
RGET_IMG_1: 
    // 1. ��ȡͼ��  0x00
    uart_send_data(USART1, 7, (uint8_t *)"Genimg\n");
    while(FinGenImg() == false);
    
    // 2. ��������  0x00
    if(FinImg2Tz(img2tzbuf1, 13) == false)
    {
        uart_send_data(USART1, 7, (uint8_t *)"Tz Err\n");
        goto RGET_IMG_1;
    }
    else
    {
         uart_send_data(USART1, 7, (uint8_t *)"Tz  Ok\n");
    }
    
    // 3. �鿴����ģ���� 0x00
     if(FinTemplateNum() == true)
    {
        uart_send_data(USART1, 10, (uint8_t *)"GetNum Ok\n");
    }
    else
    {
        uart_send_data(USART1, 11, (uint8_t *)"GetNum Err\n");
    }
    
    // 4. ���¶Ա�ָ��
    UpdateSearchCMD();
    
    
    // 5. ָ�������Ա� 0x00
    if(Search() == true)   // �����ϳ�ģ��
    {       
        uart_send_data(USART1, 10, (uint8_t *)"Search Ok\n");
    }
    else
    {
        uart_send_data(USART1, 11, (uint8_t *)"Search Err\n");
    }  
}


/* 
** ע��ָ��ģ��������� 
*/
void fin_cmd_init(void)
{
	CMD fin_cmds[] = {
		{"finger num", finger_num},
		{"finger in", finger_in},
		{"finger empty", finger_empty},
        {"finger confirm", finger_confirm}
	};
	
	/* ע������ */
	register_cmds(fin_cmds, 4);
}


/*
** ��ʱ����
*/
void Delaym(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}

