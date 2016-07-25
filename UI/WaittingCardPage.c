/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"WaittingCardPage.h"
#include	"Define.h"
#include	"LCD_Driver.h"
#include	"UI_Data.h"
#include	"MyMem.h"

#include	"LunchPage.h"
#include	"SampleIDPage.h"
#include	"OperGuidePage.h"
#include	"PreReadCardPage.h"
#include	"CardStatues_Data.h"
#include	"Motor_Data.h"
#include	"Motor_Fun.h"
#include	"PlaySong_Task.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"

/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static WaitPageData * S_WaitPageData = NULL;
/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static void Input(unsigned char *pbuf , unsigned short len);
static void PageUpData(void);

static MyState_TypeDef PageInit(void *  parm);
static MyState_TypeDef PageBufferMalloc(void);
static MyState_TypeDef PageBufferFree(void);
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/

unsigned char DspWaittingCardPage(void *  parm)
{
	SysPage * myPage = GetSysPage();

	myPage->CurrentPage = DspWaittingCardPage;
	myPage->LCDInput = Input;
	myPage->PageUpData = PageUpData;
	myPage->ParentPage = DspSampleIDPage;
	myPage->ChildPage = DspOperGuidePage;
	myPage->PageInit = PageInit;
	myPage->PageBufferMalloc = PageBufferMalloc;
	myPage->PageBufferFree = PageBufferFree;
	
	SelectPage(58);
	
	myPage->PageInit(parm);

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
	
	/*����*/
	if(pdata[0] == 0x1e00)
	{
		PageBufferFree();
		DspSampleIDPage(NULL);
	}
	
	/*�鿴�������*/
	else if(pdata[0] == 0x1e01)
	{
		PageBufferFree();
		DspOperGuidePage(NULL);
	}

	MyFree(pdata);
}

static void PageUpData(void)
{
	/*�Ƿ�忨*/
	if(GetCardState() == CardIN)
	{
		//if((S_WaitPageData->currenttestdata)&& (S_WaitPageData->currenttestdata->statues == NotStart))
		PageBufferFree();
		DspPreReadCardPage(NULL);
	}
	/*ʱ�䵽��δ�忨������*/
	else if(TimeOut == timer_expired(&(S_WaitPageData->timer)))
	{
		AddNumOfSongToList(8, 0);
		PageBufferFree();
		DeleteCurrentTest();
		DspLunchPage(NULL);
		return;
	}
	
	/*��ʾ�忨*/
	if(TimeOut == timer_expired(&(S_WaitPageData->timer2)))
	{
		AddNumOfSongToList(43, 0);
		timer_restart(&(S_WaitPageData->timer2));
	}
}

static MyState_TypeDef PageInit(void *  parm)
{
	if(My_Fail == PageBufferMalloc())
		return My_Fail;
	
	MotorMoveTo(WaittingCardLocation, 1);
	
	S_WaitPageData->currenttestdata = GetCurrentTestItem();
	
	/*�ȴ�ʱ�䣬��ʱ��ȡ������*/
	timer_set(&(S_WaitPageData->timer), 55);
	
	/*���һ��ʱ����ʾ�忨*/
	timer_set(&(S_WaitPageData->timer2), 10);
	
	AddNumOfSongToList(43, 0);
	
	return My_Pass;
}

static MyState_TypeDef PageBufferMalloc(void)
{	
	S_WaitPageData = (WaitPageData *)MyMalloc(sizeof(WaitPageData));
			
	if(S_WaitPageData)
	{
		memset(S_WaitPageData, 0, sizeof(WaitPageData));
		
		return My_Pass;
	}
	else
		return My_Fail;
}

static MyState_TypeDef PageBufferFree(void)
{
	MyFree(S_WaitPageData);
	S_WaitPageData = NULL;
	
	return My_Pass;
}
