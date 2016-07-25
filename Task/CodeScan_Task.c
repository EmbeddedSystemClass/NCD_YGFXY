/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"CodeScan_Task.h"
#include	"CodeScanFunction.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"
#include	"semphr.h"
/******************************************************************************************/
/*****************************************�ֲ���������*************************************/

#define vCodeScanTask_PRIORITY			( ( unsigned portBASE_TYPE ) 2U )
const char * CodeScanTaskName = "vCodeScanTask";

static xQueueHandle xStartScanQueue = NULL ;			//��������������յ����ݣ���ʼɨ���ά�룬���յ�����Ϊ��ά���ŵ�ַ
static xQueueHandle xScanResultQueue = NULL;											//����ɨ����
/******************************************************************************************/
/*****************************************�ֲ���������*************************************/

static void vCodeScanTask( void *pvParameters );

/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/

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
MyState_TypeDef StartCodeScanTask(void)
{
	if(xStartScanQueue == NULL)
		xStartScanQueue = xQueueCreate(1, sizeof(void *));
	
	if(xStartScanQueue == NULL)
		return My_Fail;
	
	if(xScanResultQueue == NULL)
		xScanResultQueue = xQueueCreate(1, sizeof(ScanCodeResult));
	
	if(xScanResultQueue == NULL)
		return My_Fail;
	
	xTaskCreate( vCodeScanTask, CodeScanTaskName, configMINIMAL_STACK_SIZE+100, NULL, vCodeScanTask_PRIORITY, NULL );
	
	return My_Pass;
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
static void vCodeScanTask( void *pvParameters )
{
	void * parm;
	ScanCodeResult scanresult;
	
	while(1)
	{
		if(pdPASS == xQueueReceive( xStartScanQueue, &parm, portMAX_DELAY))
		{
			vTaskDelay(1000 * portTICK_RATE_MS);
			scanresult = ScanCodeFun(parm);										//��ȡ��ά��
			vTaskDelay(1000 * portTICK_RATE_MS);
			
			/*���Ͳ��Խ��*/
			xQueueSend( xScanResultQueue, &scanresult, 10000/portTICK_RATE_MS );
				
			parm = NULL;
		}
		
		vTaskDelay(1000 * portTICK_RATE_MS);
	}
}

/***************************************************************************************************
*FunctionName��StartScanQRCode
*Description����ʼ��ȡ��ά�룬��ɨ�������������״̬
*Input��parm -- ��ȡ��ά��ĵ�ַ�������Ӧ���Ǵ�Ŷ�ά�����ݵĵ�ַ�����Զ��з��͵�ʱ���͵��Ǵ�Ŷ�ά���ַ��ָ��ĵ�ַ
*Output��None
*Author��xsx
*Data��2016��7��11��14:56:57
***************************************************************************************************/
MyState_TypeDef StartScanQRCode(void * parm)
{
	if(pdPASS == xQueueSend( xStartScanQueue, &parm, 10*portTICK_RATE_MS))
		return My_Pass;
	else
		return My_Fail;	
}

MyState_TypeDef TakeScanQRCodeResult(ScanCodeResult *scanresult)
{
	if(pdPASS == xQueueReceive( xScanResultQueue, scanresult,  10/portTICK_RATE_MS))
		return My_Pass;
	else
		return My_Fail;
}

