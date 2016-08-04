/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"UserMPage.h"
#include	"Define.h"
#include	"LCD_Driver.h"
#include	"UI_Data.h"
#include	"MyMem.h"
#include	"CRC16.h"
#include	"SystemSetPage.h"
#include	"SDFunction.h"
#include	"MyTools.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"

/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static UserMPageBuffer * GB_UserMPageBuffer = NULL;

/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static void Input(unsigned char *pbuf , unsigned short len);
static void PageUpDate(void);
static MyState_TypeDef PageInit(void * parm);
static MyState_TypeDef PageBufferMalloc(void);
static MyState_TypeDef PageBufferFree(void);

static void ShowList(void);
static void ShowDetail(void);

static void AddANewUser(void);
static void DeleteAUser(void);
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/

unsigned char DspUserMPage(void *  parm)
{
	SetGBSysPage(DspUserMPage, DspSystemSetPage, NULL, Input, PageUpDate, PageInit, PageBufferMalloc, PageBufferFree);
	
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
	if(pdata[0] == 0x2400)
	{
		GBPageBufferFree();
		GotoGBParentPage(NULL);
	}
	
	/*�Ϸ�Ҳ*/
	else if(pdata[0] == 0x2403)
	{
		if(GB_UserMPageBuffer)
		{
			if(GB_UserMPageBuffer->pageindex > 1)
			{
				GB_UserMPageBuffer->pageindex--;
				
				GB_UserMPageBuffer->selectindex = 0;
				
				ShowList();
				ShowDetail();
			}
		}
	}
	/*�·�ҳ*/
	else if(pdata[0] == 0x2404)
	{
		if(GB_UserMPageBuffer)
		{
			if(GB_UserMPageBuffer->pageindex < (MaxUserNum / MaxPageShowNum))
			{
				GB_UserMPageBuffer->tempuser = &GB_UserMPageBuffer->user[(GB_UserMPageBuffer->pageindex)*MaxPageShowNum];
		
				if(GB_UserMPageBuffer->tempuser->crc == CalModbusCRC16Fun1(GB_UserMPageBuffer->tempuser, sizeof(User_Type)-2))
				{
					GB_UserMPageBuffer->pageindex++;
					
					GB_UserMPageBuffer->selectindex = 0;
					
					ShowList();
					ShowDetail();
				}
			}
		}
	}
	/*ɾ��*/
	else if(pdata[0] == 0x2401)
	{
		DeleteAUser();
	}
	/*�޸Ļ������*/
	else if(pdata[0] == 0x2402)
	{
		AddANewUser();
	}
	/*ѡ�������*/
	else if((pdata[0] >= 0x240b)&&(pdata[0] <= 0x2414))
	{
		GB_UserMPageBuffer->tempuser = &GB_UserMPageBuffer->user[(GB_UserMPageBuffer->pageindex - 1)*MaxPageShowNum + pdata[0] - 0x240b];
		
		if(GB_UserMPageBuffer->tempuser->crc == CalModbusCRC16Fun1(GB_UserMPageBuffer->tempuser, sizeof(User_Type)-2))
		{
			GB_UserMPageBuffer->selectindex = pdata[0] - 0x240b+1;
			ShowDetail();
		}
	}
	/*����*/
	else if(pdata[0] == 0x24d0)
	{
		if(GB_UserMPageBuffer)
		{
			memset(GB_UserMPageBuffer->tempnewuser.user_name, 0, MaxNameLen);
			memcpy(GB_UserMPageBuffer->tempnewuser.user_name, &pbuf[7], GetBufLen(&pbuf[7] , 2*pbuf[6]));
			GB_UserMPageBuffer->tempnewuser.crc = CalModbusCRC16Fun1(&(GB_UserMPageBuffer->tempnewuser), sizeof(User_Type)-2);
		}
	}
	/*����*/
	else if(pdata[0] == 0x24d5)
	{
		if(GB_UserMPageBuffer)
		{	
			memset(GB_UserMPageBuffer->tempnewuser.user_age, 0, MaxAgeLen);
			memcpy(GB_UserMPageBuffer->tempnewuser.user_age, &pbuf[7], GetBufLen(&pbuf[7] , 2*pbuf[6]));
			GB_UserMPageBuffer->tempnewuser.crc = CalModbusCRC16Fun1(&(GB_UserMPageBuffer->tempnewuser), sizeof(User_Type)-2);
		}
	}
	/*�Ա�*/
	else if(pdata[0] == 0x24da)
	{
		if(GB_UserMPageBuffer)
		{
			memset(GB_UserMPageBuffer->tempnewuser.user_sex, 0, MaxSexLen);
			memcpy(GB_UserMPageBuffer->tempnewuser.user_sex, &pbuf[7], GetBufLen(&pbuf[7] , 2*pbuf[6]));
			GB_UserMPageBuffer->tempnewuser.crc = CalModbusCRC16Fun1(&(GB_UserMPageBuffer->tempnewuser), sizeof(User_Type)-2);
		}
	}
	/*��ϵ��ʽ*/
	else if(pdata[0] == 0x24e0)
	{
		if(GB_UserMPageBuffer)
		{	
			memset(GB_UserMPageBuffer->tempnewuser.user_phone, 0, MaxPhoneLen);
			memcpy(GB_UserMPageBuffer->tempnewuser.user_phone, &pbuf[7], GetBufLen(&pbuf[7] , 2*pbuf[6]));
			GB_UserMPageBuffer->tempnewuser.crc = CalModbusCRC16Fun1(&(GB_UserMPageBuffer->tempnewuser), sizeof(User_Type)-2);
		}
	}
	/*ְλ*/
	else if(pdata[0] == 0x24f0)
	{
		if(GB_UserMPageBuffer)
		{	
			memset(GB_UserMPageBuffer->tempnewuser.user_job, 0, MaxJobLen);
			memcpy(GB_UserMPageBuffer->tempnewuser.user_job, &pbuf[7], GetBufLen(&pbuf[7] , 2*pbuf[6]));
			GB_UserMPageBuffer->tempnewuser.crc = CalModbusCRC16Fun1(&(GB_UserMPageBuffer->tempnewuser), sizeof(User_Type)-2);
		}
	}
	/*��ע*/
	else if(pdata[0] == 0x2510)
	{
		if(GB_UserMPageBuffer)
		{
			memset(GB_UserMPageBuffer->tempnewuser.user_desc, 0, MaxDescLen);
			memcpy(GB_UserMPageBuffer->tempnewuser.user_desc, &pbuf[7], GetBufLen(&pbuf[7] , 2*pbuf[6]));
			GB_UserMPageBuffer->tempnewuser.crc = CalModbusCRC16Fun1(&(GB_UserMPageBuffer->tempnewuser), sizeof(User_Type)-2);
		}
	}
	MyFree(pdata);
}

