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

#include	"ReadBarCode_Task.h"
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

/***************************************************************************************************
*FunctionName��DspWelcomePage
*Description����ӭ����
*Input��parm -- ����
*Output��None
*Author��xsx
*Data��2016��6��27��08:53:22
***************************************************************************************************/
unsigned char DspWelcomePage(void *  parm)
{
	PageInfo * currentpage = NULL;
	
	if(My_Pass == GetCurrentPage(&currentpage))
	{
		currentpage->PageInit = PageInit;
		currentpage->PageUpDate = PageUpDate;
		currentpage->LCDInput = Input;
		currentpage->PageBufferMalloc = PageBufferMalloc;
		currentpage->PageBufferFree = PageBufferFree;
		
		currentpage->PageInit(currentpage->pram);
	}
	
	return 0;
}

/***************************************************************************************************
*FunctionName��Input
*Description������lcd����
*Input��pbuf -- ��������
*		len -- �������ݳ���
*Output��None
*Author��xsx
*Data��2016��6��27��08:54:10
***************************************************************************************************/
static void Input(unsigned char *pbuf , unsigned short len)
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
					/*������ȡ����ǹ����*/
					StartBarCodeTask();
					
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
					
					PageBufferFree();
					PageAdvanceTo(DspLunchPage, NULL);
				}
			}
		}
		else if(0x83 == pbuf[3])
		{
			
		}
	}
}

/***************************************************************************************************
*FunctionName��PageUpData
*Description���������ݸ��£�10msִ��һ��
*Input��None
*Output��None
*Author��xsx
*Data��2016��6��27��08:55:02
***************************************************************************************************/
static void PageUpDate(void)
{
	if(TimeOut == timer_expired(&(S_WelcomePageBuffer->timer)))
	{
		ReadCurrentPageId();
		
		timer_reset(&(S_WelcomePageBuffer->timer));
	}
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
	if(My_Pass == PageBufferMalloc())
	{
		timer_set(&(S_WelcomePageBuffer->timer), 1);
	}
	
	SetLEDLight(100);
	
	SelectPage(0);
	
	AddNumOfSongToList(52, 0);
	
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
static MyState_TypeDef PageBufferFree(void)
{
	MyFree(S_WelcomePageBuffer);
	S_WelcomePageBuffer = NULL;
	return My_Pass;
}


