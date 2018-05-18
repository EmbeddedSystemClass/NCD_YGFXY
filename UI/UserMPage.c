/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"UserMPage.h"
#include	"LCD_Driver.h"
#include	"MyMem.h"
#include	"CRC16.h"
#include	"SystemSetPage.h"
#include	"UserDao.h"
#include	"MyTools.h"
#include	"SleepPage.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"

/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static UserMPageBuffer * S_UserMPageBuffer = NULL;

/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static void activityStart(void);
static void activityInput(unsigned char *pbuf , unsigned short len);
static void activityFresh(void);
static void activityHide(void);
static void activityResume(void);
static void activityDestroy(void);
static MyState_TypeDef activityBufferMalloc(void);
static void activityBufferFree(void);

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

/***************************************************************************************************
*FunctionName: createSelectUserActivity
*Description: ����ѡ������˽���
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:00:09
***************************************************************************************************/
MyState_TypeDef createUserManagerActivity(Activity * thizActivity, Intent * pram)
{
	if(NULL == thizActivity)
		return My_Fail;
	
	if(My_Pass == activityBufferMalloc())
	{
		InitActivity(thizActivity, "UserManagerActivity\0", activityStart, activityInput, activityFresh, activityHide, activityResume, activityDestroy);
		
		return My_Pass;
	}
	
	return My_Fail;
}

/***************************************************************************************************
*FunctionName: activityStart
*Description: ��ʾ������
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:00:32
***************************************************************************************************/
static void activityStart(void)
{
	if(S_UserMPageBuffer)
	{
		/*��ȡ���в�����*/
		ReadUserData(S_UserMPageBuffer->user);
		
		S_UserMPageBuffer->pageindex = 1;
		S_UserMPageBuffer->selectindex = 0;
		
		ShowList();
		ShowDetail();
	}
	SelectPage(106);
}

