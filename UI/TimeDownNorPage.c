/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"TimeDownNorPage.h"
#include	"Define.h"
#include	"LCD_Driver.h"
#include	"UI_Data.h"
#include	"SystemSetPage.h"
#include	"MyMem.h"
#include	"TestPage.h"
#include	"MyTest_Data.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"

/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static TimeDownPageData *S_TimeDownPageData = NULL;

/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static void RefreshTimeText(void);
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

unsigned char DspTimeDownNorPage(void *  parm)
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

}

static void PageUpDate(void)
{
	static unsigned char count = 0;
	
	if(count % 5 == 0)
	{
		RefreshTimeText();
		if((S_TimeDownPageData)&&(TimeOut == timer_expired(S_TimeDownPageData->S_Timer)))
		{
			PageBufferFree();
			PageAdvanceTo(DspTestPage, NULL);
		}
	}
	
	count++;
}

static MyState_TypeDef PageInit(void *  parm)
{
	if(My_Fail == PageBufferMalloc())
		return My_Fail;
	
	SelectPage(95);
	
	S_TimeDownPageData->currenttestdata = GetCurrentTestItem();
	
	S_TimeDownPageData->S_Timer = &(S_TimeDownPageData->currenttestdata->timer);
	
	return My_Pass;
}

static MyState_TypeDef PageBufferMalloc(void)
{	
	S_TimeDownPageData = (TimeDownPageData *)MyMalloc(sizeof(TimeDownPageData));
			
	if(S_TimeDownPageData)
	{
		memset(S_TimeDownPageData, 0, sizeof(TimeDownPageData));
		
		return My_Pass;
	}
	else
		return My_Fail;
}

static MyState_TypeDef PageBufferFree(void)
{
	MyFree(S_TimeDownPageData);
	S_TimeDownPageData = NULL;
	
	return My_Pass;
}

static void RefreshTimeText(void)
{
	if(S_TimeDownPageData)
	{
		S_TimeDownPageData->time = timer_surplus(S_TimeDownPageData->S_Timer);
		DspNum(0x1700 , S_TimeDownPageData->time, 2);
	}
}