static void PageUpDate(void)
{

}

static MyState_TypeDef PageInit(void * parm)
{
	if(My_Fail == PageBufferMalloc())
		return My_Fail;
	
	SelectPage(76);
	
	/*��ȡ���в�����*/
	ReadUserData(GB_UserMPageBuffer->user);
	
	GB_UserMPageBuffer->pageindex = 1;
	GB_UserMPageBuffer->selectindex = 0;
	
	ShowList();
	ShowDetail();

	return My_Pass;
}

static MyState_TypeDef PageBufferMalloc(void)
{	
	GB_UserMPageBuffer = (UserMPageBuffer *)MyMalloc(sizeof(UserMPageBuffer));
			
	if(GB_UserMPageBuffer)
	{
		memset(GB_UserMPageBuffer, 0, sizeof(UserMPageBuffer));
		
		return My_Pass;
	}
	else
		return My_Fail;
}

static MyState_TypeDef PageBufferFree(void)
{
	MyFree(GB_UserMPageBuffer);
	GB_UserMPageBuffer = NULL;
	
	return My_Pass;
}

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
static void ShowList(void)
{
	unsigned char i = 0;
	
	i = (GB_UserMPageBuffer->pageindex-1)*MaxPageShowNum;
	
	GB_UserMPageBuffer->tempuser = &(GB_UserMPageBuffer->user[i]);
	
	/*��ʾ�б�����*/
	for(i=0; i<MaxPageShowNum; i++)
	{
		ClearText(0x2430+16*i, 32);
		
		if(GB_UserMPageBuffer->tempuser->crc == CalModbusCRC16Fun1(GB_UserMPageBuffer->tempuser, sizeof(User_Type)-2))
			DisText(0x2430+16*i, GB_UserMPageBuffer->tempuser->user_name, strlen(GB_UserMPageBuffer->tempuser->user_name));
		
		GB_UserMPageBuffer->tempuser++;
	}
}

