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
	unsigned short *pdata = NULL;
	
	pdata = MyMalloc((len/2)*sizeof(unsigned short));
	if(pdata == NULL)
		return;
	
	/*����*/
	pdata[0] = pbuf[4];
	pdata[0] = (pdata[0]<<8) + pbuf[5];
	
	/*������Ϣ*/
	if(pdata[0] == 0x2200)
	{
		SetGBChildPage(DspShowDeviceInfoPage);
		GotoGBChildPage(NULL);
	}
	/*�����˹���*/
	else if(pdata[0] == 0x2201)
	{
		SetGBChildPage(DspUserMPage);
		GotoGBChildPage(NULL);
	}
	/*��������*/
	else if(pdata[0] == 0x2202)
	{
		SetGBChildPage(DspNetPreSetPage);
		GotoGBChildPage(NULL);
	}
	/*���ݹ���*/
	else if(pdata[0] == 0x2203)
	{
		SetGBChildPage(DspRecordPage);
		GotoGBChildPage(NULL);
	}
	/*У׼����*/
	else if(pdata[0] == 0x2204)
	{
		SetGBChildPage(DspAdjustPage);
		GotoGBChildPage(NULL);
	}
	else if(pdata[0] == 0x2230)
	{
		if(GetBufLen(&pbuf[7] , 2*pbuf[6]) == 6)
		{
			if(pdPASS == CheckStrIsSame(&pbuf[7] , AdjustPassWord ,GetBufLen(&pbuf[7] , 2*pbuf[6])))
			{
				SetGBChildPage(DspAdjustPage);
				GotoGBChildPage(NULL);
			}
			else if(pdPASS == CheckStrIsSame(&pbuf[7] , TestPassWord ,GetBufLen(&pbuf[7] , 2*pbuf[6])))
			{
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
	else if(pdata[0] == 0x2205)
	{
		SetGBChildPage(DspOtherSetPage);
		GotoGBChildPage(NULL);
	}
	/*����*/
	else if(pdata[0] == 0x2206)
	{
		GotoGBParentPage(NULL);
	}
	
	MyFree(pdata);
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
	return My_Pass;
}



