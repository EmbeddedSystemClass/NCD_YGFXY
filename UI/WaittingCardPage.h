#ifndef __WAITCARD_P_H__
#define __WAITCARD_P_H__

#include	"Timer_Data.h"
#include	"MyTest_Data.h"

typedef struct WaitPage_tag {
	Timer timer;				//�ȴ��忨ʱ��
	Timer timer2;				//�����ʾ�忨
	ItemData * currenttestdata;
}WaitPageData;

unsigned char DspWaittingCardPage(void *  parm);

#endif

