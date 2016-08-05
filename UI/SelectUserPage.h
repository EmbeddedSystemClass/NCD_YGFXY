#ifndef __SUSER_P_H__
#define __SUSER_P_H__

#include	"Define.h"
#include	"MyTest_Data.h"
#include	"Timer_Data.h"

typedef struct UserPage_tag {
	unsigned char selectindex;										//ѡ�е�����
	unsigned char pageindex;										//ҳ������
	User_Type user[MaxUserNum];										//ѡ�е���
	User_Type * tempuser;
	ItemData * currenttestdata;										//��ǰ��������
	Timer timer;													//ҳ�泬ʱʱ��
	unsigned short lcdinput[100];									//lcd�������
}UserPageBuffer;

unsigned char DspSelectUserPage(void *  parm);

#endif

