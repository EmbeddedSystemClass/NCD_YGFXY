/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"TestPage.h"
#include	"Define.h"
#include	"LCD_Driver.h"

#include	"MyMem.h"
#include	"WaittingCardPage.h"
#include	"MyTest_Data.h"
#include	"PaiDuiPage.h"
#include	"CardLimit_Driver.h"

#include	"System_Data.h"
#include	"TestDataDao.h"
#include	"SystemSet_Dao.h"
#include	"Test_Task.h"
#include	"LunchPage.h"
#include	"Printf_Fun.h"
#include	"Motor_Fun.h"
#include	"Motor_Data.h"
#include	"CardStatues_Data.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"

/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static TestPageBuffer * S_TestPageBuffer = NULL;
const unsigned int TestLineHigh = 76500;	//��������������ʾ����߶��йأ�������治�ģ���������
/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static void InitCurve(void);
static void RefreshCurve(void);
static void AddDataToLine(unsigned short data);
static void InitPageText(void);
static void RefreshPageText(void);

static void activityStart(void);
static void activityInput(unsigned char *pbuf , unsigned short len);
static void activityFresh(void);
static void activityHide(void);
static void activityResume(void);
static void activityDestroy(void);
static MyState_TypeDef activityBufferMalloc(void);
static void activityBufferFree(void);
static void printfTestData(void);
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/

/***************************************************************************************************
*FunctionName: createSelectUserActivity
*Description: ����ѡ������˽���
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:00:09
***************************************************************************************************/
MyState_TypeDef createTestActivity(Activity * thizActivity, Intent * pram)
{
	if(NULL == thizActivity)
		return My_Fail;
	
	if(My_Pass == activityBufferMalloc())
	{
		InitActivity(thizActivity, "TestActivity\0", activityStart, activityInput, activityFresh, activityHide, activityResume, activityDestroy);
		
		return My_Pass;
	}
	
	return My_Fail;
}

/***************************************************************************************************
*FunctionName: activityStart
*Description: ��ʾ������
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:00:32
***************************************************************************************************/
static void activityStart(void)
{
	if(S_TestPageBuffer)
	{
		/*��ȡ��ǰ�������ݵĵ�ַ*/
		S_TestPageBuffer->currenttestdata = GetCurrentTestItem();
		S_TestPageBuffer->currenttestdata->statues = status_test;
		
		InitCurve();
		
		//�����ڻ�ȡ��ǰ�������ݵ�ַ��ʹ��
		InitPageText();
		
		StartTest(S_TestPageBuffer->currenttestdata);
	}
	
	SelectPage(96);

}

/***************************************************************************************************
*FunctionName: activityInput
*Description: ��������
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:00:59
***************************************************************************************************/
static void activityInput(unsigned char *pbuf , unsigned short len)
{
	if(S_TestPageBuffer)
	{
		/*����*/
		S_TestPageBuffer->lcdinput[0] = pbuf[4];
		S_TestPageBuffer->lcdinput[0] = (S_TestPageBuffer->lcdinput[0]<<8) + pbuf[5];
		
		/*�˳�*/
		if(0x1801 == S_TestPageBuffer->lcdinput[0])
		{
			if(S_TestPageBuffer->currenttestdata->testdata.testResultDesc != NoResult)
			{
				//ɾ����ǰ����
				if(S_TestPageBuffer->currenttestdata)
				{
					DeleteCurrentTest();
					S_TestPageBuffer->currenttestdata = NULL;
				}
				
				backToActivity(lunchActivityName);
				
				if(IsPaiDuiTestting())
					startActivity(createPaiDuiActivity, NULL);					
			}
			//���ڲ��Բ������˳�
			else
				SendKeyCode(4);
		}
		/*��ӡ����*/
		else if(0x1800 == S_TestPageBuffer->lcdinput[0])
		{
			printfTestData();
		}
	}
}

/***************************************************************************************************
*FunctionName: activityFresh
*Description: ����ˢ��
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:01:16
***************************************************************************************************/
static void activityFresh(void)
{
	if(S_TestPageBuffer)
	{
		if(S_TestPageBuffer->currenttestdata->testdata.testResultDesc == NoResult)
			RefreshCurve();
		else
		{
			//�����ӡ��ϣ��ҿ��γ������˳�
			if((S_TestPageBuffer->isPrintfData == 0) &&(MaxLocation == GetGB_MotorLocation()))
			{
				//ɾ����ǰ����
				if(S_TestPageBuffer->currenttestdata)
				{
					DeleteCurrentTest();
					S_TestPageBuffer->currenttestdata = NULL;
				}
				else if(GetCardState() == NoCard)
				{
					backToActivity(lunchActivityName);
				
					if(IsPaiDuiTestting())
						startActivity(createPaiDuiActivity, NULL);
				}
			}
		}
	}
}

