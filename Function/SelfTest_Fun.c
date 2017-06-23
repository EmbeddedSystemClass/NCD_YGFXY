/***************************************************************************************************
*FileName��SelfTest_Fun
*Description���Լ칦��
*Author��xsx
*Data��2016��1��27��10:29:14
***************************************************************************************************/

/***************************************************************************************************/
/******************************************ͷ�ļ�***************************************************/
/***************************************************************************************************/
#include	"SelfTest_Fun.h"

#include	"CodeScanner_Driver.h"
#include	"SDFunction.h"
#include	"WifiFunction.h"
#include	"Ads8325_Driver.h"
#include	"TLV5617_Driver.h"
#include	"MAX4051_Driver.h"
#include	"Motor_Fun.h"
#include	"Motor_Data.h"
#include	"CardLimit_Driver.h"
#include	"DRV8825_Driver.h"

#include	"Define.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"
#include	"semphr.h"

#include	"stdlib.h"
/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/
static 	xQueueHandle xTaskQueue;		//�����Լ�״̬����
/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/
static void DataBasesCheck(void);
static void ErWeiMaTest(void);
static void WifiModuleTest(void);

static MyState_TypeDef ADDASelfTest(void);
static void ADDACheck(void);

static MyState_TypeDef MotorSelfTest(void);
static void MotorCheck(void);

static void GB_DataInit(void);
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************����********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************
*FunctionName��SelfTest_Function
*Description���Լ칦��ʵ��
*Input��None
*Output��None
*Author��xsx
*Data��2016��1��27��10:38:06
***************************************************************************************************/
void SelfTest_Function(void)
{		
	SelfTestFun_Init();
	
	WifiModuleTest();
	vTaskDelay(500 * portTICK_RATE_MS);

	SetSelfTestFunState(SelfCheckOver, 10*portTICK_RATE_MS);
}

