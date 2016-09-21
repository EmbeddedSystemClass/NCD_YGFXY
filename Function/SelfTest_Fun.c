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
#include	"System_Data.h"

#include	"MyMem.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"
#include	"semphr.h"

#include	"stdlib.h"
#include	<string.h>
/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/
static 	xQueueHandle xSelfCehckQueue;													//�����Լ��������
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
	GB_DataInit();
	vTaskDelay(500 *portTICK_RATE_MS);
	
//	ErWeiMaTest();
	vTaskDelay(1000 * portTICK_RATE_MS);
	
	DataBasesCheck();
	vTaskDelay(500 * portTICK_RATE_MS);
	
//	WifiModuleTest();
	vTaskDelay(500 * portTICK_RATE_MS);
	
	ADDACheck();
	vTaskDelay(500 *portTICK_RATE_MS);
	
	MotorCheck();
	vTaskDelay(500 *portTICK_RATE_MS);
	
	SetSelfCheckStatus(SelfCheckOver, 10*portTICK_RATE_MS);
}

/***************************************************************************************************
*FunctionName��ReadSelfCheckStatus
*Description��Get SelfTest Task State
*Input��receivedchar -- �������ݵ�ַ
*		xBlockTime -- �ȴ�ʱ��
*Output����ȡ״̬
*Author��xsx
*Data��2016��1��27��10:21:33
***************************************************************************************************/
MyState_TypeDef GetSelfCheckStatus(unsigned char * receivedchar , portTickType xBlockTime)
{
	if(xSelfCehckQueue == NULL)
		xSelfCehckQueue = xQueueCreate( 10, ( unsigned portBASE_TYPE ) sizeof( signed portCHAR ) );
	
	if(xSelfCehckQueue == NULL)
		return pdFAIL;
	
	return xQueueReceive( xSelfCehckQueue, receivedchar, xBlockTime );
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
MyState_TypeDef SetSelfCheckStatus(unsigned char txchar , portTickType xBlockTime)
{
	if(xSelfCehckQueue == NULL)
		xSelfCehckQueue = xQueueCreate( 10, ( unsigned portBASE_TYPE ) sizeof( signed portCHAR ) );
	
	if(xSelfCehckQueue == NULL)
		return pdFAIL;
	
	return xQueueSend( xSelfCehckQueue, &txchar, xBlockTime);
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
	SetSelfCheckStatus(DataBasesChecking, 10*portTICK_RATE_MS);
	vTaskDelay(1000*portTICK_RATE_MS);
	
	if(My_Pass == CheckSDFunction())
		SetSelfCheckStatus(DataBasesSuccess, 10*portTICK_RATE_MS);
	else
		SetSelfCheckStatus(DataBasesError, 10*portTICK_RATE_MS);
	
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
	SetSelfCheckStatus(ADDAChecking, 10*portTICK_RATE_MS);
	vTaskDelay(1000*portTICK_RATE_MS);
	
	if(My_Pass == ADDASelfTest())
	{
		SetSelfCheckStatus(ADDASuccess, 10*portTICK_RATE_MS);
	}
	else
		SetSelfCheckStatus(ADDAError, 10*portTICK_RATE_MS);
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
	SetSelfCheckStatus(ErWeiMaChecking, 10*portTICK_RATE_MS);
	vTaskDelay(1000*portTICK_RATE_MS);
	
	if(My_Pass == ErWeiMaSelfTest())
	{
		SetSelfCheckStatus(ErWeiMaSuccess, 10*portTICK_RATE_MS);
	}
	else
		SetSelfCheckStatus(ErWeiMaError, 10*portTICK_RATE_MS);
}

static void WifiModuleTest(void)
{
	SetSelfCheckStatus(WIFIChecking, 10*portTICK_RATE_MS);
	vTaskDelay(1000*portTICK_RATE_MS);
	
	if(My_Pass == WIFICheck())
	{
		SetSelfCheckStatus(WIFISuccess, 10*portTICK_RATE_MS);
	}
	else
		SetSelfCheckStatus(WIFIError, 10*portTICK_RATE_MS);
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
	SetSelfCheckStatus(MotorChecking, 10*portTICK_RATE_MS);
	vTaskDelay(1000*portTICK_RATE_MS);
	
	if(My_Pass == MotorSelfTest())
	{
		SetSelfCheckStatus(MotorSuccess, 10*portTICK_RATE_MS);
	}
	else
		SetSelfCheckStatus(MotorError, 10*portTICK_RATE_MS);
}

static void GB_DataInit(void)
{
	NetData *mynetdata = NULL;
	DeviceInfo *mydeviceinfo;
	
	SetSelfCheckStatus(ReadServerData, 10*portTICK_RATE_MS);
	vTaskDelay(1000*portTICK_RATE_MS);
	
	mynetdata = MyMalloc(sizeof(NetData));
	
	mydeviceinfo = MyMalloc(sizeof(DeviceInfo));
	
	if(mynetdata && mydeviceinfo)
	{
		memset(mynetdata, 0, sizeof(NetData));
		memset(mydeviceinfo, 0, sizeof(DeviceInfo));
		
		//��ȡϵͳ��������
		ReadNetData(mynetdata);
		SetGB_NetConfigureData(mynetdata);

		//��ȡ�豸��Ϣ
		ReadDeviceInfo(mydeviceinfo);
		SetGB_DeviceInfo(mydeviceinfo);
	}
	
	MyFree(mynetdata);
	MyFree(mydeviceinfo);
}
