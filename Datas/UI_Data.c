/***************************************************************************************************
*FileName:
*Description:
*Author:xsx
*Data:
***************************************************************************************************/


/***************************************************************************************************/
/******************************************ͷ�ļ�***************************************************/
/***************************************************************************************************/

#include	"UI_Data.h"

#include	"MyMem.h"

#include	<string.h>
#include	"stdio.h"

/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/

static LinkStack GB_ActivityLinkStack;	


/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************����********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************
*FunctionName: startActivity
*Description: ����һ���µ�activity
*Input: pageCreate -- activity�Ĵ�������
*		pram -- ����Ĳ���
*Output: 
*Return: MyState_TypeDef -- �����ɹ����
*Author: xsx
*Date: 2016��12��20��11:16:59
***************************************************************************************************/
MyState_TypeDef startActivity(MyState_TypeDef (* pageCreate)(Activity * thizactivity, void * pram), void * pram)
{
	Activity * activity = NULL;
	
	if(NULL == pageCreate)
		return My_Fail;
	
	//Ϊ��ҳ�������ڴ�
	activity = MyMalloc(sizeof(Activity));
	
	if(activity)
	{
		memset(activity, 0, sizeof(Activity));
		
		activity->pageCreate = pageCreate;
		
		//��ҳ����ջ
		if(My_Pass == StackPush(&GB_ActivityLinkStack, activity))
		{
			//ִ��ҳ�洴��,�����ɹ�����ʾ
			if(My_Pass == activity->pageCreate(activity, pram))
			{
				//��ҳ����ʾ
				if(activity->pageStart)
					activity->pageStart();
				
				return My_Pass;
			}
			//����ʧ�ܣ����ջ��ҳ�棬������
			else
				StackPop(&GB_ActivityLinkStack, NULL, true);
		}
		
		//��ջʧ�ܣ�������
		MyFree(activity);
	}
	
	return My_Fail;
}

/***************************************************************************************************
*FunctionName: backToActivity
*Description: ����
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��20��11:53:00
***************************************************************************************************/
MyState_TypeDef backToActivity(char * pageName)
{
	Activity * activity = NULL;
	
	if(NULL == pageName)
		return My_Fail;
	
	while(My_Pass == StackTop(&GB_ActivityLinkStack, &activity))
	{
		if(0 == strcmp(activity->pageName, pageName))
		{
			if(activity->pageResume)
				activity->pageResume();
			
			return My_Pass;
		}
		else
			StackPop(&GB_ActivityLinkStack, NULL, false);
	}
	
	return My_Fail;
}

/***************************************************************************************************
*FunctionName: InitActivity
*Description: ��ʼ��һ��ҳ����¼�����
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��20��16:26:14
***************************************************************************************************/
void InitActivity(Activity * activity, char * activityName, void (* pageStart)(void), 
	void (* pageInput)(unsigned char *pbuf , unsigned short len), 
	void (* pageFresh)(void),
	void (* pageHide)(void),
	void (* pageResume)(void),
	void (* pageDestroy)(void))
{
	activity->pageName = activityName;
	activity->pageStart = pageStart;
	activity->pageInput = pageInput;
	activity->pageFresh = pageFresh;
	activity->pageHide = pageHide;
	activity->pageResume = pageResume;
	activity->pageDestroy = pageDestroy;
}

/***************************************************************************************************
*FunctionName: activityInputFunction
*Description: UIϵͳ������ӿ�
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��20��16:27:52
***************************************************************************************************/
void activityInputFunction(unsigned char *pbuf , unsigned short len)
{
	Activity * activity = NULL;
	
	if(My_Pass == StackTop(&GB_ActivityLinkStack, &activity))
	{
		if( (activity) && (activity->pageInput))
			activity->pageInput(pbuf, len);
	}
}

/***************************************************************************************************
*FunctionName: activityFreshFunction
*Description: UIϵͳ��ˢ�½ӿ�
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��20��16:28:30
***************************************************************************************************/
void activityFreshFunction(void)
{
	Activity * activity = NULL;
	
	if(My_Pass == StackTop(&GB_ActivityLinkStack, &activity))
	{
		if( (activity) && (activity->pageFresh))
			activity->pageFresh();
	}
}

