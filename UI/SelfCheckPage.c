/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"SelfCheckPage.h"
#include	"Define.h"
#include	"LCD_Driver.h"
#include	"UI_Data.h"
#include	"SelfCheck_Task.h"
#include	"LunchPage.h"
#include	"MyMem.h"
#include	"PlaySong_Task.h"
#include	"SelfCheck_Data.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"

/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
const char selfchecktextinfo[19][50] = {
	"���ط���������\0",
	"���������ݼ���ʧ��\0",
	"���������ݼ��سɹ�\0",
	"���ڼ���ά��ģ��\0", 
	"��ά��ģ�����\0", 
	"��ά��ģ������\0" ,
	"���ڼ��洢ģ��\0", 
	"�洢ģ�����\0", 
	"�洢ģ������\0" ,
	"���ڼ��WIFIģ��\0",
	"WIFIģ�����\0",
	"WIFIģ������\0",
	"���ڼ��ɼ�ģ��\0",
	"�ɼ�ģ�����\0",
	"�ɼ�ģ������\0",
	"���ڼ�⴫��ģ��\0",
	"����ģ�����\0",
	"����ģ������\0",
	
	"�Լ����",
};
const char selfcheckpicinfo[19] = {5, 10, 10, 15, 20, 20,25,30,30, 35, 45, 45, 55, 75, 75, 80,85, 85,100};

/******************************************************************************************/
/*****************************************�ֲ���������*************************************/

static void Input(unsigned char *pbuf , unsigned short len);
static void PageUpDate(void);
static MyState_TypeDef PageInit(void *  parm);
static MyState_TypeDef PageBufferMalloc(void);
static MyState_TypeDef PageBufferFree(void);

static void PageClear(void);
static void RefreshPageValue(void);
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/

unsigned char DspSelfCheckPage(void *  parm)
{
	SetGBSysPage(DspSelfCheckPage, NULL, DspLunchPage, Input, PageUpDate, PageInit, PageBufferMalloc, PageBufferFree);
	
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
	
	/*����Ǵ��󣬵����˵�*/
	if(pdata[0] == 0x1a30)
	{
		/*����*/
		pdata[1] = pbuf[7];
		pdata[1] = (pdata[1]<<8) + pbuf[8];
		
		/*����*/
		if(pdata[1] == 0x0001)
			while(1);
	}
	/*����Ǹ澯�������˵�*/
	else if(pdata[0] == 0x1a31)
	{
		/*����*/
		pdata[1] = pbuf[7];
		pdata[1] = (pdata[1]<<8) + pbuf[8];
		
		/*����*/
		if(pdata[1] == 0x0001)
			while(1);
		/*����*/
		else if(pdata[1] == 0x0002)
		{
			SetGB_SelfCheckResult(SelfCheck_OK);
			DspLunchPage(NULL);
		}
	}
	
	
	MyFree(pdata);
}

static void PageUpDate(void)
{
	RefreshPageValue();
}

static MyState_TypeDef PageInit(void *  parm)
{
	SelectPage(51);
	
	/*��յ�ǰҳ��*/
	PageClear();
	
	/*��ʾ��ʼ��ʼ��*/
	AddNumOfSongToList(46, 0);
	
	/*��ʼ��ʼ������*/
	SelfCheckTaskRun();
	
	return My_Pass;
}

static MyState_TypeDef PageBufferMalloc(void)
{
	return My_Pass;
}

static MyState_TypeDef PageBufferFree(void)
{
	return My_Pass;
}


/********************************************************************************************************/
/********************************************************************************************************/
/********************************************************************************************************/
/********************************************************************************************************/

static void PageClear(void)
{
	/*����ϴε�����*/
	ClearText(0x1a00, 60);
	
	/*���½�����*/
	BasicPic(0x1a20, 0, 100, 19, 295, 19+4*selfcheckpicinfo[0],302, 316, 357);
}

static void RefreshPageValue(void)
{
	unsigned char recvdat = 0;
	if(pdPASS == GetSelfTestTaskState(&recvdat, 10*portTICK_RATE_MS))
	{		
		/*����ϴε�����*/
		ClearText(0x1a00, 60);
		
		/*�����ı�*/
		DisText(0x1a00, (void *)selfchecktextinfo[recvdat], strlen(selfchecktextinfo[recvdat]));				
		
		/*���½�����*/
		BasicPic(0x1a20, 1, 100, 19, 295, 19+4*selfcheckpicinfo[recvdat],302, 316, 357);
		
		/*�в��ɺ��ԵĴ���*/
		if((recvdat == DataBasesError)||(recvdat == ErWeiMaError)||(recvdat == ADDAError)||(recvdat == MotorError))
			SetGB_SelfCheckResult(SelfCheck_Error);
		/*û�в��ɺ��Դ����ǰ���£��пɺ��ԵĴ���*/
		else if((GetGB_SelfCheckResult() != SelfCheck_Error) && ( (recvdat == WIFIError) || (recvdat == ReadServerError)))
			SetGB_SelfCheckResult(SelfCheck_Alam);
		
		if(recvdat == SelfCheckOver)
		{
			vTaskDelay(1000 / portTICK_RATE_MS);
			if(GetGB_SelfCheckResult() == SelfCheck_Error)
			{
				SendKeyCode(1);
				AddNumOfSongToList(49, 0);
			}
			else if(GetGB_SelfCheckResult() == SelfCheck_Alam)
			{
				SendKeyCode(2);
				AddNumOfSongToList(48, 0);
			}
			else
			{
				SetGB_SelfCheckResult(SelfCheck_OK);
				DspLunchPage(NULL);
				AddNumOfSongToList(50, 0);
			}
		}
	}
}