/***************************************************************************************************
*FunctionName: activityInput
*Description: ��������
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:00:59
***************************************************************************************************/
static void activityInput(unsigned char *pbuf , unsigned short len)
{
	if(S_UserMPageBuffer)
	{
		/*����*/
		S_UserMPageBuffer->lcdinput[0] = pbuf[4];
		S_UserMPageBuffer->lcdinput[0] = (S_UserMPageBuffer->lcdinput[0]<<8) + pbuf[5];
		
		/*����*/
		if(S_UserMPageBuffer->lcdinput[0] == 0x1d00)
		{
			backToFatherActivity();
		}
		
		/*�Ϸ�Ҳ*/
		else if(S_UserMPageBuffer->lcdinput[0] == 0x1d03)
		{
			if(S_UserMPageBuffer->pageindex > 1)
			{
				S_UserMPageBuffer->pageindex--;
					
				S_UserMPageBuffer->selectindex = 0;
					
				ShowList();
				ShowDetail();
			}
		}
		/*�·�ҳ*/
		else if(S_UserMPageBuffer->lcdinput[0] == 0x1d04)
		{
			if(S_UserMPageBuffer->pageindex < (MaxUserNum / MaxPageShowNum))
			{
				S_UserMPageBuffer->tempuser = &S_UserMPageBuffer->user[(S_UserMPageBuffer->pageindex)*MaxPageShowNum];
			
				if(S_UserMPageBuffer->tempuser->crc == CalModbusCRC16Fun1(S_UserMPageBuffer->tempuser, sizeof(User_Type)-2))
				{
					S_UserMPageBuffer->pageindex++;
					
					S_UserMPageBuffer->selectindex = 0;

					ShowList();
					ShowDetail();
				}
			}
		}
		/*ɾ��*/
		else if(S_UserMPageBuffer->lcdinput[0] == 0x1d01)
		{
			DeleteAUser();
		}
		/*�޸Ļ������*/
		else if(S_UserMPageBuffer->lcdinput[0] == 0x1d02)
		{
			if(strlen(S_UserMPageBuffer->tempnewuser.user_name) > 0)
				AddANewUser();
			else
				SendKeyCode(2);
		}
		/*ѡ�������*/
		else if((S_UserMPageBuffer->lcdinput[0] >= 0x1d07)&&(S_UserMPageBuffer->lcdinput[0] <= 0x1d0B))
		{
			S_UserMPageBuffer->tempuser = &S_UserMPageBuffer->user[(S_UserMPageBuffer->pageindex - 1)*MaxPageShowNum + S_UserMPageBuffer->lcdinput[0] - 0x1d07];
			
			if(S_UserMPageBuffer->tempuser->crc == CalModbusCRC16Fun1(S_UserMPageBuffer->tempuser, sizeof(User_Type)-2))
			{
				S_UserMPageBuffer->selectindex = S_UserMPageBuffer->lcdinput[0] - 0x1d07+1;
				ShowDetail();
			}
		}
		/*����*/
		else if(S_UserMPageBuffer->lcdinput[0] == 0x1d50)
		{
			memset(S_UserMPageBuffer->tempnewuser.user_name, 0, MaxNameLen);
			memcpy(S_UserMPageBuffer->tempnewuser.user_name, &pbuf[7], GetBufLen(&pbuf[7] , 2*pbuf[6]));
			S_UserMPageBuffer->tempnewuser.crc = CalModbusCRC16Fun1(&(S_UserMPageBuffer->tempnewuser), sizeof(User_Type)-2);
		}
		/*����*/
		else if(S_UserMPageBuffer->lcdinput[0] == 0x1d60)
		{
			memset(S_UserMPageBuffer->tempnewuser.user_age, 0, MaxAgeLen);
			memcpy(S_UserMPageBuffer->tempnewuser.user_age, &pbuf[7], GetBufLen(&pbuf[7] , 2*pbuf[6]));
			S_UserMPageBuffer->tempnewuser.crc = CalModbusCRC16Fun1(&(S_UserMPageBuffer->tempnewuser), sizeof(User_Type)-2);
		}
		/*�Ա�*/
		else if(S_UserMPageBuffer->lcdinput[0] == 0x1d70)
		{
			memset(S_UserMPageBuffer->tempnewuser.user_sex, 0, MaxSexLen);
			memcpy(S_UserMPageBuffer->tempnewuser.user_sex, &pbuf[7], GetBufLen(&pbuf[7] , 2*pbuf[6]));
			S_UserMPageBuffer->tempnewuser.crc = CalModbusCRC16Fun1(&(S_UserMPageBuffer->tempnewuser), sizeof(User_Type)-2);
		}
		/*��ϵ��ʽ*/
		else if(S_UserMPageBuffer->lcdinput[0] == 0x1d80)
		{
			memset(S_UserMPageBuffer->tempnewuser.user_phone, 0, MaxPhoneLen);
			memcpy(S_UserMPageBuffer->tempnewuser.user_phone, &pbuf[7], GetBufLen(&pbuf[7] , 2*pbuf[6]));
			S_UserMPageBuffer->tempnewuser.crc = CalModbusCRC16Fun1(&(S_UserMPageBuffer->tempnewuser), sizeof(User_Type)-2);
		}
		/*ְλ*/
		else if(S_UserMPageBuffer->lcdinput[0] == 0x1d90)
		{
			memset(S_UserMPageBuffer->tempnewuser.user_job, 0, MaxJobLen);
			memcpy(S_UserMPageBuffer->tempnewuser.user_job, &pbuf[7], GetBufLen(&pbuf[7] , 2*pbuf[6]));
			S_UserMPageBuffer->tempnewuser.crc = CalModbusCRC16Fun1(&(S_UserMPageBuffer->tempnewuser), sizeof(User_Type)-2);
		}
		/*��ע*/
		else if(S_UserMPageBuffer->lcdinput[0] == 0x1da0)
		{
			memset(S_UserMPageBuffer->tempnewuser.user_desc, 0, MaxDescLen);
			memcpy(S_UserMPageBuffer->tempnewuser.user_desc, &pbuf[7], GetBufLen(&pbuf[7] , 2*pbuf[6]));
			S_UserMPageBuffer->tempnewuser.crc = CalModbusCRC16Fun1(&(S_UserMPageBuffer->tempnewuser), sizeof(User_Type)-2);
		}
	}
}

/***************************************************************************************************
*FunctionName: activityFresh
*Description: ����ˢ��
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:01:16
***************************************************************************************************/
static void activityFresh(void)
{

}

/***************************************************************************************************
*FunctionName: activityHide
*Description: ���ؽ���ʱҪ������
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:01:40
***************************************************************************************************/
static void activityHide(void)
{

}

