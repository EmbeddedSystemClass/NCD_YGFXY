#ifndef __RECORD_P_H__
#define __RECORD_P_H__

#include	"MyTest_Data.h"



typedef struct Record_tag {
	unsigned char longpresscount;						//����ʱ��
	TestData testdata[DataNumInPage];					//��������
	unsigned char selectindex;							//ѡ�е�����
	unsigned char pageindex;							//ҳ������
	
	unsigned int maxdatanum;
	unsigned int maxpagenum;

	char buf[300];										//��ʱ������
	char buf2[100];
	unsigned int tempvalue1;
	unsigned int tempvalue2;
	TestData *tempdata;
	unsigned short lcdinput[100];
}RecordPageBuffer;

unsigned char DspRecordPage(void *  parm);

#endif