static void ShowDetail(void)
{
	unsigned char i = 0;
	
	ClearText(0x24d0, 32);
	ClearText(0x24d5, 16);
	ClearText(0x24da, 16);
	ClearText(0x24e0, 32);
	ClearText(0x24f0, 32);
	ClearText(0x2510, 64);
	BasicPic(0x2420, 0, 76, 600, 1, 840, 31, 184, 135+(GB_UserMPageBuffer->selectindex-1)*34);
	
	if((GB_UserMPageBuffer->selectindex > 0) && (GB_UserMPageBuffer->selectindex <= MaxPageShowNum))
	{
		i = (GB_UserMPageBuffer->pageindex-1)*MaxPageShowNum + GB_UserMPageBuffer->selectindex-1;
	
		GB_UserMPageBuffer->tempuser = &(GB_UserMPageBuffer->user[i]);
		
		if(GB_UserMPageBuffer->tempuser->crc == CalModbusCRC16Fun1(GB_UserMPageBuffer->tempuser, sizeof(User_Type)-2))
		{
			memcpy(&(GB_UserMPageBuffer->tempnewuser), GB_UserMPageBuffer->tempuser, sizeof(User_Type));
			DisText(0x24d0, GB_UserMPageBuffer->tempuser->user_name, strlen(GB_UserMPageBuffer->tempuser->user_name));
			DisText(0x24d5, GB_UserMPageBuffer->tempuser->user_age, strlen(GB_UserMPageBuffer->tempuser->user_age));
			DisText(0x24da, GB_UserMPageBuffer->tempuser->user_sex, strlen(GB_UserMPageBuffer->tempuser->user_sex));
			DisText(0x24e0, GB_UserMPageBuffer->tempuser->user_phone, strlen(GB_UserMPageBuffer->tempuser->user_phone));
			DisText(0x24f0, GB_UserMPageBuffer->tempuser->user_job, strlen(GB_UserMPageBuffer->tempuser->user_job));
			DisText(0x2510, GB_UserMPageBuffer->tempuser->user_desc, strlen(GB_UserMPageBuffer->tempuser->user_desc));
			BasicPic(0x2420, 1, 76, 600, 1, 840, 31, 184, 135+(GB_UserMPageBuffer->selectindex-1)*34);
		}
	}
	else
		memset(&(GB_UserMPageBuffer->tempnewuser), 0, sizeof(User_Type));
}


static void AddANewUser(void)
{
	unsigned char i=0;
	unsigned char type = 0;
	
	if(GB_UserMPageBuffer->tempnewuser.crc == CalModbusCRC16Fun1(&(GB_UserMPageBuffer->tempnewuser), sizeof(User_Type)-2))
	{
		/*����Ƿ��Ѵ���*/
		GB_UserMPageBuffer->tempuser = &(GB_UserMPageBuffer->user[0]);
		
		for(i=0; i<MaxUserNum; i++)
		{
			if( (GB_UserMPageBuffer->tempuser->crc == CalModbusCRC16Fun1(GB_UserMPageBuffer->tempuser, sizeof(User_Type)-2)) &&
				(pdPASS == CheckStrIsSame(GB_UserMPageBuffer->tempuser->user_name, GB_UserMPageBuffer->tempnewuser.user_name, MaxNameLen)))
			{
				memcpy(GB_UserMPageBuffer->tempuser, &(GB_UserMPageBuffer->tempnewuser), sizeof(User_Type));
				type = 1;
				break;
			}
			GB_UserMPageBuffer->tempuser++;
		}
		
		/*��������ڣ����½�*/
		if(i >= MaxUserNum)
		{
			GB_UserMPageBuffer->tempuser = &(GB_UserMPageBuffer->user[0]);
		
			for(i=0; i<MaxUserNum; i++)
			{
				if(GB_UserMPageBuffer->tempuser->crc != CalModbusCRC16Fun1(GB_UserMPageBuffer->tempuser, sizeof(User_Type)-2))
				{
					memcpy(GB_UserMPageBuffer->tempuser, &(GB_UserMPageBuffer->tempnewuser), sizeof(User_Type));
					type = 2;
					break;
				}
				GB_UserMPageBuffer->tempuser++;
			}
		}
		
		/*����пռ�*/
		if(i < MaxUserNum)
		{
			/*��������*/
			if(My_Fail == SaveUserData(&(GB_UserMPageBuffer->user[0])))
			{
				if(type == 1)
					SendKeyCode(6);
				else if(type == 2)
					SendKeyCode(4);
				ReadUserData(GB_UserMPageBuffer->user);
			}
			else
			{
				if(type == 1)
					SendKeyCode(5);
				else if(type == 2)
					SendKeyCode(3);
			}
			
			memset(&(GB_UserMPageBuffer->tempnewuser), 0, sizeof(User_Type));
			GB_UserMPageBuffer->selectindex = 0;

			ShowList();
			ShowDetail();
		}
		else
			SendKeyCode(4);
	}
}

static void DeleteAUser(void)
{
	if((GB_UserMPageBuffer->selectindex > 0) && (GB_UserMPageBuffer->selectindex <= MaxPageShowNum))
	{
		GB_UserMPageBuffer->tempuser = &(GB_UserMPageBuffer->user[(GB_UserMPageBuffer->pageindex-1)*MaxPageShowNum + GB_UserMPageBuffer->selectindex-1]);
		memset(GB_UserMPageBuffer->tempuser, 0, sizeof(User_Type));
		
		
		/*��������*/
		
		if(My_Fail == SaveUserData(&(GB_UserMPageBuffer->user[0])))
			SendKeyCode(2);
		else
			SendKeyCode(1);
		
		memset(GB_UserMPageBuffer->user, 0, sizeof(User_Type)*MaxUserNum);
		ReadUserData(GB_UserMPageBuffer->user);
		
		GB_UserMPageBuffer->pageindex = 1;
		GB_UserMPageBuffer->selectindex = 0;
			
		ShowList();
		ShowDetail();
	}

}

