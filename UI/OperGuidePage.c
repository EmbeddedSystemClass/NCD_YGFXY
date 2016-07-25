/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"OperGuidePage.h"
#include	"Define.h"
#include	"LCD_Driver.h"
#include	"UI_Data.h"
#include	"MyMem.h"

#include	"WaittingCardPage.h"
#include	"PreReadCardPage.h"
#include	"PlaySong_Task.h"

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
static void PageUpData(void);
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/

unsigned char DspOperGuidePage(void *  parm)
{
	SysPage * myPage = GetSysPage();

	myPage->CurrentPage = DspOperGuidePage;
	myPage->LCDInput = Input;
	myPage->PageUpData = PageUpData;
	myPage->ParentPage = DspWaittingCardPage;
	myPage->ChildPage = DspWaittingCardPage;
	
	SelectPage(60);
	
	AddNumOfSongToList(48, 0);
	
	return 0;
}


static void Input(unsigned char *pbuf , unsigned short len)
{
	unsigned short *pdata = NULL;
	SysPage * myPage = GetSysPage();
	
	pdata = MyMalloc((len/2)*sizeof(unsigned short));
	if(pdata == NULL)
		return;
	
	/*����*/
	pdata[0] = pbuf[4];
	pdata[0] = (pdata[0]<<8) + pbuf[5];
	
	/*����*/
	if(pdata[0] == 0x1e02)
	{
		myPage->ParentPage(NULL);
	}

	MyFree(pdata);
}

static void PageUpData(void)
{

}

