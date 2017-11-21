#ifndef __TESTF_H__
#define __TESTF_H__

#include	"Define.h"
#include 	"FreeRTOS.h"
#include	"MyTest_Data.h"

#define	DataMaxWinSize			20


#pragma pack(1)
typedef struct TempCalData_tag{
	double tempvalue1;
	double tempvalue2;
	double average;														//ƽ��ֵ
	double stdev;														//��׼��
	double tempCV;
	double CV_T;															//����ϵ��1
	double CV_C;															//����ϵ��1
	double CV_0;															//����ϵ��1
	unsigned short tempvalue3;
	unsigned short tempSeries[MaxPointLen+FilterNum];
	unsigned int motorLocation;														//��ǰ���λ��
	char tempBuf[100];
	unsigned short maxdata;
	ItemData * itemData;												//��������ָ��
	ResultState resultstatues;											//���Խ��״̬
}TempCalData;
#pragma pack()

MyState_TypeDef InitTestFunData(void);
ResultState TestFunction(void * parm);

MyState_TypeDef TakeTestPointData(void * data);
MyState_TypeDef TakeTestResultData(void * data);

#endif

