#ifndef __RECORD_P_H__
#define __RECORD_P_H__

#include	"MyTest_Data.h"



typedef struct Record_tag {
	unsigned char selectindex;							//ѡ�е�����
	unsigned char pageindex;							//ҳ������
	char filename[30];									//��ǰѡ��鿴���ļ�����
	MyTime_Def time;									//
	unsigned char longpresscount;						//����ʱ��
	TestData testdata[DataNumInPage];					//��������
	char buf[300];										//��ʱ������
	unsigned short tempvalue1;
	unsigned short tempvalue2;
	TestData *tempdata;
}RecordPageBuffer;

unsigned char DspRecordPage(void *  parm);

#endif