/***************************************************************************************************
*FunctionName��
*Description��
*Input��
*Output��
*Author��
*Data��
***************************************************************************************************/
portBASE_TYPE SelfTestFun_Init(void)
{
	if(xTaskQueue == NULL)
	{	
		xTaskQueue = xQueueCreate( 10, ( unsigned portBASE_TYPE ) sizeof( signed portCHAR ) );
		
		if(xTaskQueue == NULL)
			return pdFAIL;
		
		while(pdPASS == xSemaphoreTake(xTaskQueue , 0));			//����ź���
	}
	
	return pdPASS;
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
portBASE_TYPE GetSelfTestFunState(unsigned char * receivedchar , portTickType xBlockTime)
{
	return xQueueReceive( xTaskQueue, receivedchar, xBlockTime );

}
/***************************************************************************************************
*FunctionName��SetSelfTestTaskState
*Description�������Լ�״̬
*Input��txchar -- �����µ��Լ�״̬
*		xBlockTime -- д��ȴ�ʱ��
*Output��д��״̬
*Author��xsx
*Data��2016��1��27��10:22:39
***************************************************************************************************/
portBASE_TYPE SetSelfTestFunState(unsigned char txchar , portTickType xBlockTime)
{
	return xQueueSend( xTaskQueue, &txchar, xBlockTime);
}


/***************************************************************************************************
*FunctionName��DataBasesCheck
*Description���洢ģ���Լ�
*Input��None
*Output��None
*Author��xsx
*Data��2016��1��27��13:37:34
***************************************************************************************************/
static void DataBasesCheck(void)
{
	//���ڼ��洢ģ��
	SetSelfTestFunState(DataBasesChecking, 10*portTICK_RATE_MS);
	vTaskDelay(1000*portTICK_RATE_MS);
	
	if(My_Pass == CheckSDFunction())
		SetSelfTestFunState(DataBasesSuccess, 10*portTICK_RATE_MS);
	else
		SetSelfTestFunState(DataBasesError, 10*portTICK_RATE_MS);
	
	vTaskDelay(1000*portTICK_RATE_MS);
	
}

/***************************************************************************************************
*FunctionName��ADDASelfTest
*Description���ɼ�ģ���Լ�
*Input��None
*Output��None
*Author��xsx
*Data��2016��6��27��17:21:14
***************************************************************************************************/
static MyState_TypeDef ADDASelfTest(void)
{
	static double tempvalue1 = 0.0, tempvalue2 = 0.0;
	
	SetGB_LedValue(0);
	vTaskDelay(100 / portTICK_RATE_MS);	
	SetGB_CLineValue(0);
	vTaskDelay(100 / portTICK_RATE_MS);	
	SelectChannel(0);
	vTaskDelay(100 / portTICK_RATE_MS);
	
	tempvalue1 = ADS8325();
	vTaskDelay(100 / portTICK_RATE_MS);
	
	SetGB_LedValue(300);
	vTaskDelay(100 / portTICK_RATE_MS);
	tempvalue2 = ADS8325();
	vTaskDelay(100 / portTICK_RATE_MS);
	if(tempvalue2 <= tempvalue1)
		return My_Fail;
	
	SetGB_CLineValue(5);
	vTaskDelay(100 / portTICK_RATE_MS);
	tempvalue1 = ADS8325();
	vTaskDelay(100 / portTICK_RATE_MS);
	if(tempvalue2 < tempvalue1)
		return My_Fail;
	
	SelectChannel(7);
	vTaskDelay(100 / portTICK_RATE_MS);
	tempvalue2 = ADS8325();
	vTaskDelay(100 / portTICK_RATE_MS);	
	if(tempvalue2 < tempvalue1)
		return My_Fail;
	else
		return My_Pass;
}
/***************************************************************************************************
*FunctionName��ADDACheck
*Description���ɼ�ģ���Լ�
*Input��None
*Output��None
*Author��xsx
*Data��2016��5��2��09:39:09
***************************************************************************************************/
static void ADDACheck(void)
{
	SetSelfTestFunState(ADDAChecking, 10*portTICK_RATE_MS);
	vTaskDelay(1000*portTICK_RATE_MS);
	
	if(My_Pass == ADDASelfTest())
	{
		SetSelfTestFunState(ADDASuccess, 10*portTICK_RATE_MS);
	}
	else
		SetSelfTestFunState(ADDAError, 10*portTICK_RATE_MS);
}

/***************************************************************************************************
*FunctionName��MotorTest
*Description���������
*Input��None
*Output��None
*Author��xsx
*Data��2016��1��27��13:37:34
***************************************************************************************************/
static void ErWeiMaTest(void)
{
	SetSelfTestFunState(ErWeiMaChecking, 10*portTICK_RATE_MS);
	vTaskDelay(1000*portTICK_RATE_MS);
	
	if(My_Pass == ErWeiMaSelfTest())
	{
		SetSelfTestFunState(ErWeiMaSuccess, 10*portTICK_RATE_MS);
	}
	else
		SetSelfTestFunState(ErWeiMaError, 10*portTICK_RATE_MS);
}

static void WifiModuleTest(void)
{
	SetSelfTestFunState(WIFIChecking, 10*portTICK_RATE_MS);
	vTaskDelay(1000*portTICK_RATE_MS);
	
	if(My_Pass == WIFICheck())
	{
		SetSelfTestFunState(WIFISuccess, 10*portTICK_RATE_MS);
	}
	else
		SetSelfTestFunState(WIFIError, 10*portTICK_RATE_MS);
}

/***************************************************************************************************
*FunctionName��MotorSelfTest
*Description������Լ�
*Input��None
*Output��None
*Author��xsx
*Data��2016��6��27��17:39:36
***************************************************************************************************/
static MyState_TypeDef MotorSelfTest(void)
{
	static unsigned char count = 0;
	
	SetDRVPowerStatues(LowPower);

	SetGB_MotorLocation(10000);
	MotorMoveTo(0, 1);
	vTaskDelay(100 / portTICK_RATE_MS);
	
	while(!BackLimited)
	{
		vTaskDelay(500 / portTICK_RATE_MS);
		
		count++;
		if(count > 6)
			break;
	}
	
	if(count > 6)
	{
		StopMotor();
		return My_Fail;
	}
	
	MotorMoveTo(10000, 1);
	vTaskDelay(100 / portTICK_RATE_MS);

	SetDRVPowerStatues(NonamalPower);
	vTaskDelay(100 / portTICK_RATE_MS);
	
	count = 0;
	while(!PreLimited)
	{
		vTaskDelay(500 / portTICK_RATE_MS);
		count++;
		if(count > 8)
			break;
	}
	
	if(count > 6)
	{
		StopMotor();
		return My_Fail;
	}
	
	return My_Pass;
}
/***************************************************************************************************
*FunctionName��MotorCheck
*Description������Լ�
*Input��None
*Output��None
*Author��xsx
*Data��2016��6��27��17:39:59
***************************************************************************************************/
static void MotorCheck(void)
{
	SetSelfTestFunState(MotorChecking, 10*portTICK_RATE_MS);
	vTaskDelay(1000*portTICK_RATE_MS);
	
	if(My_Pass == MotorSelfTest())
	{
		SetSelfTestFunState(MotorSuccess, 10*portTICK_RATE_MS);
	}
	else
		SetSelfTestFunState(MotorError, 10*portTICK_RATE_MS);
}

static void GB_DataInit(void)
{
	SetSelfTestFunState(ReadServerData, 10*portTICK_RATE_MS);
	vTaskDelay(1000*portTICK_RATE_MS);
	
	if(My_Pass == ReadNetData(GetGB_NetData()))
		SetSelfTestFunState(ReadServerSuccess, 10*portTICK_RATE_MS);
	else
		SetSelfTestFunState(ReadServerError, 10*portTICK_RATE_MS);
}
