#ifndef __RECORD_P_H__
#define __RECORD_P_H__

#include	"MyTest_Data.h"



typedef struct Record_tag {
	unsigned char longpresscount;						//����ʱ��
	TestData testdata[DataNumInPage];					//��������
	unsigned char selectindex;							//ѡ�е�����
	unsigned char pageindex;							//ҳ������
	
	TestDataSaveHead saveddatahead;						//���ݱ�������
	
	unsigned int maxpagenum;

	char buf[300];										//��ʱ������
	unsigned int tempvalue1;
	unsigned int tempvalue2;
	TestData *tempdata;
	unsigned short lcdinput[100];
}RecordPageBuffer;

unsigned char DspRecordPage(void *  parm);

#endif