/***************************************************************************************************
*FunctionName: activityHide
*Description: ���ؽ���ʱҪ������
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:01:40
***************************************************************************************************/
static void activityHide(void)
{
	//�����ǰҳ��ĸ澯������
	SendKeyCode(16);
}

/***************************************************************************************************
*FunctionName: activityResume
*Description: ����ָ���ʾʱҪ������
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:01:58
***************************************************************************************************/
static void activityResume(void)
{

}

/***************************************************************************************************
*FunctionName: activityDestroy
*Description: ��������
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:02:15
***************************************************************************************************/
static void activityDestroy(void)
{
	//�����ǰҳ��ĸ澯������
	SendKeyCode(16);
	
	activityBufferFree();
}

/***************************************************************************************************
*FunctionName: activityBufferMalloc
*Description: ���������ڴ�����
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 
***************************************************************************************************/
static MyState_TypeDef activityBufferMalloc(void)
{
	if(NULL == S_TestPageBuffer)
	{
		S_TestPageBuffer = MyMalloc(sizeof(TestPageBuffer));
		
		if(S_TestPageBuffer)
		{
			memset(S_TestPageBuffer, 0, sizeof(TestPageBuffer));
	
			return My_Pass;
		}
		else
			return My_Fail;
	}
	else
		return My_Pass;
}

/***************************************************************************************************
*FunctionName: activityBufferFree
*Description: �����ڴ��ͷ�
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:03:10
***************************************************************************************************/
static void activityBufferFree(void)
{
	MyFree(S_TestPageBuffer);
	S_TestPageBuffer = NULL;
}


static void InitCurve(void)
{
	ClearLine(0x56);
				
	//��ʼ����������
	S_TestPageBuffer->line.MaxData = 0;
	S_TestPageBuffer->line.MUL_Y = 1;
	S_TestPageBuffer->line.Y_Scale = 100;
				
	SetChartSize(0x1870 , S_TestPageBuffer->line.MUL_Y);
				
	DspNum(0x180B , S_TestPageBuffer->line.Y_Scale, 2);
	DspNum(0x180A , S_TestPageBuffer->line.Y_Scale*2, 2);
	
	BasicUI(0x1880 ,0x1807 , 0, &(S_TestPageBuffer->myico[0]) , sizeof(Basic_ICO)*3);
}

static void InitPageText(void)
{
	sprintf(S_TestPageBuffer->buf, "%s\0", S_TestPageBuffer->currenttestdata->testdata.temperweima.ItemName);
	DisText(0x1810, S_TestPageBuffer->buf, strlen(S_TestPageBuffer->buf)+1);
			
	memcpy(S_TestPageBuffer->buf, S_TestPageBuffer->currenttestdata->testdata.sampleid, MaxSampleIDLen);
	DisText(0x1820, S_TestPageBuffer->buf, strlen(S_TestPageBuffer->buf)+1);
			
	sprintf(S_TestPageBuffer->buf, "%2.1f ��\0", S_TestPageBuffer->currenttestdata->testdata.TestTemp.O_Temperature);
	DisText(0x1830, S_TestPageBuffer->buf, strlen(S_TestPageBuffer->buf)+1);
			
	sprintf(S_TestPageBuffer->buf, "%s-%s\0", S_TestPageBuffer->currenttestdata->testdata.temperweima.PiHao, 
		S_TestPageBuffer->currenttestdata->testdata.temperweima.piNum);
	DisText(0x1840, S_TestPageBuffer->buf, strlen(S_TestPageBuffer->buf)+1);
			
	sprintf(S_TestPageBuffer->buf, "%s\0", S_TestPageBuffer->currenttestdata->testdata.temperweima.itemConstData.normalResult);
	DisText(0x1850, S_TestPageBuffer->buf, strlen(S_TestPageBuffer->buf)+1);
	
	sprintf(S_TestPageBuffer->buf, "\0");
	DisText(0x1838, S_TestPageBuffer->buf, strlen(S_TestPageBuffer->buf)+1);
}

