#ifndef __TESTF_H__
#define __TESTF_H__

#include	"Define.h"
#include 	"FreeRTOS.h"
#include	"MyTest_Data.h"

#define	DataMaxWinSize			20

typedef struct PointData_tag {
	unsigned short data;
	unsigned short index;
}PointData;

#pragma pack(1)
typedef struct PeakData_tag {
	unsigned short StartLocation;											//���λ��
	unsigned short StartValue;												//���ֵ
	unsigned short PeakLocation;											//����λ��
	unsigned short PeakValue;												//����ֵ
	unsigned short EndLocation;												//����λ��
	unsigned short EndValue;												//����ֵ
	unsigned short PeakWidth;												//����
	unsigned short UpWidth;													//���������
	unsigned short DownWidth;												//���½����
	float PeakScale;														//���������ֵ
	unsigned char step;														//�ҷ岽��
}PeakData;
#pragma pack()

#pragma pack(1)
typedef struct TempCalData_tag{
	unsigned short tempvalue;
	double tempvalue2;
	short testline2[MaxPointLen];
	double lastdata;														//��¼�ϴ��˲�����
	
	PeakData peakdata[10];													//����10����
	unsigned char peaknum;
	
	unsigned short maxdata;
	ResultState resultstatues;											//���Խ��״̬
}TempCalData;
#pragma pack()

MyState_TypeDef InitTestFunData(void);
ResultState TestFunction(void * parm);

MyState_TypeDef TakeTestPointData(void * data);
MyState_TypeDef TakeTestResultData(void * data);

#endif

