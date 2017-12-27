#ifndef _HARD_FINGER_H_
#define _HARD_FINGER_H_
#include <stdbool.h>
#include "stm32f10x.h"



/*
** ָ��ģ�����ݰ�����
*/
#define print(err)  uart_get_string((uint8_t *)err) 

// ָ�����
#define GenImg   0x01   // ¼ָ��ͼ��
#define Img2Tz   0x02   // ͼ��ת����
#define Match    0x03   // �����Ա�
#define Serach   0x04   // ����ָ��
#define RegModel 0x05   // �����ϳ�ģ��
#define Store    0x06   // �洢ģ��
#define LoadChar 0x07   // ����ģ��
#define UpChar   0x08   // �ϴ�����
#define DownChar 0x09   // ��������
#define UpImage  0x0a   // �ϴ�ͼ��
#define DownImage 0x0b  // ����ͼ��
#define DeleteChar 0x0c //  ɾ��ģ��

#define Empty        0x0d  // ���ָ�ƿ�
#define SetAddr      0x15  // ���õ�ַ
#define TemplateNum  0x1d  // ��ȡָ��ģ����

#define ACK_OK   0x00    // ִ��  �ɹ�
#define ACK_ERR  0x01    // ִ��  ʧ��


#define GINGER_CMD_LEN 14   // ���Ͱ�����
#define GINGER_ACK_LEN 12   // ���հ�����

#define BufferID1   0x01    // �����洢 1
#define BufferID2   0x02    // �����洢 2

// ��·����
//  PA2  ---  RXD
//  PA3  ---  TXD

//  PA9  ---  RXD
//  PA10 ---  TXD

void fin_cmd_init(void);
//void FP_send_cmd(void);




#endif

