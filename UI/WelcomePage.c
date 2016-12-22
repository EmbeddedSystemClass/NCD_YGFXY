/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"WelcomePage.h"
#include	"Define.h"
#include	"LCD_Driver.h"
#include	"UI_Data.h"
#include	"MyMem.h"
#include	"SelfCheck_Data.h"
#include	"PlaySong_Task.h"

#include	"LunchPage.h"

#include	"Test_Task.h"
#include 	"netconf.h"
#include	"NormalUpLoad_Task.h"
#include	"CodeScan_Task.h"
#include	"Paidui_Task.h"

#include	<string.h>
#include	"stdio.h"
/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static WelcomePageBuffer * S_WelcomePageBuffer = NULL;
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
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/

/***************************************************************************************************
*FunctionName: createWelcomeActivity
*Description: ������ӭ����
*Input: thizActivity -- ��ǰ����
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��20��16:21:51
***************************************************************************************************/
MyState_TypeDef createWelcomeActivity(Activity * thizActivity, Intent * pram)
{
	if(NULL == thizActivity)
		return My_Fail;
	
	if(My_Pass == activityBufferMalloc())
	{
		InitActivity(thizActivity, "WelcomeActivity", activityStart, activityInput, activityFresh, activityHide, activityResume, activityDestroy);
		
		return My_Pass;
	}
	
	return My_Fail;
}

/***************************************************************************************************
*FunctionName: activityStart
*Description: ��ʾ��ǰ����
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��20��16:22:23
***************************************************************************************************/
static void activityStart(void)
{
	if(S_WelcomePageBuffer)
	{
		timer_set(&(S_WelcomePageBuffer->timer), 1);
	}
	
	SetLEDLight(100);
	
	SelectPage(0);
	
	AddNumOfSongToList(52, 0);
}

/***************************************************************************************************
*FunctionName: activityInput
*Description: ��ǰ��������
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��20��16:22:42
***************************************************************************************************/
static void activityInput(unsigned char *pbuf , unsigned short len)
{
	if(S_WelcomePageBuffer)
	{
		if(0x81 == pbuf[3])
		{
			//ҳ��id
			if(0x03 == pbuf[4])
			{
				S_WelcomePageBuffer->lcdinput[0] = pbuf[6];
				S_WelcomePageBuffer->lcdinput[0] = (S_WelcomePageBuffer->lcdinput[0]<<8) + pbuf[7];
				
				//�������ŵ�ĩβ81��ҳ��,���Լ����
				if((81 == S_WelcomePageBuffer->lcdinput[0]) && (SelfCheck_None != GetGB_SelfCheckStatus()))
				{					
					/*������������*/
					StartvTestTask();
					
					/*������������*/
					StartEthernet();

					/*�ϴ�����*/
					StartvNormalUpLoadTask();
					
					/*��������ά������*/
					StartCodeScanTask();
					
					//��ʼ�Ŷ�����
					StartPaiduiTask();
					
					destroyTopActivity();
					startActivity(createLunchActivity, NULL);
				}
			}
		}
		else if(0x83 == pbuf[3])
		{
			
		}
	}
}

/***************************************************************************************************
*FunctionName: activityFresh
*Description: ��ǰ����ˢ��
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��20��16:22:56
***************************************************************************************************/
static void activityFresh(void)
{
	if(S_WelcomePageBuffer)
	{
		if(TimeOut == timer_expired(&(S_WelcomePageBuffer->timer)))
		{
			ReadCurrentPageId();

			timer_reset(&(S_WelcomePageBuffer->timer));
		}
	}
}


static void activityHide(void)
{

}

static void activityResume(void)
{
	
}

/***************************************************************************************************
*FunctionName: activityFinish
*Description: 
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 
***************************************************************************************************/
static void activityDestroy(void)
{
	activityBufferFree();
}


/***************************************************************************************************
*FunctionName��PageBufferMalloc
*Description����ǰ������ʱ��������
*Input��None
*Output��MyState_TypeDef -- ���سɹ����
*Author��xsx
*Data��2016��6��27��08:56:02
***************************************************************************************************/
static MyState_TypeDef activityBufferMalloc(void)
{
	if(NULL == S_WelcomePageBuffer)
	{
		S_WelcomePageBuffer = (WelcomePageBuffer *)MyMalloc(sizeof(WelcomePageBuffer));
			
		if(S_WelcomePageBuffer)
		{
			memset(S_WelcomePageBuffer, 0, sizeof(WelcomePageBuffer));
		
			return My_Pass;
			
		}
	}
	
	return My_Fail;
}

/***************************************************************************************************
*FunctionName��PageBufferFree
*Description����ǰ������ʱ�����ͷ�
*Input��None
*Output��None
*Author��xsx
*Data��2016��6��27��08:56:21
***************************************************************************************************/
static void activityBufferFree(void)
{
	MyFree(S_WelcomePageBuffer);
	S_WelcomePageBuffer = NULL;
}


