/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"SystemSetPage.h"
#include	"Define.h"
#include	"LCD_Driver.h"
#include	"MyMem.h"
#include	"LunchPage.h"
#include	"ShowDeviceInfoPage.h"
#include	"AdjustPage.h"
#include	"ReTestPage.h"
#include	"UserMPage.h"
#include	"NetPreSetPage.h"
#include	"RecordPage.h"
#include	"OtherSetPage.h"
#include	"MyTools.h"
#include	"SleepPage.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"

/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static SysSetPageBuffer * S_SysSetPageBuffer = NULL;
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
*FunctionName: createSelectUserActivity
*Description: ����ѡ������˽���
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:00:09
***************************************************************************************************/
MyState_TypeDef createSystemSetActivity(Activity * thizActivity, Intent * pram)
{
	if(NULL == thizActivity)
		return My_Fail;
	
	if(My_Pass == activityBufferMalloc())
	{
		InitActivity(thizActivity, "SystemSetActivity\0", activityStart, activityInput, activityFresh, activityHide, activityResume, activityDestroy);
		
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
	if(S_SysSetPageBuffer)
	{
		//��ȡϵͳ����
		getSystemSetData(&(S_SysSetPageBuffer->systemSetData));
		
		timer_set(&(S_SysSetPageBuffer->timer), S_SysSetPageBuffer->systemSetData.ledSleepTime);
	}
		
	SelectPage(98);
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
	if(S_SysSetPageBuffer)
	{
		/*����*/
		S_SysSetPageBuffer->lcdinput[0] = pbuf[4];
		S_SysSetPageBuffer->lcdinput[0] = (S_SysSetPageBuffer->lcdinput[0]<<8) + pbuf[5];
		
		//��������ʱ��
		timer_restart(&(S_SysSetPageBuffer->timer));
		
		//������Ϣ
		if(S_SysSetPageBuffer->lcdinput[0] == 0x1900)
		{
			startActivity(createDeviceInfoActivity, NULL);
		}
		//�����˹���
		else if(S_SysSetPageBuffer->lcdinput[0] == 0x1901)
		{
			startActivity(createUserManagerActivity, NULL);
		}
		//��������
		else if(S_SysSetPageBuffer->lcdinput[0] == 0x1902)
		{
			startActivity(createNetPreActivity, NULL);
		}
		//���ݹ���
		else if(S_SysSetPageBuffer->lcdinput[0] == 0x1903)
		{
			startActivity(createRecordActivity, NULL);
		}
		//У׼����
		else if(S_SysSetPageBuffer->lcdinput[0] == 0x1910)
		{
			if(GetBufLen(&pbuf[7] , 2*pbuf[6]) == 6)
			{
				if(pdPASS == CheckStrIsSame(&pbuf[7] , AdjustPassWord ,GetBufLen(&pbuf[7] , 2*pbuf[6])))
				{
					startActivity(createAdjActivity, NULL);
				}
				else if(pdPASS == CheckStrIsSame(&pbuf[7] , TestPassWord ,GetBufLen(&pbuf[7] , 2*pbuf[6])))
				{
					startActivity(createReTestActivity, NULL);
				}
				else
					SendKeyCode(1);
			}
			else
					SendKeyCode(1);
		}
		//��������
		else if(S_SysSetPageBuffer->lcdinput[0] == 0x1904)
		{
			startActivity(createOtherSetActivity, NULL);
		}
		//����
		else if(S_SysSetPageBuffer->lcdinput[0] == 0x1906)
		{
			backToFatherActivity();
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
	if(S_SysSetPageBuffer)
	{
		if(TimeOut == timer_expired(&(S_SysSetPageBuffer->timer)))
			startActivity(createSleepActivity, NULL);
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
	if(S_SysSetPageBuffer)
	{
		timer_restart(&(S_SysSetPageBuffer->timer));
	}
	
	SelectPage(98);
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
	if(NULL == S_SysSetPageBuffer)
	{
		S_SysSetPageBuffer = MyMalloc(sizeof(SysSetPageBuffer));
		
		if(S_SysSetPageBuffer)
		{
			memset(S_SysSetPageBuffer, 0, sizeof(SysSetPageBuffer));
	
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
	MyFree(S_SysSetPageBuffer);
	S_SysSetPageBuffer = NULL;
}


