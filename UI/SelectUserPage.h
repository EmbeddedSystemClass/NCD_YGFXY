#ifndef __SUSER_P_H__
#define __SUSER_P_H__

#include	"Define.h"
#include	"MyTest_Data.h"
#include	"Timer_Data.h"

#define		TextAddr1	0x1c10
#define		TextAddr2	0x1610
#define		TextAddr3	0x1620
#define		TextAddr4	0x1630
#define		TextAddr5	0x1640
#define		TextAddr6	0x1650


typedef struct UserPage_tag {
	unsigned char selectindex;										//ѡ�е�����
	unsigned char pageindex;										//ҳ������
	User_Type user[MaxUserNum];										//ѡ�е���
	User_Type * tempuser;
	ItemData * currenttestdata;										//��ǰ��������
	Timer timer;													//ҳ�泬ʱʱ��
}UserPageBuffer;

unsigned char DspSelectUserPage(void *  parm);

#endif