/***************************************************************************************************
*FunctionName: activityResume
*Description: ����ָ���ʾʱҪ������
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:01:58
***************************************************************************************************/
static void activityResume(void)
{
	if(S_UserMPageBuffer)
	{

	}
	
	SelectPage(106);
}

/***************************************************************************************************
*FunctionName: activityDestroy
*Description: ��������
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:02:15
***************************************************************************************************/
static void activityDestroy(void)
{
	activityBufferFree();
}

/***************************************************************************************************
*FunctionName: activityBufferMalloc
*Description: ���������ڴ�����
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 
***************************************************************************************************/
static MyState_TypeDef activityBufferMalloc(void)
{
	if(NULL == S_UserMPageBuffer)
	{
		S_UserMPageBuffer = MyMalloc(sizeof(UserMPageBuffer));
		
		if(S_UserMPageBuffer)
		{
			memset(S_UserMPageBuffer, 0, sizeof(UserMPageBuffer));
	
			return My_Pass;
		}
		else
			return My_Fail;
	}
	else
		return My_Pass;
}

/***************************************************************************************************
*FunctionName: activityBufferFree
*Description: �����ڴ��ͷ�
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:03:10
***************************************************************************************************/
static void activityBufferFree(void)
{
	MyFree(S_UserMPageBuffer);
	S_UserMPageBuffer = NULL;
}

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
static void ShowList(void)
{
	unsigned char i = 0;
	
	i = (S_UserMPageBuffer->pageindex-1)*MaxPageShowNum;
	
	S_UserMPageBuffer->tempuser = &(S_UserMPageBuffer->user[i]);
	
	/*��ʾ�б�����*/
	for(i=0; i<MaxPageShowNum; i++)
	{		
		if(S_UserMPageBuffer->tempuser->crc == CalModbusCRC16Fun1(S_UserMPageBuffer->tempuser, sizeof(User_Type)-2))
			sprintf(S_UserMPageBuffer->buf, "%s\0", S_UserMPageBuffer->tempuser->user_name);
		else
			sprintf(S_UserMPageBuffer->buf, "\0");
		
		DisText(0x1d10+8*i, S_UserMPageBuffer->buf, strlen(S_UserMPageBuffer->buf)+1);
		
		S_UserMPageBuffer->tempuser++;
	}
}

static void ShowDetail(void)
{
	unsigned char i = 0;
	
	if((S_UserMPageBuffer->selectindex > 0) && (S_UserMPageBuffer->selectindex <= MaxPageShowNum))
	{
		i = (S_UserMPageBuffer->pageindex-1)*MaxPageShowNum + S_UserMPageBuffer->selectindex-1;
	
		memcpy(&(S_UserMPageBuffer->tempnewuser), &(S_UserMPageBuffer->user[i]), sizeof(User_Type));
	}
	else
		memset(&(S_UserMPageBuffer->tempnewuser), 0, sizeof(User_Type));
	
	if(S_UserMPageBuffer->tempnewuser.crc == CalModbusCRC16Fun1(&(S_UserMPageBuffer->tempnewuser), sizeof(User_Type)-2))
	{	
		BasicPic(0x1d40, 1, 137, 266, 215, 559, 284, 157, 135+(S_UserMPageBuffer->selectindex-1)*72);	
	}
	else
		BasicPic(0x1d40, 0, 137, 266, 215, 559, 284, 157, 135+(S_UserMPageBuffer->selectindex-1)*72);
	
	sprintf(S_UserMPageBuffer->buf, "%s\0", S_UserMPageBuffer->tempnewuser.user_name);
	DisText(0x1d50, S_UserMPageBuffer->buf, strlen(S_UserMPageBuffer->buf)+1);
		
	sprintf(S_UserMPageBuffer->buf, "%s\0", S_UserMPageBuffer->tempnewuser.user_age);
	DisText(0x1d60, S_UserMPageBuffer->buf, strlen(S_UserMPageBuffer->buf)+1);
		
	sprintf(S_UserMPageBuffer->buf, "%s\0", S_UserMPageBuffer->tempnewuser.user_sex);
	DisText(0x1d70, S_UserMPageBuffer->buf, MaxSexLen);
		
	sprintf(S_UserMPageBuffer->buf, "%s\0", S_UserMPageBuffer->tempnewuser.user_phone);
	DisText(0x1d80, S_UserMPageBuffer->buf, strlen(S_UserMPageBuffer->buf)+1);
		
	sprintf(S_UserMPageBuffer->buf, "%s\0", S_UserMPageBuffer->tempnewuser.user_job);
	DisText(0x1d90, S_UserMPageBuffer->buf, strlen(S_UserMPageBuffer->buf)+1);
		
	sprintf(S_UserMPageBuffer->buf, "%s\0", S_UserMPageBuffer->tempnewuser.user_desc);
	DisText(0x1da0, S_UserMPageBuffer->buf, strlen(S_UserMPageBuffer->buf)+1);
}


