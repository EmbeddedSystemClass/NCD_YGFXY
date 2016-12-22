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
#include	"Test_Task.h"
#include	"LunchPage.h"
#include	"Printf_Fun.h"
#include	"Motor_Fun.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"

/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static TestPageBuffer * S_TestPageBuffer = NULL;
const unsigned int TestLineHigh = 77010;	//��������������ʾ����߶��йأ�������治�ģ���������
/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static void RefreshCurve(void);
static void AddDataToLine(unsigned short data);
static void RefreshPageText(void);

static void activityStart(void);
static void activityInput(unsigned char *pbuf , unsigned short len);
static void activityFresh(void);
static void activityHide(void);
static void activityResume(void);
static void activityDestroy(void);
static MyState_TypeDef activityBufferMalloc(void);
static void activityBufferFree(void);
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
		InitActivity(thizActivity, "TestActivity", activityStart, activityInput, activityFresh, activityHide, activityResume, activityDestroy);
		
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
		/*�������*/
		ClearLine(0x56);
		
		//��ʼ����������
		S_TestPageBuffer->line.MaxData = 0;
		S_TestPageBuffer->line.MUL_Y = 1;
		S_TestPageBuffer->line.Y_Scale = 100;
		
		SetChartSize(0x1870 , S_TestPageBuffer->line.MUL_Y);
		
		DspNum(0x180B , S_TestPageBuffer->line.Y_Scale, 2);
		DspNum(0x180A , S_TestPageBuffer->line.Y_Scale*2, 2);
		
		/*��ȡ��ǰ�������ݵĵ�ַ*/
		S_TestPageBuffer->currenttestdata = GetCurrentTestItem();
		
		RefreshPageText();

		StartTest(&(S_TestPageBuffer->currenttestdata->testdata));
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
			if(S_TestPageBuffer->cardpretestresult != NoResult)
			{
				if(CardPinIn)
				{
					SendKeyCode(5);
					return;
				}
				
				//ɾ����ǰ����
				DeleteCurrentTest();
				
				backToActivity(lunchActivityName);
				
				//������п����Ŷӣ���ֱ�������Ŷӽ���
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
			if(S_TestPageBuffer->cardpretestresult == ResultIsOK)
			{
				if(My_Pass == ConnectPrintter())
				{
					SendKeyCode(6);
					PrintfData(&(S_TestPageBuffer->currenttestdata->testdata));
					SendKeyCode(16);
				}
				else
					SendKeyCode(4);
			}
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
		RefreshCurve();
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



/*��������*/
static void RefreshCurve(void)
{
	unsigned short temp;
	
	if(pdTRUE == TakeTestPointData(&temp))
	{
		if(0xffff == temp)
		{
			ClearLine(0x56);
				
			//��ʼ����������
			S_TestPageBuffer->line.MaxData = 0;
			S_TestPageBuffer->line.MUL_Y = 1;
			S_TestPageBuffer->line.Y_Scale = 100;
				
			SetChartSize(0x1870 , S_TestPageBuffer->line.MUL_Y);
				
			DspNum(0x180B , S_TestPageBuffer->line.Y_Scale, 2);
			DspNum(0x180A , S_TestPageBuffer->line.Y_Scale*2, 2);
		}
		else
			AddDataToLine(temp);
	}
	 
	if(My_Pass == TakeTestResult(&(S_TestPageBuffer->cardpretestresult)))
	{
		
		MotorMoveTo(MaxLocation, 1);
		
		if(S_TestPageBuffer->cardpretestresult == ResultIsOK)
		{
			RefreshPageText();
				
			GetGB_Time(&(GetCurrentTestItem()->testdata.TestTime));
			WriteTestData(&(GetCurrentTestItem()->testdata));
		}
		else if(S_TestPageBuffer->cardpretestresult == PeakError)
		{
			/*����ʧ��*/
			SendKeyCode(2);
		}
		else
		{
			SendKeyCode(1);			
		}
	}
}

static void RefreshPageText(void)
{
	char *buf = NULL;
	double tempvalue = 0.0;
	
	buf = MyMalloc(100);
	if(buf)
	{
		memset(buf, 0, 100);
		
		if((S_TestPageBuffer)&&(S_TestPageBuffer->currenttestdata))
		{
			sprintf(buf, "%s", S_TestPageBuffer->currenttestdata->testdata.temperweima.ItemName);
			DisText(0x1810, buf, 20);
			
			memset(buf, 0, 100);
			memcpy(buf, S_TestPageBuffer->currenttestdata->testdata.sampleid, MaxSampleIDLen);
			DisText(0x1820, buf, 20);
			
			sprintf(buf, "%2.1f", S_TestPageBuffer->currenttestdata->testdata.TestTemp.O_Temperature);
			DisText(0x1830, buf, 8);
			
			sprintf(buf, "%s", S_TestPageBuffer->currenttestdata->testdata.temperweima.CardPiCi);
			DisText(0x1840, buf, 30);
			
			if(S_TestPageBuffer->currenttestdata->testdata.testline.AdjustResult <= S_TestPageBuffer->currenttestdata->testdata.temperweima.LowstResult)
				sprintf(buf, "<%.3f", S_TestPageBuffer->currenttestdata->testdata.temperweima.LowstResult);
			else if(S_TestPageBuffer->currenttestdata->testdata.testline.AdjustResult >= S_TestPageBuffer->currenttestdata->testdata.temperweima.HighestResult)
				sprintf(buf, ">%.3f", S_TestPageBuffer->currenttestdata->testdata.temperweima.HighestResult);
			else
				sprintf(buf, "%.3f", S_TestPageBuffer->currenttestdata->testdata.testline.AdjustResult);
			DisText(0x1850, buf, 30);
		}
	}
	MyFree(buf);
	
	if(S_TestPageBuffer->cardpretestresult == ResultIsOK)
	{
		//�������ϱ�ǳ�T,C,����
		S_TestPageBuffer->myico[0].ICO_ID = 22;
		S_TestPageBuffer->myico[0].X = 505+S_TestPageBuffer->currenttestdata->testdata.testline.T_Point[1]-5;
		tempvalue = S_TestPageBuffer->currenttestdata->testdata.testline.T_Point[0];
		tempvalue /= S_TestPageBuffer->line.Y_Scale*2;
		tempvalue = 1-tempvalue;
		tempvalue *= 302;										//���ߴ��ڿ��
		tempvalue += 139;										//���ߴ�����ʼy
		S_TestPageBuffer->myico[0].Y = (unsigned short)tempvalue - 5;
		
		S_TestPageBuffer->myico[1].ICO_ID = 22;
		S_TestPageBuffer->myico[1].X = 505+S_TestPageBuffer->currenttestdata->testdata.testline.C_Point[1]-5;
		tempvalue = S_TestPageBuffer->currenttestdata->testdata.testline.C_Point[0];
		tempvalue /= S_TestPageBuffer->line.Y_Scale*2;
		tempvalue = 1-tempvalue;
		tempvalue *= 302;										//���ߴ��ڿ��
		tempvalue += 139;										//���ߴ�����ʼy
		S_TestPageBuffer->myico[1].Y = (unsigned short)tempvalue - 5;
		
		S_TestPageBuffer->myico[2].ICO_ID = 22;
		S_TestPageBuffer->myico[2].X = 505+S_TestPageBuffer->currenttestdata->testdata.testline.B_Point[1]-5;
		tempvalue = S_TestPageBuffer->currenttestdata->testdata.testline.B_Point[0];
		tempvalue /= S_TestPageBuffer->line.Y_Scale*2;
		tempvalue = 1-tempvalue;
		tempvalue *= 302;										//���ߴ��ڿ��
		tempvalue += 139;										//���ߴ�����ʼy
		S_TestPageBuffer->myico[2].Y = (unsigned short)tempvalue - 5;
		
		BasicUI(0x1880 ,0x1907 , 3, &(S_TestPageBuffer->myico[0]) , sizeof(Basic_ICO)*3);
	}
	else
	{
		memset(S_TestPageBuffer->myico, 0, sizeof(Basic_ICO)*3);
		BasicUI(0x1880 ,0x1907 , 3, &S_TestPageBuffer->myico[0] , sizeof(Basic_ICO)*3);
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

