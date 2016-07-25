/***************************************************************************************************
*FileName:SelfCheck_Task
*Description:�Լ�����
*Author:xsx
*Data:2016��4��27��10:42:43
***************************************************************************************************/

/***************************************************************************************************/
/******************************************ͷ�ļ�***************************************************/
/***************************************************************************************************/
#include	"SelfCheck_Task.h"
#include	"SelfTest_Fun.h"


#include 	"queue.h"
#include	"semphr.h"
/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/
#define vSelfCheckTask_PRIORITY			( ( unsigned portBASE_TYPE ) 2U )
const char * SelfCheckTaskName = "vSelfCheckTask";

static xSemaphoreHandle xTaskMutex = NULL ;									//�Լ���������
/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/
static portBASE_TYPE vSelfTestTaskInit(void);
static void vSelfCheckTask( void *pvParameters );
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************����********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/************************************************************************
** ������:
** ��  ��:
** ��  ��:��
** ��  ��:��
** ��  �أ���
** ��  ע����
** ʱ  ��:  
** ��  �ߣ�xsx                                                 
************************************************************************/
void StartSelfCheckTask(void)
{
	/*�����������������*/
	if(xTaskMutex == NULL)
		xTaskMutex = xSemaphoreCreateMutex();
	
	if(xTaskMutex == NULL)
		return;
	
	while(pdPASS == xSemaphoreTake(xTaskMutex , 10*portTICK_RATE_MS));			//����ź���

	if(pdFAIL == vSelfTestTaskInit())
		return;
	
	xTaskCreate( vSelfCheckTask, SelfCheckTaskName, configMINIMAL_STACK_SIZE, NULL, vSelfCheckTask_PRIORITY, NULL );
}


static portBASE_TYPE vSelfTestTaskInit(void)
{
	return SelfTestFun_Init();
}

/************************************************************************
** ������:
** ��  ��:
** ��  ��:��
** ��  ��:��
** ��  �أ���
** ��  ע����
** ʱ  ��:  
** ��  �ߣ�xsx                                                 
************************************************************************/
static void vSelfCheckTask( void *pvParameters )
{
	while(1)
	{
		if(pdPASS == xSemaphoreTake(xTaskMutex , portMAX_DELAY))
		{
			SelfTest_Function();
			vTaskDelay(1000 * portTICK_RATE_MS);
		}
		
		vTaskDelete(NULL);
	}
}

/***************************************************************************************************
*FunctionName��SelfCheckTaskRun
*Description�������Լ�����
*Input��None
*Output��None
*Author��xsx
*Data��2016��4��30��15:48:04
***************************************************************************************************/
void SelfCheckTaskRun(void)
{
	/*�����������������*/
	if(xTaskMutex == NULL)
	{
		xTaskMutex = xSemaphoreCreateMutex();
		
		if(xTaskMutex == NULL)
			return;
		
		while(pdPASS == xSemaphoreTake(xTaskMutex , 10*portTICK_RATE_MS));			//����ź���
	}
	xSemaphoreGive(xTaskMutex);
}

/***************************************************************************************************
*FunctionName��GetSelfTestTaskState
*Description��Get SelfTest Task State
*Input��receivedchar -- �������ݵ�ַ
*		xBlockTime -- �ȴ�ʱ��
*Output����ȡ״̬
*Author��xsx
*Data��2016��1��27��10:21:33
***************************************************************************************************/
portBASE_TYPE GetSelfTestTaskState(unsigned char * receivedchar , portTickType xBlockTime)
{
	return GetSelfTestFunState(receivedchar, xBlockTime);

}