/*��������*/
static void RefreshCurve(void)
{
	unsigned short temp;
	
	while(pdTRUE == TakeTestPointData(&temp))
	{
		if(0xffff == temp)
			InitCurve();
		else
			AddDataToLine(temp);
	}
	 
	if(My_Pass == TakeTestResult(&(S_TestPageBuffer->currenttestdata->testdata.testResultDesc)))
	{
		GetGB_Time(&(S_TestPageBuffer->currenttestdata->testdata.TestTime));
		MotorMoveTo(MaxLocation, 1);

		//����һ�����ݸ���ӡ����ӡ
		memcpy(&(S_TestPageBuffer->testDataForPrintf), &(S_TestPageBuffer->currenttestdata->testdata), sizeof(TestData));
		
		RefreshPageText();
		
		if(S_TestPageBuffer->currenttestdata->testdata.testResultDesc == ResultIsOK)
		{
			if((isAutoPrint() == true) && (S_TestPageBuffer->isPrintfData == 0))
				printfTestData();
		}
		else if(S_TestPageBuffer->currenttestdata->testdata.testResultDesc == PeakError)
		{
			/*����ʧ��*/
			SendKeyCode(2);
		}
		else
		{
			SendKeyCode(1);		
		}
		
		//��������
		WriteTestData(&(S_TestPageBuffer->currenttestdata->testdata), getTestDataTotalNum());
		//������Ŀ+1
		plusTestDataTotalNum(1);
		//���������Ŀ
		SaveSystemSetData(getGBSystemSetData());
	}
}

static void RefreshPageText(void)
{
	double tempvalue = 0.0;
	
	if(S_TestPageBuffer->currenttestdata)
	{
		if(S_TestPageBuffer->currenttestdata->testdata.testResultDesc != ResultIsOK)
			sprintf(S_TestPageBuffer->buf, "Error\0");
		else if(IsShowRealValue() == true)
			sprintf(S_TestPageBuffer->buf, "%.*f %s\0", S_TestPageBuffer->currenttestdata->testdata.temperweima.itemConstData.pointNum,
				S_TestPageBuffer->currenttestdata->testdata.testline.AdjustResult, S_TestPageBuffer->currenttestdata->testdata.temperweima.itemConstData.itemMeasure);
		else if(S_TestPageBuffer->currenttestdata->testdata.testline.AdjustResult <= S_TestPageBuffer->currenttestdata->testdata.temperweima.itemConstData.lowstResult)
			sprintf(S_TestPageBuffer->buf, "<%.*f %s\0", S_TestPageBuffer->currenttestdata->testdata.temperweima.itemConstData.pointNum, 
				S_TestPageBuffer->currenttestdata->testdata.temperweima.itemConstData.lowstResult, S_TestPageBuffer->currenttestdata->testdata.temperweima.itemConstData.itemMeasure);
		else if(S_TestPageBuffer->currenttestdata->testdata.testline.AdjustResult >= S_TestPageBuffer->currenttestdata->testdata.temperweima.itemConstData.highestResult)
			sprintf(S_TestPageBuffer->buf, ">%.*f %s\0", S_TestPageBuffer->currenttestdata->testdata.temperweima.itemConstData.pointNum, 
				S_TestPageBuffer->currenttestdata->testdata.temperweima.itemConstData.highestResult, S_TestPageBuffer->currenttestdata->testdata.temperweima.itemConstData.itemMeasure);
		else
			sprintf(S_TestPageBuffer->buf, "%.*f %s\0", S_TestPageBuffer->currenttestdata->testdata.temperweima.itemConstData.pointNum, 
				S_TestPageBuffer->currenttestdata->testdata.testline.AdjustResult, S_TestPageBuffer->currenttestdata->testdata.temperweima.itemConstData.itemMeasure);
		
		DisText(0x1838, S_TestPageBuffer->buf, strlen(S_TestPageBuffer->buf)+1);
	}
	
	if(S_TestPageBuffer->currenttestdata->testdata.testResultDesc == ResultIsOK)
	{
		//�������ϱ�ǳ�T,C,����
		S_TestPageBuffer->myico[0].ICO_ID = 22;
		S_TestPageBuffer->myico[0].X = 574+S_TestPageBuffer->currenttestdata->testdata.testline.T_Point.x-12;
		tempvalue = S_TestPageBuffer->currenttestdata->testdata.testline.T_Point.y;
		tempvalue /= S_TestPageBuffer->line.Y_Scale*2;
		tempvalue = 1-tempvalue;
		tempvalue *= 302;										//���ߴ��ڿ��
		tempvalue += 139;										//���ߴ�����ʼy
		S_TestPageBuffer->myico[0].Y = (unsigned short)tempvalue - 11;
		
		S_TestPageBuffer->myico[1].ICO_ID = 22;
		S_TestPageBuffer->myico[1].X = 574+S_TestPageBuffer->currenttestdata->testdata.testline.C_Point.x-12;
		tempvalue = S_TestPageBuffer->currenttestdata->testdata.testline.C_Point.y;
		tempvalue /= S_TestPageBuffer->line.Y_Scale*2;
		tempvalue = 1-tempvalue;
		tempvalue *= 302;										//���ߴ��ڿ��
		tempvalue += 139;										//���ߴ�����ʼy
		S_TestPageBuffer->myico[1].Y = (unsigned short)tempvalue - 11;
		
		S_TestPageBuffer->myico[2].ICO_ID = 22;
		S_TestPageBuffer->myico[2].X = 574+S_TestPageBuffer->currenttestdata->testdata.testline.B_Point.x-12;
		tempvalue = S_TestPageBuffer->currenttestdata->testdata.testline.B_Point.y;
		tempvalue /= S_TestPageBuffer->line.Y_Scale*2;
		tempvalue = 1-tempvalue;
		tempvalue *= 302;										//���ߴ��ڿ��
		tempvalue += 139;										//���ߴ�����ʼy
		S_TestPageBuffer->myico[2].Y = (unsigned short)tempvalue - 11;
		
		BasicUI(0x1880 ,0x1807 , 3, &(S_TestPageBuffer->myico[0]) , sizeof(Basic_ICO)*3);
	}
	else
	{
		BasicUI(0x1880 ,0x1807 , 0, &S_TestPageBuffer->myico[0] , sizeof(Basic_ICO)*3);
	}
}


