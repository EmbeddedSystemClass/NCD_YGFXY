#ifndef __SampleID_P_H__
#define __SampleID_P_H__

#include	"Define.h"
#include	"Timer_Data.h"
#include	"MyTest_Data.h"

typedef struct SampleIDPage_tag {
	char tempid[MaxSampleIDLen];				//��ʱ����������
	char *tempbuf;								//��ʱָ��
	Timer timer;				//һ����ʱ��
	ItemData * currenttestdata;												//��ǰ��������
}SampleIDPage;

unsigned char DspSampleIDPage(void *  parm);

#endif

