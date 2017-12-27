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

//读取系统参数
uint8_t ReadSysPara[12] = { 0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, \
                            0x01, 0x00, 0x03, 0x0F, 0x00, 0x13};
//读取有效模个数
uint8_t templatenum[12] = { 0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, \
                            0x01, 0x00, 0x03, 0x1D, 0x00, 0x21};
// 清空模板
uint8_t empty[12]       = { 0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, \
                            0x01, 0x00, 0x03, 0x0D, 0x00, 0x11};
// 指纹特征对比
uint8_t search[17]      = { 0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, \
                            0x01, 0x00, 0x08, 0x04, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x0F};
// 获取指纹图像
uint8_t genimg[12]      = { 0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, \
                            0x01, 0x00, 0x03, 0x01, 0x00, 0x05};
// 生成特征到 buf1
uint8_t img2tzbuf1[13]  = { 0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, \
                            0x01, 0x00, 0x04, 0x02, 0x01, 0x00, 0x08};
// 生成特征到 buf2
uint8_t img2tzbuf2[13]  = { 0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, \
                            0x01, 0x00, 0x04, 0x02, 0x02, 0x00, 0x09};
// 特征合成
uint8_t regmodel[12]    = { 0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, \
                            0x01, 0x00, 0x03, 0x05, 0x00, 0x09}; 
// 存储模板
uint8_t store[15]       = { 0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, \
                            0x01, 0x00, 0x06, 0x06, 0x01, 0x00, 0x00, 0x00, 0x0E};
// 模板对比
uint8_t match[12]       = { 0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, \
                            0x01, 0x00, 0x03, 0x03, 0x00, 0x07};
  
                            

/*
** 获取图像
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
** 获取图像
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
** 特征合成
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
** 特征对比
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
** 特征合成模板
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
** 存储模版文件
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
** 获取指纹模板数存到PageID中
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
                PageID[0] = UART2Buf.buf[10];  // 高
                PageID[1] = UART2Buf.buf[11];  // 底
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
** 指纹对比
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
** 获取已模版个数,显示到串口
*/
static void finger_num(void)
{
    uint8_t tmp[2];
	// 7. 获取已使用模板个数,设置存储位置号
    if(FinTemplateNum() == true)
    {
        tmp[0] = PageID[0] + 0x30;   // 16进制转 char
        tmp[1] = PageID[1] + 0x30;
        uart_send_data(USART1, 2, tmp);
    }
    else
    {
        uart_send_data(USART1, 11, (uint8_t *)"getNum err\n");
    }
    
    
}
/*
** 更新存储位置
*/
static void UpdateStoreCMD(void)
{
    // 写入指令
    store[11] = PageID[0];   
    store[12] = PageID[1];
    // 校验和  13  14
    store[13] = (uint8_t)(((0x000E + PageID[0] + PageID[1])&0xFF00)>>8);
    store[14] = (uint8_t)(((0x000E + PageID[0] + PageID[1])&0x00FF));
    // uart_send_data(USART2, sizeof(store), store);  
}


/*
** 录入指纹
*/
static void finger_in(void)
{   
RGET_IMG_1:
    // 1. 获取图像  0x00
    uart_send_data(USART1, 7, (uint8_t *)"Genimg\n");
    while(FinGenImg() == false);
    
    // 2. 生成特征  0x00
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
    // 3. 获取图像  0x00 
    uart_send_data(USART1, 7, (uint8_t *)"Genimg\n");
    while(FinGenImg() == false);
    
    // 4. 生成特征  0x00
    if(FinImg2Tz(img2tzbuf2, 13) == false)
    {
        uart_send_data(USART1, 7, (uint8_t *)"Tz Err\n");
        goto RGET_IMG_2;
    }
    else 
    {
         uart_send_data(USART1, 7, (uint8_t *)"Tz  Ok\n");
    }
    
    // 5. 特征对比  0x00
    if(FinMatch() == false)
    {
       uart_send_data(USART1, 9, (uint8_t *)"Match Ok\n");
    }
    else
    {
        uart_send_data(USART1, 10, (uint8_t *)"Match Err\n");
    }
    
    // 6. 特征合成模版 0x00 
    if(FinRegModel() == true)   
    {       
        uart_send_data(USART1, 12, (uint8_t *)"RegModel Ok\n");
    }
    else
    {
        uart_send_data(USART1, 13, (uint8_t *)"RegModel Err\n");
    }
    
    // 7. 查看已用模板数 0x00
     if(FinTemplateNum() == true)
    {
        uart_send_data(USART1, 10, (uint8_t *)"GetNum Ok\n");
    }
    else
    {
        uart_send_data(USART1, 11, (uint8_t *)"GetNum Err\n");
    }
    
    // 8. 更新存储指令
    UpdateStoreCMD();
    
    
    // 9. 存储录入的模板 0x00
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
** 删除指纹
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
** 更新指纹对比指令
*/
static void UpdateSearchCMD(void)
{
    // 修改对比个数 13 14
    search[13] = PageID[0];
    search[14] = PageID[1];
    // 校验和  15 16
    search[15] = (uint8_t)(((0x000E + PageID[0] + PageID[1])&0xFF00)>>8);
    search[16] = (uint8_t)(((0x000E + PageID[0] + PageID[1])&0x00FF));
    //uart_send_data(USART1, sizeof(search), search);
    
}


/*
** 指纹对比 
*/
static void finger_confirm(void)
{
RGET_IMG_1: 
    // 1. 获取图像  0x00
    uart_send_data(USART1, 7, (uint8_t *)"Genimg\n");
    while(FinGenImg() == false);
    
    // 2. 生成特征  0x00
    if(FinImg2Tz(img2tzbuf1, 13) == false)
    {
        uart_send_data(USART1, 7, (uint8_t *)"Tz Err\n");
        goto RGET_IMG_1;
    }
    else
    {
         uart_send_data(USART1, 7, (uint8_t *)"Tz  Ok\n");
    }
    
    // 3. 查看已用模板数 0x00
     if(FinTemplateNum() == true)
    {
        uart_send_data(USART1, 10, (uint8_t *)"GetNum Ok\n");
    }
    else
    {
        uart_send_data(USART1, 11, (uint8_t *)"GetNum Err\n");
    }
    
    // 4. 更新对比指令
    UpdateSearchCMD();
    
    
    // 5. 指纹特征对比 0x00
    if(Search() == true)   // 特征合成模板
    {       
        uart_send_data(USART1, 10, (uint8_t *)"Search Ok\n");
    }
    else
    {
        uart_send_data(USART1, 11, (uint8_t *)"Search Err\n");
    }  
}


/* 
** 注册指纹模块控制命令 
*/
void fin_cmd_init(void)
{
	CMD fin_cmds[] = {
		{"finger num", finger_num},
		{"finger in", finger_in},
		{"finger empty", finger_empty},
        {"finger confirm", finger_confirm}
	};
	
	/* 注册命令 */
	register_cmds(fin_cmds, 4);
}


/*
** 延时函数
*/
void Delaym(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}

