/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"SelectUserPage.h"
#include	"Define.h"
#include	"LCD_Driver.h"
#include	"UI_Data.h"
#include	"MyMem.h"
#include	"Users_Data.h"
#include	"LunchPage.h"
#include	"SampleIDPage.h"
#include	"MyTest_Data.h"
#include	"CRC16.h"
#include	"SDFunction.h"
#include	"PlaySong_Task.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"

/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static UserPageBuffer * GB_UserPageBuffer = NULL;

/******************************************************************************************/
/*****************************************�ֲ���������*************************************/

static void Input(unsigned char *pbuf , unsigned short len);
static void PageUpData(void);

static MyState_TypeDef PageInit(void *  parm);
static MyState_TypeDef PageBufferMalloc(void);
static MyState_TypeDef PageBufferFree(void);

static void ShowList(void);
static void SelectUser(unsigned char index);
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/***************************************************************************************************
*FunctionName��DspSelectUserPage
*Description����ʾѡ������˽���
*Input��None
*Output��None
*Author��xsx
*Data��2016��6��29��08:39:45
***************************************************************************************************/
unsigned char DspSelectUserPage(void *  parm)
{
	SysPage * myPage = GetSysPage();

	myPage->CurrentPage = DspSelectUserPage;
	myPage->LCDInput = Input;
	myPage->PageUpData = PageUpData;
	myPage->ParentPage = DspLunchPage;
	myPage->ChildPage = DspSampleIDPage;
	myPage->PageInit = PageInit;
	myPage->PageBufferMalloc = PageBufferMalloc;
	myPage->PageBufferFree = PageBufferFree;
	
	SelectPage(54);
	
	myPage->PageInit(NULL);

	return 0;
}

/***************************************************************************************************
*FunctionName��Input
*Description����ǰ����İ�������
*Input��pbuf -- ����ָ��
*		len -- ָ���
*Output��None
*Author��xsx
*Data��2016��6��29��08:40:22
***************************************************************************************************/
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
	if(pdata[0] == 0x1c00)
	{
		DeleteCurrentTest();
		
		PageBufferFree();
		myPage->ParentPage(NULL);
	}
	
	/*�Ϸ�Ҳ*/
	else if(pdata[0] == 0x1c02)
	{
		if(GB_UserPageBuffer)
		{
			if(GB_UserPageBuffer->pageindex > 1)
			{
				GB_UserPageBuffer->pageindex--;
					
				GB_UserPageBuffer->selectindex = 0;
					
				ShowList();
				SelectUser(GB_UserPageBuffer->selectindex);
			}
		}
	}
	/*�·�ҳ*/
	else if(pdata[0] == 0x1c03)
	{
		if(GB_UserPageBuffer)
		{
			if(GB_UserPageBuffer->pageindex < (MaxUserNum / MaxPageShowNum))
			{
				GB_UserPageBuffer->tempuser = &GB_UserPageBuffer->user[(GB_UserPageBuffer->pageindex)*MaxPageShowNum];
		
				if(GB_UserPageBuffer->tempuser->crc == CalModbusCRC16Fun1(GB_UserPageBuffer->tempuser, sizeof(User_Type)-2))
				{
					GB_UserPageBuffer->pageindex++;
					
					GB_UserPageBuffer->selectindex = 0;
					
					ShowList();
					SelectUser(GB_UserPageBuffer->selectindex);
				}
			}
		}
	}
	/*ȷ��*/
	else if(pdata[0] == 0x1c01)
	{
		if(GB_UserPageBuffer->selectindex != 0)
		{
			/*�Ե�ǰѡ��Ĳ�������Ϊ���β������ݵĲ�����*/
			memcpy(&(GB_UserPageBuffer->currenttestdata->testdata.user), GB_UserPageBuffer->user, sizeof(User_Type));
		
			PageBufferFree();
			myPage->ChildPage(NULL);
		}
		else
		{
			AddNumOfSongToList(45, 0);
			SendKeyCode(1);
		}
	}
	/*ѡ�������*/
	else if((pdata[0] >= 0x1c04)&&(pdata[0] <= 0x1c0e))
	{
		GB_UserPageBuffer->tempuser = &GB_UserPageBuffer->user[(GB_UserPageBuffer->pageindex - 1)*MaxPageShowNum + pdata[0] - 0x1c04];
		
		if(GB_UserPageBuffer->tempuser->crc == CalModbusCRC16Fun1(GB_UserPageBuffer->tempuser, sizeof(User_Type)-2))
		{
			GB_UserPageBuffer->selectindex = pdata[0] - 0x1c04+1;
			SelectUser(GB_UserPageBuffer->selectindex);
		}
	}

	MyFree(pdata);
}

