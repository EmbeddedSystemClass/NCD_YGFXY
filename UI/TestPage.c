/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"TestPage.h"
#include	"Define.h"
#include	"LCD_Driver.h"
#include	"UI_Data.h"
#include	"MyMem.h"
#include	"WaittingCardPage.h"
#include	"MyTest_Data.h"

#include	"Temperature_Data.h"
#include	"System_Data.h"
#include	"SDFunction.h"
#include	"Test_Task.h"
#include	"LunchPage.h"
#include	"Printf_Fun.h"

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
static void Input(unsigned char *pbuf , unsigned short len);
static void PageUpDate(void);
static void AddDataToLine(unsigned short data);
static void RefreshPageText(void);
static void ClearPageText(void);

static MyState_TypeDef PageInit(void *  parm);
static MyState_TypeDef PageBufferMalloc(void);
static MyState_TypeDef PageBufferFree(void);
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/

unsigned char DspTestPage(void *  parm)
{
	SetGBSysPage(DspTestPage, NULL, DspLunchPage, Input, PageUpDate, PageInit, PageBufferMalloc, PageBufferFree);
	
	GBPageInit(parm);
	
	return 0;
}


static void Input(unsigned char *pbuf , unsigned short len)
{
	if(S_TestPageBuffer)
	{
		/*����*/
		S_TestPageBuffer->lcdinput[0] = pbuf[4];
		S_TestPageBuffer->lcdinput[0] = (S_TestPageBuffer->lcdinput[0]<<8) + pbuf[5];
		
		/*�˳�*/
		if(0x2191 == S_TestPageBuffer->lcdinput[0])
		{
			if(S_TestPageBuffer->cardpretestresult != NoResult)
			{
				DeleteCurrentTest();

				GBPageBufferFree();
				GotoGBChildPage(NULL);
			}
			else
				SendKeyCode(7);
		}
		/*��ӡ����*/
		else if(0x2190 == S_TestPageBuffer->lcdinput[0])
		{
			if(S_TestPageBuffer->cardpretestresult == ResultIsOK)
			{
				if(My_Pass == ConnectPrintter())
				{
					SendKeyCode(6);
					PrintfData(&(S_TestPageBuffer->currenttestdata->testdata));
					SendKeyCode(9);
				}
				else
					SendKeyCode(8);
			}
		}
	}
}

static void PageUpDate(void)
{
	if(S_TestPageBuffer)
		RefreshCurve();
}

static MyState_TypeDef PageInit(void *  parm)
{
	if(My_Fail == PageBufferMalloc())
		return My_Fail;
	
	SelectPage(66);
	
	/*�������*/
	ClearLine(0x56);
	
	ClearPageText();
	
	//��ʼ����������
	S_TestPageBuffer->line.MaxData = 0;
	S_TestPageBuffer->line.MUL_Y = 1;
	S_TestPageBuffer->line.Y_Scale = 100;
	
	SetChartSize(0x1870 , S_TestPageBuffer->line.MUL_Y);
	
	DspNum(0x1860 , S_TestPageBuffer->line.Y_Scale, 2);
	DspNum(0x1850 , S_TestPageBuffer->line.Y_Scale*2, 2);
	
	/*��ȡ��ǰ�������ݵĵ�ַ*/
	S_TestPageBuffer->currenttestdata = GetCurrentTestItem();
	
//	RefreshPageText();
	
	StartTest(&(S_TestPageBuffer->currenttestdata->testdata));
	
	return My_Pass;
}

static MyState_TypeDef PageBufferMalloc(void)
{	
	S_TestPageBuffer = (TestPageBuffer *)MyMalloc(sizeof(TestPageBuffer));
			
	if(S_TestPageBuffer)
	{
		memset(S_TestPageBuffer, 0, sizeof(TestPageBuffer));
		
		return My_Pass;
	}
	else
		return My_Fail;
}

static MyState_TypeDef PageBufferFree(void)
{
	MyFree(S_TestPageBuffer);
	S_TestPageBuffer = NULL;
	
	return My_Pass;
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
				
			DspNum(0x1860 , S_TestPageBuffer->line.Y_Scale, 2);
			DspNum(0x1850 , S_TestPageBuffer->line.Y_Scale*2, 2);
		}
		else
			AddDataToLine(temp);
	}
	 
	if(My_Pass == TakeTestResult(&(S_TestPageBuffer->cardpretestresult)))
	{
		
		if(S_TestPageBuffer->cardpretestresult == ResultIsOK)
		{
			RefreshPageText();
				
			GetGB_Time(&(GetCurrentTestItem()->testdata.TestTime));
			SaveTestData(&(GetCurrentTestItem()->testdata));
		}
		else if(S_TestPageBuffer->cardpretestresult == PeakNumZero)
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

static void ClearPageText(void)
{
	ClearText(0x2100, 60);
	ClearText(0x2110, 60);
	ClearText(0x2120, 60);
	ClearText(0x2130, 60);
	ClearText(0x2140, 60);
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
			DisText(0x2100, buf, strlen(buf));
			
			memset(buf, 0, 100);
			memcpy(buf, S_TestPageBuffer->currenttestdata->testdata.sampleid, MaxSampleIDLen);
			DisText(0x2110, buf, strlen(buf));
			
			sprintf(buf, "%2.1f", GetCardTemperature());
			DisText(0x2120, buf, strlen(buf));
			
			sprintf(buf, "%s", S_TestPageBuffer->currenttestdata->testdata.temperweima.CardPiCi);
			DisText(0x2130, buf, strlen(buf));
			
/*			if(S_TestPageBuffer->currenttestdata->testdata.testline.AdjustResult <= S_TestPageBuffer->currenttestdata->testdata.temperweima.LowstResult)
				sprintf(buf, "<%.3f", S_TestPageBuffer->currenttestdata->testdata.temperweima.LowstResult);
			else if(S_TestPageBuffer->currenttestdata->testdata.testline.AdjustResult >= S_TestPageBuffer->currenttestdata->testdata.temperweima.HighestResult)
				sprintf(buf, ">%.3f", S_TestPageBuffer->currenttestdata->testdata.temperweima.HighestResult);
			else*/
				sprintf(buf, "%.3f", S_TestPageBuffer->currenttestdata->testdata.testline.AdjustResult);
			DisText(0x2140, buf, strlen(buf));
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
		
		BasicUI(0x2150 ,0x1907 , 3, &(S_TestPageBuffer->myico[0]) , sizeof(Basic_ICO)*3);
	}
	else
	{
		memset(S_TestPageBuffer->myico, 0, sizeof(Basic_ICO)*3);
		BasicUI(0x2150 ,0x1907 , 3, &S_TestPageBuffer->myico[0] , sizeof(Basic_ICO)*3);
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
		
		SetChartSize(0x2160 , S_TestPageBuffer->line.MUL_Y);

		DspNum(0x2180 , S_TestPageBuffer->line.Y_Scale, 2);
		DspNum(0x2170 , S_TestPageBuffer->line.Y_Scale*2, 2);

	}
	DisPlayLine(0 , &tempdata , 1);

}

