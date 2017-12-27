/******************************************************************************
* �ļ����ƣ� cmd.h
* ժ Ҫ��    ���������ͷ�ļ�
* ��ǰ�汾�� 1.0
* �� �ߣ�    Dwade
* ������ڣ� 2017��12��7��

******************************************************************************/
#ifndef __CMD_H_
#define __CMD_H_

#define MAX_CMD_NAME_LENGTH 30   // ��������
#define MAX_CMDS_COUNT      20   // ���������

typedef void (*handler)(void);   // �����������ָ������

/* ����ṹ������ */
typedef struct cmd
{
	char cmd_name[MAX_CMD_NAME_LENGTH + 1];  // ������
	handler cmd_operate;         //�����������
} CMD;


/* �����б�ṹ�� */
typedef struct cmds
{
	CMD cmds[MAX_CMDS_COUNT];    //�б�����
	int num;    // �б���
} CMDS;


/* �ⲿ�������� */
void register_cmds(CMD reg_cmds[], int num);
void match_cmd(char *str);


#endif

