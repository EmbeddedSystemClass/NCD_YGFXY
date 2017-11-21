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
	double average;														//平均值
	double stdev;														//标准差
	double tempCV;
	double CV_T;															//变异系数1
	double CV_C;															//变异系数1
	double CV_0;															//变异系数1
	unsigned short tempvalue3;
	unsigned short tempSeries[MaxPointLen+FilterNum];
	unsigned int motorLocation;														//当前电机位置
	char tempBuf[100];
	unsigned short maxdata;
	ItemData * itemData;												//测试数据指针
	ResultState resultstatues;											//测试结果状态
}TempCalData;
#pragma pack()

MyState_TypeDef InitTestFunData(void);
ResultState TestFunction(void * parm);

MyState_TypeDef TakeTestPointData(void * data);
MyState_TypeDef TakeTestResultData(void * data);

#endif

