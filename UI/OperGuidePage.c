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

unsigned char DspOperGuidePage(void *  parm)
{
	SetGBSysPage(DspOperGuidePage, DspWaittingCardPage, DspWaittingCardPage, Input, PageUpDate, PageInit, PageBufferMalloc, PageBufferFree);
	
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
	
	/*����*/
	if(pdata[0] == 0x1e02)
	{
		GBPageBufferFree();
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
	SelectPage(90);
	
	AddNumOfSongToList(48, 0);
	
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
