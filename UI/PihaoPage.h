#ifndef __PIHAO_P_H__
#define __PIHAO_P_H__

#include	"Define.h"
#include	"Timer_Data.h"
#include	"MyTest_Data.h"

typedef struct PihaoPage_tag {
	Timer timer;															//һ����ʱ��
	unsigned char tempbuf[150];
	unsigned int tempv1;
	ItemData * currenttestdata;												//��ǰ��������
	unsigned short lcdinput[100];
}PihaoPage;

unsigned char DspPihaoPage(void *  parm);

#endif
