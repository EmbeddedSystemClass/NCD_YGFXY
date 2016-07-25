/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"WelcomePage.h"
#include	"Define.h"
#include	"LCD_Driver.h"
#include	"UI_Data.h"

#include	"PlaySong_Task.h"
#include	"SelfCheckPage.h"

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
	SysPage * myPage = GetSysPage();

	myPage->CurrentPage = DspWelcomePage;
	myPage->LCDInput = Input;
	myPage->PageUpData = PageUpData;
	myPage->ParentPage = NULL;
	myPage->ChildPage = NULL;
	myPage->PageInit = PageInit;
	myPage->PageBufferMalloc = PageBufferMalloc;
	myPage->PageBufferFree = PageBufferFree;

	SelectPage(50);
	
	myPage->PageInit(NULL);
	
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
	

}

/***************************************************************************************************
*FunctionName��PageUpData
*Description���������ݸ��£�10msִ��һ��
*Input��None
*Output��None
*Author��xsx
*Data��2016��6��27��08:55:02
***************************************************************************************************/
static void PageUpData(void)
{
	static unsigned short timecount = 0;
	
	timecount++;
	
	if(timecount >= 500)
	{
		timecount = 0;
		DspSelfCheckPage(NULL);
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

