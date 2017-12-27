#ifndef _HARD_FINGER_H_
#define _HARD_FINGER_H_
#include <stdbool.h>
#include "stm32f10x.h"



/*
** 指纹模块数据包定义
*/
#define print(err)  uart_get_string((uint8_t *)err) 

// 指令代码
#define GenImg   0x01   // 录指纹图像
#define Img2Tz   0x02   // 图像转特征
#define Match    0x03   // 特征对比
#define Serach   0x04   // 搜索指纹
#define RegModel 0x05   // 特征合成模版
#define Store    0x06   // 存储模版
#define LoadChar 0x07   // 读出模版
#define UpChar   0x08   // 上传特征
#define DownChar 0x09   // 下载特征
#define UpImage  0x0a   // 上传图像
#define DownImage 0x0b  // 下载图像
#define DeleteChar 0x0c //  删除模版

#define Empty        0x0d  // 清空指纹库
#define SetAddr      0x15  // 设置地址
#define TemplateNum  0x1d  // 读取指纹模版数

#define ACK_OK   0x00    // 执行  成功
#define ACK_ERR  0x01    // 执行  失败


#define GINGER_CMD_LEN 14   // 发送包长度
#define GINGER_ACK_LEN 12   // 接收包长度

#define BufferID1   0x01    // 特征存储 1
#define BufferID2   0x02    // 特征存储 2

// 线路连接
//  PA2  ---  RXD
//  PA3  ---  TXD

//  PA9  ---  RXD
//  PA10 ---  TXD

void fin_cmd_init(void);
//void FP_send_cmd(void);




#endif

