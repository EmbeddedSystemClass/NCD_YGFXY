/***************************************************************************************************
*FileName : TestLinks_Data
*Description: ������������
*Author:xsx
*Data: 2016��5��14��17:14:02
***************************************************************************************************/

/***************************************************************************************************/
/******************************************ͷ�ļ�***************************************************/
/***************************************************************************************************/
#include	"MyTest_Data.h"

#include	"MyMem.h"
#include	"string.h"
#include 	"FreeRTOS.h"
#include 	"task.h"
/***************************************************************************************************/
/**************************************����*************************************************/
/***************************************************************************************************/
//������Ի���
static ItemData * GB_NormalTestDataBuffer = NULL;
//�������Ի���
static ItemData * (GB_PaiduiTestDataBuffer[PaiDuiWeiNum]) = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
//��ǰ����������
static ItemData * GB_CurrentTestDataBuffer = NULL;
/***************************************************************************************************/
/**************************************�ڲ�����*************************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************����********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************
*FunctionName: IsPaiDuiTestting
*Description: ����Ƿ�ǰ�ڽ�����������
*Input: None
*Output: None
*Return: 	true -- �п����ŶӲ�����
*			false -- û�н�����������
*Author: xsx
*Date: 2016��12��2��15:10:07
***************************************************************************************************/
bool IsPaiDuiTestting(void)
{
	unsigned char i=0;

	for(i=0; i<PaiDuiWeiNum; i++)
	{
		if(GB_PaiduiTestDataBuffer[i] != NULL)
			return true;
	}
	
	return false;
}

CreateTestErrorType CreateANewTest(TestType testtype)
{
	unsigned char i=0;
	
	//����ǳ������
	if(testtype == NormalTestType)
	{
		//����������Թ�����ʹ���У����ֹ�������
		if(true == IsPaiDuiTestting())
			return Error_StopNormalTest;
		
		//��������ڴ�
		GB_NormalTestDataBuffer = (ItemData *)MyMalloc(sizeof(ItemData));
		
		//�ڴ�����ʧ��
		if(GB_NormalTestDataBuffer == NULL)
			return Error_Mem;
		else
		{
			GB_CurrentTestDataBuffer = GB_NormalTestDataBuffer;
			memset(GB_NormalTestDataBuffer, 0, sizeof(ItemData));
			return Error_OK;
		}
	}
	else
	{
		for(i=0; i<PaiDuiWeiNum; i++)
		{
			if(GB_PaiduiTestDataBuffer[i] == NULL)
			{
				//��������ڴ�
				GB_PaiduiTestDataBuffer[i] = (ItemData *)MyMalloc(sizeof(ItemData));
				
				//�ڴ�����ʧ��
				if(GB_PaiduiTestDataBuffer[i] == NULL)
					return Error_Mem;
				else
				{
					GB_CurrentTestDataBuffer = GB_PaiduiTestDataBuffer[i];
					memset(GB_CurrentTestDataBuffer, 0, sizeof(ItemData));
					return Error_OK;
				}
			}
		}
		
		return Error_PaiDuiBusy;
	}
}




ItemData * GetTestItemByIndex(unsigned char index)
{
	return GB_PaiduiTestDataBuffer[index];
}


unsigned short GetMinWaitTime(void)
{
	unsigned char index = 0;
	unsigned short min = 0xffff;
	unsigned short temp = 0;
	
	for(index = 0; index < PaiDuiWeiNum; index++)
	{
		if((GB_PaiduiTestDataBuffer[index])&&((GB_PaiduiTestDataBuffer[index]->statues == statues3) || ((GB_PaiduiTestDataBuffer[index]->statues == statues4))))
		{
			temp = timer_surplus(&(GB_PaiduiTestDataBuffer[index]->timer));
			if(temp < min)
				min = temp;
		}
	}
	
	return min;
}


/***************************************************************************************************
*FunctionName: SetCurrentTestItem, GetCurrentTestItem
*Description: ��д��ǰ���Ի���
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��2��15:54:25
***************************************************************************************************/
void SetCurrentTestItem(ItemData * s_itemdata)
{
	GB_CurrentTestDataBuffer = s_itemdata;
}

ItemData * GetCurrentTestItem(void)
{
	return GB_CurrentTestDataBuffer;
}

/***************************************************************************************************
*FunctionName: DeleteCurrentTest
*Description: ɾ����ǰ����
*Input: None
*Output: None
*Return: My_Pass -- ɾ���ɹ�
*Author: xsx
*Date: 2016��12��2��15:55:07
***************************************************************************************************/
MyState_TypeDef DeleteCurrentTest(void)
{
	ItemData * temp = GetCurrentTestItem();
	unsigned char i=0;
	if(temp)
	{
		memset(temp, 0, sizeof(ItemData));
		MyFree(temp);

		for(i=0; i<PaiDuiWeiNum; i++)
		{
			if(GB_PaiduiTestDataBuffer[i] == temp)
			{
				GB_PaiduiTestDataBuffer[i] = NULL;
				SetCurrentTestItem(NULL);
			}
		}
	}
	return My_Pass;
}