static void AddDataToLine(unsigned short data)
{
	unsigned short tempdata = data;
	double TempMul_Y2;
	double TempY_Scale;
	unsigned short tempvalue;
	
	if(S_TestPageBuffer->line.MaxData <= tempdata)
	{
		S_TestPageBuffer->line.MaxData = tempdata;

		////////////////////////��Ե�ǰ�������ֵ����y��Ŵ���//////////////////////////////////////
		TempMul_Y2 = TestLineHigh;
		TempMul_Y2 /= S_TestPageBuffer->line.MaxData;
		TempMul_Y2 *= 0.8;			//*0.8�ǽ����ֵ���ŵ����̶ȵ�0.8�߶ȴ�

		
		tempvalue = (unsigned short)(TempMul_Y2*10);
		S_TestPageBuffer->line.MUL_Y = ((tempvalue%10) > 5)?(tempvalue/10 + 1):(tempvalue/10);
		
		if(S_TestPageBuffer->line.MUL_Y < 1)			//��СֵΪ1
			S_TestPageBuffer->line.MUL_Y = 1;

		/////////////////////////��Ե�ǰ�Ŵ���������y��̶ȵ�������/////////////////////////////////////
		TempY_Scale = TestLineHigh ;
		TempY_Scale /= S_TestPageBuffer->line.MUL_Y;
		TempY_Scale /= 2.0;																//Ŀǰ��ʾ2��y��̶�
		S_TestPageBuffer->line.Y_Scale = (unsigned short)TempY_Scale;
		
		SetChartSize(0x1870 , S_TestPageBuffer->line.MUL_Y);

		DspNum(0x180B , S_TestPageBuffer->line.Y_Scale, 2);
		DspNum(0x180A , S_TestPageBuffer->line.Y_Scale*2, 2);
	}
	DisPlayLine(0 , &tempdata , 1);
}

/***************************************************************************************************
*FunctionName: printfTestData
*Description: ��ӡ��������
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2017��1��19��16:39:51
***************************************************************************************************/
static void printfTestData(void)
{
	if(S_TestPageBuffer->testDataForPrintf.testResultDesc != NoResult)
	{
		if(S_TestPageBuffer->isPrintfData == 0)
		{
			S_TestPageBuffer->isPrintfData = 1;
			SendKeyCode(6);
			PrintfData(&(S_TestPageBuffer->testDataForPrintf));
			SendKeyCode(16);
			S_TestPageBuffer->isPrintfData = 0;
		}
	}
	else
		SendKeyCode(4);
}

