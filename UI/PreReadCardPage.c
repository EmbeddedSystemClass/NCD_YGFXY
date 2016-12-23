/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"PreReadCardPage.h"
#include	"Define.h"
#include	"LCD_Driver.h"
#include	"UI_Data.h"
#include	"MyMem.h"
#include	"MyTest_Data.h"
#include 	"MLX90614_Driver.h"
#include	"LunchPage.h"
#include	"CodeScan_Task.h"
#include	"WaittingCardPage.h"
#include	"TimeDownNorPage.h"
#include	"CodeScanFunction.h"
#include	"PaiDuiPage.h"
#include	"CardStatues_Data.h"
#include	"Motor_Fun.h"
#include	"PlaySong_Task.h"
#include	"Test_Task.h"
#include	"SDFunction.h"
#include	"MyTools.h"
#include	"CRC16.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"

/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static PreReadPageBuffer * S_PreReadPageBuffer = NULL;
const unsigned int TestLineHigh1 = 77010;	//��������������ʾ����߶��йأ�������治�ģ���������
/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static void activityStart(void);
static void activityInput(unsigned char *pbuf , unsigned short len);
static void activityFresh(void);
static void activityHide(void);
static void activityResume(void);
static void activityDestroy(void);
static MyState_TypeDef activityBufferMalloc(void);
static void activityBufferFree(void);

