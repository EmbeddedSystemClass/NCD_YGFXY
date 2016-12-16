/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"SelectUserPage.h"
#include	"Define.h"
#include	"LCD_Driver.h"
#include	"UI_Data.h"
#include	"MyMem.h"
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
static UserPageBuffer * S_UserPageBuffer = NULL;

/******************************************************************************************/
/*****************************************�ֲ���������*************************************/

static void Input(unsigned char *pbuf , unsigned short len);
static void PageUpDate(void);

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
*Description����ǰ����İ�������
*Input��pbuf -- ����ָ��
*		len -- ָ���
*Output��None
*Author��xsx
*Data��2016��6��29��08:40:22
***************************************************************************************************/
static void Input(unsigned char *pbuf , unsigned short len)
{
	if(S_UserPageBuffer)
	{
		/*����*/
		S_UserPageBuffer->lcdinput[0] = pbuf[4];
		S_UserPageBuffer->lcdinput[0] = (S_UserPageBuffer->lcdinput[0]<<8) + pbuf[5];
		
		/*����*/
		if(S_UserPageBuffer->lcdinput[0] == 0x1200)
		{
			DeleteCurrentTest();
			
			PageBufferFree();
			PageBackTo(ParentPage);
		}
		
		/*�Ϸ�Ҳ*/
		else if(S_UserPageBuffer->lcdinput[0] == 0x1203)
		{
			/*���õ���ʱ30s�������ʱ����ȡ���˴β���*/
			timer_set(&(S_UserPageBuffer->timer), 10);
			
			if(S_UserPageBuffer->pageindex > 1)
			{
				S_UserPageBuffer->pageindex--;
						
				S_UserPageBuffer->selectindex = 0;
						
				ShowList();
				SelectUser(S_UserPageBuffer->selectindex);
			}
		}
		/*�·�ҳ*/
		else if(S_UserPageBuffer->lcdinput[0] == 0x1204)
		{
			/*���õ���ʱ30s�������ʱ����ȡ���˴β���*/
			timer_set(&(S_UserPageBuffer->timer), 10);
			
			if(S_UserPageBuffer->pageindex < (MaxUserNum / MaxPageShowNum))
			{
				S_UserPageBuffer->tempuser = &S_UserPageBuffer->user[(S_UserPageBuffer->pageindex)*MaxPageShowNum];
			
				if(S_UserPageBuffer->tempuser->crc == CalModbusCRC16Fun1(S_UserPageBuffer->tempuser, sizeof(User_Type)-2))
				{
					S_UserPageBuffer->pageindex++;
						
					S_UserPageBuffer->selectindex = 0;
						
					ShowList();
					SelectUser(S_UserPageBuffer->selectindex);
				}
			}
		}
		/*ȷ��*/
		else if(S_UserPageBuffer->lcdinput[0] == 0x1201)
		{
			if(S_UserPageBuffer->tempuser != NULL)
			{
				//������ŶӲ��ԣ��򱣴�����˵��ŶӲ��Թ��ò�����
				if(S_UserPageBuffer->currenttestdata->testlocation > 0)
					SetPaiduiUser(S_UserPageBuffer->tempuser);
				
				/*�Ե�ǰѡ��Ĳ�������Ϊ���β������ݵĲ�����*/
				memcpy(&(S_UserPageBuffer->currenttestdata->testdata.user), S_UserPageBuffer->tempuser, sizeof(User_Type));
			
				PageBufferFree();
				PageAdvanceTo(DspSampleIDPage, NULL);
			}
			else
			{
				AddNumOfSongToList(45, 0);
				SendKeyCode(1);
			}
		}
		/*ѡ�������*/
		else if((S_UserPageBuffer->lcdinput[0] >= 0x1205)&&(S_UserPageBuffer->lcdinput[0] <= 0x1209))
		{
			/*���õ���ʱ30s�������ʱ����ȡ���˴β���*/
			timer_set(&(S_UserPageBuffer->timer), 10);
			
			S_UserPageBuffer->selectindex = S_UserPageBuffer->lcdinput[0] - 0x1205+1;
			SelectUser(S_UserPageBuffer->selectindex);
		}
	}
}

/***************************************************************************************************
*FunctionName��PageUpData
*Description����ǰҳ��ˢ��
*Input��None
*Output��None
*Author��xsx
*Data��2016��6��29��08:40:47
***************************************************************************************************/
static void PageUpDate(void)
{
	if(TimeOut == timer_expired(&(S_UserPageBuffer->timer)))
	{
		AddNumOfSongToList(8, 0);
		DeleteCurrentTest();
		
		PageBufferFree();
		PageBackTo(ParentPage);
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
	
	SelectPage(84);
	
	/*���õ���ʱ30s�������ʱ����ȡ���˴β���*/
	timer_set(&(S_UserPageBuffer->timer), 10);
	
	/*��ȡ��ǰ���Ե�����ָ��*/
	S_UserPageBuffer->currenttestdata = GetCurrentTestItem();
	
	/*��ȡ���в�����*/
	ReadUserData(S_UserPageBuffer->user);
	
	S_UserPageBuffer->pageindex = 1;
	S_UserPageBuffer->selectindex = 0;
	
	ShowList();
	SelectUser(S_UserPageBuffer->selectindex);
	
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
	if(NULL == S_UserPageBuffer)
	{
		S_UserPageBuffer = (UserPageBuffer *)MyMalloc(sizeof(UserPageBuffer));
			
		if(NULL == S_UserPageBuffer)
			return My_Fail;
	}
	
	memset(S_UserPageBuffer, 0, sizeof(UserPageBuffer));
		
	return My_Pass;
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
	MyFree(S_UserPageBuffer);
	S_UserPageBuffer = NULL;
	
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
	
	i = (S_UserPageBuffer->pageindex-1)*MaxPageShowNum;
	
	S_UserPageBuffer->tempuser = &(S_UserPageBuffer->user[i]);
	
	/*��ʾ�б�����*/
	for(i=0; i<MaxPageShowNum; i++)
	{
		DisText(0x1210+i*8, S_UserPageBuffer->tempuser->user_name, MaxNameLen);
		
		S_UserPageBuffer->tempuser++;
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
	
	BasicPic(0x1240, 0, 140, 506, 402, 798, 470, 364, 142+(S_UserPageBuffer->selectindex-1)*72);
	
	if((S_UserPageBuffer->selectindex > 0) && (S_UserPageBuffer->selectindex <= MaxPageShowNum))
	{
		i = (S_UserPageBuffer->pageindex-1)*MaxPageShowNum + S_UserPageBuffer->selectindex-1;
		
		S_UserPageBuffer->tempuser = &(S_UserPageBuffer->user[i]);
		
		if(S_UserPageBuffer->tempuser->crc == CalModbusCRC16Fun1(S_UserPageBuffer->tempuser, sizeof(User_Type)-2))
		{
			BasicPic(0x1240, 1, 137, 268, 225, 558, 272, 364, 142+(S_UserPageBuffer->selectindex-1)*72);	
		}
		else
		{
			S_UserPageBuffer->tempuser = NULL;
			S_UserPageBuffer->selectindex = 0;
		}
	}
}