static void AddANewUser(void)
{
	unsigned char i=0;
	
	if(S_UserMPageBuffer->tempnewuser.crc == CalModbusCRC16Fun1(&(S_UserMPageBuffer->tempnewuser), sizeof(User_Type)-2))
	{
		/*����Ƿ��Ѵ���*/
		S_UserMPageBuffer->tempuser = &(S_UserMPageBuffer->user[0]);
		
		for(i=0; i<MaxUserNum; i++)
		{
			if( (S_UserMPageBuffer->tempuser->crc == CalModbusCRC16Fun1(S_UserMPageBuffer->tempuser, sizeof(User_Type)-2)) &&
				(pdPASS == CheckStrIsSame(S_UserMPageBuffer->tempuser->user_name, S_UserMPageBuffer->tempnewuser.user_name, MaxNameLen)))
			{
				memcpy(S_UserMPageBuffer->tempuser, &(S_UserMPageBuffer->tempnewuser), sizeof(User_Type));
				break;
			}
			S_UserMPageBuffer->tempuser++;
		}
		
		/*��������ڣ����½�*/
		if(i >= MaxUserNum)
		{
			S_UserMPageBuffer->tempuser = &(S_UserMPageBuffer->user[0]);
		
			for(i=0; i<MaxUserNum; i++)
			{
				if(S_UserMPageBuffer->tempuser->crc != CalModbusCRC16Fun1(S_UserMPageBuffer->tempuser, sizeof(User_Type)-2))
				{
					memcpy(S_UserMPageBuffer->tempuser, &(S_UserMPageBuffer->tempnewuser), sizeof(User_Type));
					break;
				}
				S_UserMPageBuffer->tempuser++;
			}
		}
		
		/*����пռ�*/
		if(i < MaxUserNum)
		{
			/*�������ݣ��˴�Ϊ����ȫ�����������ݣ����Դ���������׵�ַ*/
			if(My_Fail == SaveUserData(&(S_UserMPageBuffer->user[0])))
			{
				SendKeyCode(2);
				ReadUserData(S_UserMPageBuffer->user);
			}
			else
			{
				//����ʧ�ܣ�������Ѹ��Ƶ�����
				memset(S_UserMPageBuffer->tempuser, 0, sizeof(User_Type));
				SendKeyCode(1);
			}
			
			memset(&(S_UserMPageBuffer->tempnewuser), 0, sizeof(User_Type));
			
			S_UserMPageBuffer->selectindex = 0;

			ShowList();
			ShowDetail();
		}
		else
			SendKeyCode(2);
	}
}

static void DeleteAUser(void)
{
	if((S_UserMPageBuffer->selectindex > 0) && (S_UserMPageBuffer->selectindex <= MaxPageShowNum))
	{
		S_UserMPageBuffer->tempuser = &(S_UserMPageBuffer->user[(S_UserMPageBuffer->pageindex-1)*MaxPageShowNum + S_UserMPageBuffer->selectindex-1]);
		memset(S_UserMPageBuffer->tempuser, 0, sizeof(User_Type));
		
		/*��������*/
		
		if(My_Fail == SaveUserData(&(S_UserMPageBuffer->user[0])))
			SendKeyCode(2);
		else
			SendKeyCode(1);
		
		memset(S_UserMPageBuffer->user, 0, sizeof(User_Type)*MaxUserNum);
		ReadUserData(S_UserMPageBuffer->user);
		
		S_UserMPageBuffer->pageindex = 1;
		S_UserMPageBuffer->selectindex = 0;
			
		ShowList();
		ShowDetail();
	}

}

