/***************************************************************************************************
*FileName:
*Description:
*Author: xsx_kair
*Data:
***************************************************************************************************/

/***************************************************************************************************/
/******************************************Header List********************************************/
/***************************************************************************************************/
#include	"Test_Task.h"
#include	"Test_Fun.h"
#include	"System_Data.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"
#include	"semphr.h"

#include	<string.h>
#include	"stdio.h"
#include 	"stdlib.h"
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
#define TestTask_PRIORITY			2
const char * TestTaskName = "vTestTask";

static xQueueHandle xStartTestQueue = NULL ;						//ɨ�����ݿռ���У�����������ɨ������
static TestData * testdata;											//ɨ������ָ��

static xQueueHandle xTestResultQueue = NULL;						//ɨ���������
static ResultState resultstatues;									//ɨ�����
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
static void vTestTask( void *pvParameters );
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/****************************************File Start*************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

MyState_TypeDef StartvTestTask(void)
{
	//�����������ݶ��У�ͬʱ������������
	if(xStartTestQueue == NULL)
		xStartTestQueue = xQueueCreate(1, sizeof(void *));
	
	if(xStartTestQueue == NULL)
		return My_Fail;
	
	//��������������
	if(xTestResultQueue == NULL)
		xTestResultQueue = xQueueCreate(1, sizeof(ResultState));
	
	if(xTestResultQueue == NULL)
		return My_Fail;
	
	if(pdFAIL == xTaskCreate( vTestTask, TestTaskName, configMINIMAL_STACK_SIZE, NULL, TestTask_PRIORITY, NULL ))
		return My_Fail;
	else
		return My_Pass;
}


static void vTestTask( void *pvParameters )
{
	while(1)
	{
		if(pdPASS == xQueueReceive( xStartTestQueue, &testdata, portMAX_DELAY))
		{
			while(pdPASS == TakeTestResult(&resultstatues))
				;
			
			#if (NormalCode != CodeType)
			
				SetTestStatusFlorLab(1);
			#endif
			resultstatues = TestFunction(testdata);
			
			xQueueSend( xTestResultQueue, &resultstatues, 1000/portTICK_RATE_MS );
			
			#if (NormalCode != CodeType)
			
				SetTestStatusFlorLab(0);
			#endif
		}
	}
}


MyState_TypeDef StartTest(void * parm)
{
	if(pdPASS == xQueueSend( xStartTestQueue, &parm, 10/portTICK_RATE_MS ))
		return My_Pass;
	else
		return My_Fail;	
}


MyState_TypeDef TakeTestResult(ResultState *testsult)
{
	if(pdPASS == xQueueReceive( xTestResultQueue, testsult,  10/portTICK_RATE_MS))
		return My_Pass;
	else
		return My_Fail;
}

/****************************************end of file************************************************/