/***************************************************************************************************
*FunctionName��PageUpData
*Description����ǰҳ��ˢ��
*Input��None
*Output��None
*Author��xsx
*Data��2016��6��29��08:40:47
***************************************************************************************************/
static void PageUpData(void)
{
	if(TimeOut == timer_expired(&(GB_UserPageBuffer->timer)))
	{
		AddNumOfSongToList(8, 0);
		DeleteCurrentTest();
		
		PageBufferFree();
		GetSysPage()->ParentPage(NULL);
	}
}

/***************************************************************************************************
*FunctionName��PageInit
*Description��ҳ���ʼ��
*Input��None
*Output��None
*Author��xsx
*Data��2016��6��29��08:41:15
***************************************************************************************************/
static MyState_TypeDef PageInit(void * parm)
{
	if(My_Fail == PageBufferMalloc())
		return My_Fail;
	
	/*���õ���ʱ30s�������ʱ����ȡ���˴β���*/
	timer_set(&(GB_UserPageBuffer->timer), 10);
	
	/*��ȡ��ǰ���Ե�����ָ��*/
	GB_UserPageBuffer->currenttestdata = GetCurrentTestItem();
	
	/*��ȡ���в�����*/
	ReadUserData(GB_UserPageBuffer->user);
	
	GB_UserPageBuffer->pageindex = 1;
	GB_UserPageBuffer->selectindex = 0;
	
	ShowList();
	SelectUser(GB_UserPageBuffer->selectindex);
	
	AddNumOfSongToList(45, 0);
	
	return My_Pass;
}

/***************************************************************************************************
*FunctionName��PageBufferMalloc
*Description����ǰҳ�滺������
*Input��None
*Output��None
*Author��xsx
*Data��2016��6��29��08:41:50
***************************************************************************************************/
static MyState_TypeDef PageBufferMalloc(void)
{	
	if(GB_UserPageBuffer)
		return My_Fail;
	
	GB_UserPageBuffer = (UserPageBuffer *)MyMalloc(sizeof(UserPageBuffer));
			
	if(GB_UserPageBuffer)
	{
		memset(GB_UserPageBuffer, 0, sizeof(UserPageBuffer));
		
		return My_Pass;
	}
	else
		return My_Fail;
}

/***************************************************************************************************
*FunctionName��PageBufferFree
*Description����ǰҳ��Ļ����ͷ�
*Input��None
*Output��None
*Author��xsx
*Data��2016��6��29��08:42:07
***************************************************************************************************/
static MyState_TypeDef PageBufferFree(void)
{
	MyFree(GB_UserPageBuffer);
	GB_UserPageBuffer = NULL;
	
	return My_Pass;
}

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************
*FunctionName��ShowList
*Description����ʾ�б�����
*Input��None
*Output��None
*Author��xsx
*Data��2016��6��29��08:44:00
***************************************************************************************************/
static void ShowList(void)
{
	unsigned char i = 0;
	
	i = (GB_UserPageBuffer->pageindex-1)*MaxPageShowNum;
	
	GB_UserPageBuffer->tempuser = &(GB_UserPageBuffer->user[i]);
	
	/*��ʾ�б�����*/
	for(i=0; i<MaxPageShowNum; i++)
	{
		ClearText(0x1c10+i*16, 32);
		
		if(GB_UserPageBuffer->tempuser->crc != 0)
			DisText(0x1c10+i*16, GB_UserPageBuffer->tempuser->user_name, strlen(GB_UserPageBuffer->tempuser->user_name));
		
		GB_UserPageBuffer->tempuser++;
	}
}

/***************************************************************************************************
*FunctionName��SelectUser
*Description��ѡ��һ�������ˣ����ı���ɫ
*Input��None
*Output��None
*Author��xsx
*Data��2016��6��29��08:44:28
***************************************************************************************************/
static void SelectUser(unsigned char index)
{
	unsigned char i = 0;
	
	BasicPic(0x1cb0, 0, 54, 600, 1, 840, 31, 392, 123+(GB_UserPageBuffer->selectindex-1)*34);
	
	if((GB_UserPageBuffer->selectindex > 0) && (GB_UserPageBuffer->selectindex <= MaxPageShowNum))
	{
		i = (GB_UserPageBuffer->pageindex-1)*MaxPageShowNum + GB_UserPageBuffer->selectindex-1;
		
		GB_UserPageBuffer->tempuser = &(GB_UserPageBuffer->user[i]);
		
		if(GB_UserPageBuffer->tempuser->crc == CalModbusCRC16Fun1(GB_UserPageBuffer->tempuser, sizeof(User_Type)-2))
			BasicPic(0x1cb0, 1, 54, 600, 1, 840, 31, 392, 123+(GB_UserPageBuffer->selectindex-1)*34);
	}
}

