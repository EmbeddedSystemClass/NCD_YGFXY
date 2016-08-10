/***************************************************************************************************
*FileName��Test_Task
*Description����������
*Author��xsx
*Data��2015��8��26��17:06:08
***************************************************************************************************/

/***************************************************************************************************/
/******************************************ͷ�ļ�***************************************************/
/***************************************************************************************************/

#include	"Test_Task.h"


#include 	"task.h"
#include 	"queue.h"
#include	"semphr.h"

#include	"stdio.h"

/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/

#define TestTask_PRIORITY			2
const char * TestTaskName = "vTestTask";

static xQueueHandle xStartTestQueue = NULL ;			//��������������յ����ݣ���ʼ���ԣ����յ�����Ϊ�������ݱ����ַ
static xQueueHandle xTestResultQueue = NULL;											//���Ͳ��Խ��
/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/
static void vTestTask( void *pvParameters );

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************����********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************
*FunctionName��StartvSysLedTask
*Description������ϵͳָʾ������
*Input��None
*Output��None
*Author��xsx
*Data��2015��8��26��16:58:13
***************************************************************************************************/
MyState_TypeDef StartvTestTask(void)
{
	if(xStartTestQueue == NULL)
		xStartTestQueue = xQueueCreate(1, sizeof(void *));
	
	if(xStartTestQueue == NULL)
		return My_Fail;
	
	if(xTestResultQueue == NULL)
		xTestResultQueue = xQueueCreate(1, sizeof(ResultState));
	
	if(xTestResultQueue == NULL)
		return My_Fail;
	
	if(My_Fail == InitTestFunData())
		return My_Fail;
	
	if(pdFAIL == xTaskCreate( vTestTask, TestTaskName, configMINIMAL_STACK_SIZE, NULL, TestTask_PRIORITY, NULL ))
		return My_Fail;
	else
		return My_Pass;
}

/***************************************************************************************************
*FunctionName��vSysLedTask
*Description��ϵͳָʾ����˸���������������
*Input��None
*Output��None
*Author��xsx
*Data��2015��8��26��16:58:46
***************************************************************************************************/
static void vTestTask( void *pvParameters )
{
	void * parm = NULL;
	ResultState testresult = NoResult;
	
	while(1)
	{
		if(pdPASS == xQueueReceive( xStartTestQueue, &parm, portMAX_DELAY))
		{
			while(pdPASS == TakeTestResult(&testresult))
				;
			
			testresult = TestFunction(parm);
			
			/*���Ͳ��Խ��*/
			xQueueSend( xTestResultQueue, &testresult, 10000/portTICK_RATE_MS );
				
			parm = NULL;
		}
		
		vTaskDelay(500 * portTICK_RATE_MS);
	}
}

/***************************************************************************************************
*FunctionName��SetTestTaskRun
*Description����������
*Input��parm -- �������ݵĵ�ַ�������Ӧ���Ǵ�Ų������ݵĵ�ַ�����Զ��з��͵�ʱ���͵��Ǵ�Ų������ݵ�ָ��ĵ�ַ(��������ָ�������������ݣ�������ά��)
*Output��None
*Author��xsx
*Data��2016��5��14��17:26:56
***************************************************************************************************/
MyState_TypeDef StartTest(void * parm)
{
	if(pdPASS == xQueueSend( xStartTestQueue, &parm, 10*portTICK_RATE_MS ))
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
