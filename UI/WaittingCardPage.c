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

unsigned char DspWaittingCardPage(void *  parm)
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


static void Input(unsigned char *pbuf , unsigned short len)
{
	if(S_WaitPageData)
	{
		/*����*/
		S_WaitPageData->lcdinput[0] = pbuf[4];
		S_WaitPageData->lcdinput[0] = (S_WaitPageData->lcdinput[0]<<8) + pbuf[5];
		
		/*����*/
		if(S_WaitPageData->lcdinput[0] == 0x1303)
		{
			PageBufferFree();
			PageBackTo(ParentPage);
		}
	}
}

static void PageUpDate(void)
{
	if(S_WaitPageData)
	{
		
		/*��ʾ�忨*/
		if(TimeOut == timer_expired(&(S_WaitPageData->timer2)))
		{
			AddNumOfSongToList(43, 0);
			timer_restart(&(S_WaitPageData->timer2));
		}
		
		/*�Ƿ�忨*/
		if(GetCardState() == CardIN)
		{
			PageBufferFree();
			PageAdvanceTo(DspPreReadCardPage, NULL);
			
		}
		/*ʱ�䵽��δ�忨������*/
		else if(TimeOut == timer_expired(&(S_WaitPageData->timer)))
		{
			AddNumOfSongToList(8, 0);
			
			PageBufferFree();
			PageBackTo(ParentPage);
		}
		
		
	}
}

static MyState_TypeDef PageInit(void *  parm)
{
	if(My_Fail == PageBufferMalloc())
		return My_Fail;
	
	SelectPage(88);
	
	MotorMoveTo(WaittingCardLocation, 1);
	
	S_WaitPageData->currenttestdata = GetCurrentTestItem();
	
	/*�ȴ�ʱ�䣬��ʱ��ȡ������*/
	timer_set(&(S_WaitPageData->timer), 55);
	
	/*���һ��ʱ����ʾ�忨*/
	timer_set(&(S_WaitPageData->timer2), 20);
	
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