static void CheckQRCode(void);
static void ShowCardInfo(void);
static void CheckPreTestCard(void);
static void RefreshCurve(void);
static void AddDataToLine(unsigned short data);
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
MyState_TypeDef createPreReadCardActivity(Activity * thizActivity, Intent * pram)
{
	if(NULL == thizActivity)
		return My_Fail;
	
	if(My_Pass == activityBufferMalloc())
	{
		InitActivity(thizActivity, "PreReadCardActivity\0", activityStart, activityInput, activityFresh, activityHide, activityResume, activityDestroy);
		
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
	if(S_PreReadPageBuffer)
	{
		/*�������*/
		ClearText(0x1420, 30);
		ClearText(0x1430, 30);
		ClearText(0x1440, 10);
		ClearText(0x1450, 30);
		
		S_PreReadPageBuffer->currenttestdata = GetCurrentTestItem();
		
		AddNumOfSongToList(44, 0);
		
		StartScanQRCode(&(S_PreReadPageBuffer->temperweima));
	}
	
	SelectPage(92);
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
	if(S_PreReadPageBuffer)
	{
		/*����*/
		S_PreReadPageBuffer->lcdinput[0] = pbuf[4];
		S_PreReadPageBuffer->lcdinput[0] = (S_PreReadPageBuffer->lcdinput[0]<<8) + pbuf[5];
		
		/*��ά���ȡʧ�ܣ����ڣ���ʹ��*/
		if((S_PreReadPageBuffer->lcdinput[0] >= 0x1400) && (S_PreReadPageBuffer->lcdinput[0] <= 0x1404))
		{
			/*����*/
			S_PreReadPageBuffer->lcdinput[1] = pbuf[7];
			S_PreReadPageBuffer->lcdinput[1] = (S_PreReadPageBuffer->lcdinput[1]<<8) + pbuf[8];
			
			/*������⿨*/
			if(S_PreReadPageBuffer->lcdinput[1] == 0x0001)
			{
				backToFatherActivity();
			}
			/*�˳�*/
			else if(S_PreReadPageBuffer->lcdinput[1] == 0x0000)
			{
				DeleteCurrentTest();
				
				backToActivity(lunchActivityName);
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
	if(S_PreReadPageBuffer)
	{
		CheckQRCode();
	
		

		RefreshCurve();
		
		CheckPreTestCard();
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
	if(NULL == S_PreReadPageBuffer)
	{
		S_PreReadPageBuffer = MyMalloc(sizeof(PreReadPageBuffer));
		
		if(S_PreReadPageBuffer)
		{
			memset(S_PreReadPageBuffer, 0, sizeof(PreReadPageBuffer));
	
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
	MyFree(S_PreReadPageBuffer);
	S_PreReadPageBuffer = NULL;
}



static void CheckQRCode(void)
{
	if((S_PreReadPageBuffer) && (My_Pass == TakeScanQRCodeResult(&(S_PreReadPageBuffer->scancode))))
	{	
		ClearLine(0x56);
		if((S_PreReadPageBuffer->scancode == CardCodeScanFail) || (S_PreReadPageBuffer->scancode == CardCodeCardOut) ||
			(S_PreReadPageBuffer->scancode == CardCodeScanTimeOut) || (S_PreReadPageBuffer->scancode == CardCodeCRCError))
		{
			SendKeyCode(1);
			MotorMoveTo(MaxLocation, 1);
			AddNumOfSongToList(23, 0);
		}
		else if(S_PreReadPageBuffer->scancode == CardCodeTimeOut)
		{
			SendKeyCode(4);
			MotorMoveTo(MaxLocation, 1);
			AddNumOfSongToList(21, 0);
		}
		else if(S_PreReadPageBuffer->scancode == CardCodeScanOK)
		{
			//��ȡ��⿨�¶�
			S_PreReadPageBuffer->currenttestdata->testdata.TestTemp.O_Temperature = GetCardTemperature();
			
			ShowCardInfo();
			
			//����������ݰ��еĶ�ά��crcУ�����������ǵ�һ�ζ�ȡ��ά��
			if(S_PreReadPageBuffer->currenttestdata->testdata.temperweima.CRC16 == 0)
			{
				//����ȡ�Ķ�ά�����ݿ������������ݰ���
				memcpy(&(S_PreReadPageBuffer->currenttestdata->testdata.temperweima), &(S_PreReadPageBuffer->temperweima), sizeof(CardCodeInfo));
				
				//���õ���ʱʱ��
				timer_set(&(S_PreReadPageBuffer->currenttestdata->timer), S_PreReadPageBuffer->currenttestdata->testdata.temperweima.CardWaitTime*60);
			
				//��ȡУ׼����
				memcpy(S_PreReadPageBuffer->currenttestdata->testdata.tempadjust.ItemName, S_PreReadPageBuffer->currenttestdata->testdata.temperweima.ItemName, ItemNameLen);
				if(My_Fail == ReadAdjustData(&(S_PreReadPageBuffer->currenttestdata->testdata.tempadjust)))
					memset(&(S_PreReadPageBuffer->currenttestdata->testdata.tempadjust), 0, sizeof(AdjustData));
				
				StartTest(&(S_PreReadPageBuffer->currenttestdata->testdata));
			}
			else
			{
				//У���Ի������
				if(pdFAIL == CheckStrIsSame(S_PreReadPageBuffer->currenttestdata->testdata.temperweima.CardPiCi, S_PreReadPageBuffer->temperweima.CardPiCi, CardPiCiHaoLen))
				{
					SendKeyCode(2);
					MotorMoveTo(MaxLocation, 1);
				}
				else
				{
					startActivity(createTimeDownActivity, NULL);
				}
			}
		}
	}
}

static void CheckPreTestCard(void)
{
	if((S_PreReadPageBuffer) && (My_Pass == TakeTestResult(&(S_PreReadPageBuffer->cardpretestresult))))
	{
		ShowCardInfo();
		
		if(S_PreReadPageBuffer->cardpretestresult == TestInterrupt)
		{
			SendKeyCode(1);
			MotorMoveTo(MaxLocation, 1);
			AddNumOfSongToList(22, 0);
			memset(&(S_PreReadPageBuffer->currenttestdata->testdata.temperweima), 0, sizeof(CardCodeInfo));
		}
		else if(S_PreReadPageBuffer->cardpretestresult == NoSample)
		{
			SendKeyCode(5);
			MotorMoveTo(MaxLocation, 1);
			AddNumOfSongToList(22, 0);
			memset(&(S_PreReadPageBuffer->currenttestdata->testdata.temperweima), 0, sizeof(CardCodeInfo));
		}
		else if(S_PreReadPageBuffer->cardpretestresult == PeakError)
		{
			//������Ŷ�ģʽ��������Ŷӽ���
			if(S_PreReadPageBuffer->currenttestdata->testlocation > 0)
			{
				MotorMoveTo(MaxLocation, 1);
				
				S_PreReadPageBuffer->currenttestdata->statues = startpaidui;
				timer_restart(&(S_PreReadPageBuffer->currenttestdata->timer));

				startActivity(createPaiDuiActivity, NULL);
			}
			else
			{		
				timer_restart(&(S_PreReadPageBuffer->currenttestdata->timer));
				
				startActivity(createTimeDownActivity, NULL);
			}
		}
		else if(S_PreReadPageBuffer->cardpretestresult == ResultIsOK)
		{
			SendKeyCode(3);
			MotorMoveTo(MaxLocation, 1);
			AddNumOfSongToList(22, 0);
			memset(&(S_PreReadPageBuffer->currenttestdata->testdata.temperweima), 0, sizeof(CardCodeInfo));
		}
	}
}

static void ShowCardInfo(void)
{
	double tempvalue = 0.0;
	
	sprintf(S_PreReadPageBuffer->buf, "%s", S_PreReadPageBuffer->temperweima.ItemName);
	DisText(0x1420, S_PreReadPageBuffer->buf, strlen(S_PreReadPageBuffer->buf));
			
	sprintf(S_PreReadPageBuffer->buf, "%s", S_PreReadPageBuffer->temperweima.CardPiCi);
	DisText(0x1430, S_PreReadPageBuffer->buf, strlen(S_PreReadPageBuffer->buf));
			
	sprintf(S_PreReadPageBuffer->buf, "%d", S_PreReadPageBuffer->temperweima.CardWaitTime*60);
	DisText(0x1440, S_PreReadPageBuffer->buf, strlen(S_PreReadPageBuffer->buf));
			
	sprintf(S_PreReadPageBuffer->buf, "20%02d��%02d��%02d��", S_PreReadPageBuffer->temperweima.CardBaoZhiQi.year, S_PreReadPageBuffer->temperweima.CardBaoZhiQi.month,
		S_PreReadPageBuffer->temperweima.CardBaoZhiQi.day);
	DisText(0x1450, S_PreReadPageBuffer->buf, strlen(S_PreReadPageBuffer->buf));
	
	memset(S_PreReadPageBuffer->buf, 0, 40);
	sprintf(S_PreReadPageBuffer->buf, "cv1=%.3f", S_PreReadPageBuffer->currenttestdata->testdata.testline.BasicBili);
//	sprintf(S_PreReadPageBuffer->buf, "%2.1f", S_PreReadPageBuffer->currenttestdata->testdata.TestTemp.O_Temperature);
	DisText(0x1460, S_PreReadPageBuffer->buf, 20);
	
	memset(S_PreReadPageBuffer->buf, 0, 40);
	sprintf(S_PreReadPageBuffer->buf, "cv2=%.3f",S_PreReadPageBuffer->currenttestdata->testdata.testline.BasicResult);
	DisText(0x1470, S_PreReadPageBuffer->buf, 20);
	
	if(S_PreReadPageBuffer->cardpretestresult == ResultIsOK)
	{
		//�������ϱ�ǳ�T,C,����
		S_PreReadPageBuffer->myico[0].ICO_ID = 22;
		S_PreReadPageBuffer->myico[0].X = 505+S_PreReadPageBuffer->currenttestdata->testdata.testline.T_Point[1]-5;
		tempvalue = S_PreReadPageBuffer->currenttestdata->testdata.testline.T_Point[0];
		tempvalue /= S_PreReadPageBuffer->line.Y_Scale*2;
		tempvalue = 1-tempvalue;
		tempvalue *= 302;										//���ߴ��ڿ��
		tempvalue += 139;										//���ߴ�����ʼy
		S_PreReadPageBuffer->myico[0].Y = (unsigned short)tempvalue - 5;
		
		S_PreReadPageBuffer->myico[1].ICO_ID = 22;
		S_PreReadPageBuffer->myico[1].X = 505+S_PreReadPageBuffer->currenttestdata->testdata.testline.C_Point[1]-5;
		tempvalue = S_PreReadPageBuffer->currenttestdata->testdata.testline.C_Point[0];
		tempvalue /= S_PreReadPageBuffer->line.Y_Scale*2;
		tempvalue = 1-tempvalue;
		tempvalue *= 302;										//���ߴ��ڿ��
		tempvalue += 139;										//���ߴ�����ʼy
		S_PreReadPageBuffer->myico[1].Y = (unsigned short)tempvalue - 5;
		
		S_PreReadPageBuffer->myico[2].ICO_ID = 22;
		S_PreReadPageBuffer->myico[2].X = 505+S_PreReadPageBuffer->currenttestdata->testdata.testline.B_Point[1]-5;
		tempvalue = S_PreReadPageBuffer->currenttestdata->testdata.testline.B_Point[0];
		tempvalue /= S_PreReadPageBuffer->line.Y_Scale*2;
		tempvalue = 1-tempvalue;
		tempvalue *= 302;										//���ߴ��ڿ��
		tempvalue += 139;										//���ߴ�����ʼy
		S_PreReadPageBuffer->myico[2].Y = (unsigned short)tempvalue - 5;
		
		BasicUI(0x1880 ,0x1907 , 3, &(S_PreReadPageBuffer->myico[0]) , sizeof(Basic_ICO)*3);
	}
	else
	{
		memset(S_PreReadPageBuffer->myico, 0, sizeof(Basic_ICO)*3);
		BasicUI(0x1880 ,0x1907 , 3, &S_PreReadPageBuffer->myico[0] , sizeof(Basic_ICO)*3);
	}
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
			S_PreReadPageBuffer->line.MaxData = 0;
			S_PreReadPageBuffer->line.MUL_Y = 1;
			S_PreReadPageBuffer->line.Y_Scale = 100;
				
			SetChartSize(0x1870 , S_PreReadPageBuffer->line.MUL_Y);
		}
		else
			AddDataToLine(temp);
	}

}


static void AddDataToLine(unsigned short data)
{
	unsigned short tempdata = data;
	double TempMul_Y2;
	double TempY_Scale;
	unsigned short tempvalue;
	
	if(S_PreReadPageBuffer->line.MaxData <= tempdata)
	{
		S_PreReadPageBuffer->line.MaxData = tempdata;

		////////////////////////��Ե�ǰ�������ֵ����y��Ŵ���//////////////////////////////////////
		TempMul_Y2 = TestLineHigh1;
		TempMul_Y2 /= S_PreReadPageBuffer->line.MaxData;
		TempMul_Y2 *= 0.8;			//*0.8�ǽ����ֵ���ŵ����̶ȵ�0.8�߶ȴ�

		
		tempvalue = (unsigned short)(TempMul_Y2*10);
		S_PreReadPageBuffer->line.MUL_Y = ((tempvalue%10) > 5)?(tempvalue/10 + 1):(tempvalue/10);
		
		if(S_PreReadPageBuffer->line.MUL_Y < 1)			//��СֵΪ1
			S_PreReadPageBuffer->line.MUL_Y = 1;

		/////////////////////////��Ե�ǰ�Ŵ���������y��̶ȵ�������/////////////////////////////////////
		TempY_Scale = TestLineHigh1 ;
		TempY_Scale /= S_PreReadPageBuffer->line.MUL_Y;
		TempY_Scale /= 2.0;																//Ŀǰ��ʾ2��y��̶�
		S_PreReadPageBuffer->line.Y_Scale = (unsigned short)TempY_Scale;
		
		SetChartSize(0x1870 , S_PreReadPageBuffer->line.MUL_Y);


	}
	DisPlayLine(0 , &tempdata , 1);

}
