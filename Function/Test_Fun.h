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



typedef struct TempCalData_tag{
	unsigned short tempvalue;
	double tempvalue2;
	short testline2[MaxPointLen];
	
	PeakData peakdata[10];													//����10����
	unsigned char peaknum;
	
	unsigned short MaxPoint;												//���������ĵ�
	unsigned short MaxPointNum;												//���������ĵ����Ŀ
	unsigned short MinPoint;												//��������С�ĵ�
	unsigned short MinPointNum;												//��������С�ĵ����Ŀ
	
	unsigned char errorcount;												//�������
	ResultState resultstatues;											//���Խ��״̬
}TempCalData;

MyState_TypeDef InitTestFunData(void);
ResultState TestFunction(void * parm);

MyState_TypeDef TakeTestPointData(void * data);
MyState_TypeDef TakeTestResultData(void * data);

#endif

