/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"CodeScan_Task.h"
#include	"CodeScanFunction.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"
#include	"semphr.h"

#include	<string.h>
#include	"stdio.h"
#include 	"stdlib.h"
/******************************************************************************************/
/*****************************************�ֲ���������*************************************/

#define vCodeScanTask_PRIORITY			( ( unsigned portBASE_TYPE ) 2U )
const char * CodeScanTaskName = "vCodeScanTask";

static xQueueHandle xStartScanQueue = NULL ;			//��������������յ����ݣ���ʼɨ���ά�룬���յ�����Ϊ��ά���ŵ�ַ
static xQueueHandle xScanResultQueue = NULL;											//����ɨ����
static ScanQRTaskData S_ScanQRTaskData;					//��ά��ɨ����������
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
	while(1)
	{
		if(pdPASS == xQueueReceive( xStartScanQueue, (void *)(&(S_ScanQRTaskData.cardQR)), portMAX_DELAY))
		{
			TakeScanQRCodeResult(&(S_ScanQRTaskData.scanresult));
			vTaskDelay(1000 * portTICK_RATE_MS);
			ScanCodeFun(&S_ScanQRTaskData);										//��ȡ��ά��
			vTaskDelay(1000 * portTICK_RATE_MS);
			
			/*���Ͳ��Խ��*/
			xQueueSend( xScanResultQueue, &(S_ScanQRTaskData.scanresult), 10000/portTICK_RATE_MS );
				
			memset(&S_ScanQRTaskData, 0, sizeof(ScanQRTaskData));
		}
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

MyState_TypeDef StopScanQRCode(void)
{
	S_ScanQRTaskData.scanresult = CardCodeScanFail;
	return My_Pass;
}

MyState_TypeDef TakeScanQRCodeResult(ScanCodeResult *scanresult)
{
	if(pdPASS == xQueueReceive( xScanResultQueue, scanresult,  10/portTICK_RATE_MS))
		return My_Pass;
	else
		return My_Fail;
}

