/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"SystemSetPage.h"
#include	"Define.h"
#include	"LCD_Driver.h"
#include	"UI_Data.h"
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

static void Input(unsigned char *pbuf , unsigned short len);
static void PageUpDate(void);
static MyState_TypeDef PageInit(void *  parm);
static MyState_TypeDef PageBufferMalloc(void);
static MyState_TypeDef PageBufferFree(void);
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/

unsigned char DspSystemSetPage(void *  parm)
{
	SetGBSysPage(DspSystemSetPage, DspLunchPage, NULL, Input, PageUpDate, PageInit, PageBufferMalloc, PageBufferFree);
	
	GBPageInit(parm);
	
	return 0;
}


static void Input(unsigned char *pbuf , unsigned short len)
{
	if(S_SysSetPageBuffer)
	{
		/*����*/
		S_SysSetPageBuffer->lcdinput[0] = pbuf[4];
		S_SysSetPageBuffer->lcdinput[0] = (S_SysSetPageBuffer->lcdinput[0]<<8) + pbuf[5];
		
		/*������Ϣ*/
		if(S_SysSetPageBuffer->lcdinput[0] == 0x2200)
		{
			GBPageBufferFree();
			SetGBChildPage(DspShowDeviceInfoPage);
			GotoGBChildPage(NULL);
		}
		/*�����˹���*/
		else if(S_SysSetPageBuffer->lcdinput[0] == 0x2201)
		{
			GBPageBufferFree();
			SetGBChildPage(DspUserMPage);
			GotoGBChildPage(NULL);
		}
		/*��������*/
		else if(S_SysSetPageBuffer->lcdinput[0] == 0x2202)
		{
			GBPageBufferFree();
			SetGBChildPage(DspNetPreSetPage);
			GotoGBChildPage(NULL);
		}
		/*���ݹ���*/
		else if(S_SysSetPageBuffer->lcdinput[0] == 0x2203)
		{
			GBPageBufferFree();
			SetGBChildPage(DspRecordPage);
			GotoGBChildPage(NULL);
		}
		/*У׼����*/
		else if(S_SysSetPageBuffer->lcdinput[0] == 0x2204)
		{
			GBPageBufferFree();
			SetGBChildPage(DspAdjustPage);
			GotoGBChildPage(NULL);
		}
		else if(S_SysSetPageBuffer->lcdinput[0] == 0x2230)
		{
			if(GetBufLen(&pbuf[7] , 2*pbuf[6]) == 6)
			{
				if(pdPASS == CheckStrIsSame(&pbuf[7] , AdjustPassWord ,GetBufLen(&pbuf[7] , 2*pbuf[6])))
				{
					GBPageBufferFree();
					SetGBChildPage(DspAdjustPage);
					GotoGBChildPage(NULL);
				}
				else if(pdPASS == CheckStrIsSame(&pbuf[7] , TestPassWord ,GetBufLen(&pbuf[7] , 2*pbuf[6])))
				{
					GBPageBufferFree();
					SetGBChildPage(DspReTestPage);
					GotoGBChildPage(NULL);
				}
				else
					SendKeyCode(1);
			}
			else
					SendKeyCode(1);
		}
		/*��������*/
		else if(S_SysSetPageBuffer->lcdinput[0] == 0x2205)
		{
			GBPageBufferFree();
			SetGBChildPage(DspOtherSetPage);
			GotoGBChildPage(NULL);
		}
		/*����*/
		else if(S_SysSetPageBuffer->lcdinput[0] == 0x2206)
		{
			GBPageBufferFree();
			GotoGBParentPage(NULL);
		}
	}
}

static void PageUpDate(void)
{

}

/***************************************************************************************************
*FunctionName��PageInit
*Description����ǰ�����ʼ��
*Input��None
*Output��None
*Author��xsx
*Data��2016��6��27��08:55:25
***************************************************************************************************/
static MyState_TypeDef PageInit(void *  parm)
{
	if(My_Fail == PageBufferMalloc())
		return My_Fail;
	
	SelectPage(68);
	
	return My_Pass;
}

/***************************************************************************************************
*FunctionName��PageBufferMalloc
*Description����ǰ������ʱ��������
*Input��None
*Output��MyState_TypeDef -- ���سɹ����
*Author��xsx
*Data��2016��6��27��08:56:02
***************************************************************************************************/
static MyState_TypeDef PageBufferMalloc(void)
{
	if(NULL == S_SysSetPageBuffer)
	{
		S_SysSetPageBuffer = (SysSetPageBuffer *)MyMalloc(sizeof(SysSetPageBuffer));
		
		if(NULL == S_SysSetPageBuffer)
			return My_Fail;
	}
	
	memset(S_SysSetPageBuffer, 0, sizeof(SysSetPageBuffer));
	return My_Pass;
}

/***************************************************************************************************
*FunctionName��PageBufferFree
*Description����ǰ������ʱ�����ͷ�
*Input��None
*Output��None
*Author��xsx
*Data��2016��6��27��08:56:21
***************************************************************************************************/
static MyState_TypeDef PageBufferFree(void)
{
	MyFree(S_SysSetPageBuffer);
	S_SysSetPageBuffer = NULL;
	
	return My_Pass;
}



