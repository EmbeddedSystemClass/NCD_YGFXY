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

#include	"LEDCheck_Driver.h"
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
#include	"SystemSet_Data.h"

#include	"SystemSet_Dao.h"
#include	"MyMem.h"
#include	"CRC16.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"
#include	"semphr.h"

#include	"stdlib.h"
#include	<string.h>

/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/
static xQueueHandle xSelfTestStatusQueue = NULL;							//����ÿ���Լ�״̬
/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/
static MyState_TypeDef sendSelfTestStatus(ERROR_SelfTest selfTest);
static MyState_TypeDef loadSystemData(void);
static MyState_TypeDef testLed(void);
static MyState_TypeDef testADModel(void);
static MyState_TypeDef testMotol(void);
MyState_TypeDef testErWeiMa(void);
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************����********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************
*FunctionName: readSelfTestStatus
*Description: ��ȡ�Լ�״̬
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��23��09:02:33
***************************************************************************************************/
MyState_TypeDef readSelfTestStatus(ERROR_SelfTest * selfTest)
{
	if(NULL == xSelfTestStatusQueue)
		xSelfTestStatusQueue = xQueueCreate(10, sizeof(ERROR_SelfTest));
	
	if(pdPASS == xQueueReceive( xSelfTestStatusQueue, selfTest,  0/portTICK_RATE_MS))
		return My_Pass;
	else
		return My_Fail;
}

/***************************************************************************************************
*FunctionName: sendSelfTestStatus
*Description: �����Լ�״̬
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��23��09:04:23
***************************************************************************************************/
static MyState_TypeDef sendSelfTestStatus(ERROR_SelfTest selfTest)
{
	if(NULL == xSelfTestStatusQueue)
		xSelfTestStatusQueue = xQueueCreate(10, sizeof(ERROR_SelfTest));
	
	if(pdPASS == xQueueSend( xSelfTestStatusQueue, &selfTest,  10/portTICK_RATE_MS))
		return My_Pass;
	else
		return My_Fail;
}

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
	//����ϵͳ���ݣ����������ؽ��,�������ʧ�ܣ�����ֹ�Լ����
	if(My_Pass == loadSystemData())
		sendSelfTestStatus(SystemData_OK);
	else
	{
		sendSelfTestStatus(SystemData_ERROR);
		return;
	}
	
	//���Բɼ�ģ��
	if(My_Pass == testADModel())
		sendSelfTestStatus(AD_OK);
	else
	{
		sendSelfTestStatus(AD_ERROR);
		return;
	}
	
	//���led
	if(My_Pass == testLed())
		sendSelfTestStatus(Light_OK);
	else
	{
		sendSelfTestStatus(Light_Error);
		return;
	}
	
/*	//���Զ�ά��
	if(My_Pass == testErWeiMa())
		sendSelfTestStatus(Erweima_OK);
	else
	{
		sendSelfTestStatus(Erweima_ERROR);
		return;
	}*/

	//���Դ���ģ��
	if(My_Pass == testMotol())
		sendSelfTestStatus(Motol_OK);
	else
	{
		sendSelfTestStatus(Motol_ERROR);
		return;
	}
	
	WIFIInit();
	
	//�Լ���ɣ����ͽ��
	sendSelfTestStatus(SelfTest_OK);
}

/***************************************************************************************************
*FunctionName: loadSystemData
*Description: ����ϵͳ����
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��23��09:06:16
***************************************************************************************************/
static MyState_TypeDef loadSystemData(void)
{
	SystemSetData * systemSetData = NULL;
	MyState_TypeDef status = My_Fail;
	
	systemSetData = MyMalloc(sizeof(SystemSetData));
	
	if(systemSetData)
	{
		//��ȡSD���е������ļ�
		ReadSystemSetData(systemSetData);
		
		//���crc�����ʾ�����ļ�������Ҫʹ��Ĭ������
		if(systemSetData->crc != CalModbusCRC16Fun1(systemSetData, sizeof(SystemSetData) - 2))
		{
			//�ָ�Ĭ������
			setDefaultSystemSetData(systemSetData);
			
			//����Ĭ������
			if(My_Pass == SaveSystemSetData(systemSetData))
			{
				setSystemSetData(systemSetData);
				status = My_Pass;
			}
		}
		else
		{
			//�����ϴ�һ���豸��Ϣ
			systemSetData->deviceInfo.isnew = true;
			setSystemSetData(systemSetData);
			status = My_Pass;
		}
	}
	
	MyFree(systemSetData);
	
	return status;
}

/***************************************************************************************************
*FunctionName: testLed
*Description: ����led�Ƿ񷢹�
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��23��15:39:25
***************************************************************************************************/
static MyState_TypeDef testLed(void)
{
	SetGB_LedValue(0);
	vTaskDelay(100 / portTICK_RATE_MS);
	if(LED_Error != ReadLEDStatus())
		return My_Fail;
	
	SetGB_LedValue(300);
	vTaskDelay(100 / portTICK_RATE_MS);
	if(LED_OK == ReadLEDStatus())
	{
		SetGB_LedValue(0);
		return My_Pass;
	}
	else
	{
		SetGB_LedValue(0);
		return My_Fail;
	}
}

/***************************************************************************************************
*FunctionName: testADModel
*Description: ���Բɼ�ģ�飬 �Ƚϲ�ͬͨ���Ĳɼ�ֵ
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��23��16:14:18
***************************************************************************************************/
static MyState_TypeDef testADModel(void)
{
	double tempvalue1 = 0.0, tempvalue2 = 0.0;
	float bili[7] = {1.874, 2.725, 3.656, 4.835, 5.878, 6.973, 8.328};
	unsigned char i=0;
	
	SetGB_LedValue(300);
	vTaskDelay(100 / portTICK_RATE_MS);
	
	for(i=1; i<8; i++)
	{
		SelectChannel(0);
		vTaskDelay(100 / portTICK_RATE_MS);
		tempvalue1 = ADS8325();

		SelectChannel(i);
		vTaskDelay(100 / portTICK_RATE_MS);
		tempvalue2 = ADS8325();
		
		tempvalue2 /= tempvalue1;
		
		tempvalue1 = bili[i-1];
		
		tempvalue2 /= tempvalue1;
		
		if(tempvalue2 > 1.1)
			;//return My_Fail;
		//else if(tempvalue2 < 0.9)
		//	return My_Fail;	
	}
	
	return My_Pass;
}

/***************************************************************************************************
*FunctionName: testMotol
*Description: ���Դ���ģ��
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��23��16:21:14
***************************************************************************************************/
static MyState_TypeDef testMotol(void)
{
	unsigned char count = 0;
	
	SetDRVPowerStatues(LowPower);

	MotorMoveTo(1, 2, 0, false);
	vTaskDelay(100 / portTICK_RATE_MS);
	
	while(!BackLimited)
	{
		vTaskDelay(500 / portTICK_RATE_MS);
		
		count++;
		if(count > 3)
			break;
	}
	
	if(count > 3)
	{
		return My_Fail;
	}
	
	SetDRVPowerStatues(NonamalPower);
	vTaskDelay(100 / portTICK_RATE_MS);
	MotorMoveTo(1, 2, MaxLocation, true);
	vTaskDelay(100 / portTICK_RATE_MS);
	
	//�ߵ�����г̣����ǰ��λ���������쳣
	if(PreLimited)
		return My_Fail;
	
	return My_Pass;
}

/***************************************************************************************************
*FunctionName��ErWeiMaSelfTest
*Description�����Զ�ά���Ƿ�����
*Input��None
*Output��None
*Author��xsx
*Data��2016��5��2��16:59:09
***************************************************************************************************/
MyState_TypeDef testErWeiMa(void)
{
	unsigned char *temp1, *temp2;
	unsigned char *p;
	unsigned short *tempdata = NULL;
	unsigned char i=0, j=0;
	MyState_TypeDef result = My_Fail;
	
	temp1 = MyMalloc(50);
	temp2 = MyMalloc(50);
	tempdata = MyMalloc(sizeof(unsigned short)*20);
	
	if((temp1 == NULL)||(temp2 == NULL)||(tempdata == NULL))
		goto END;
	
	memset(temp1, 0, 50);
	memset(temp2, 0, 50);
	memset(tempdata, 0, sizeof(unsigned short)*20);
	
	/*��ȡ������*/
	p = temp1;
/*	*p++ = 0x7e;
	*p++ = 0x00;
	*p++ = 0x07;
	*p++ = 0x01;
	*p++ = 0x00;
	*p++ = 0x2a;
	*p++ = 0x02;
	*p++ = 0xd8;
	*p++ = 0x0f;*/
	
	*p++ = 0x7e;
	*p++ = 0x00;
	*p++ = 0x07;
	*p++ = 0x01;
	*p++ = 0x00;
	*p++ = 0x0a;
	*p++ = 0x01;
	*p++ = 0xee;
	*p++ = 0xa8;
/*
	for(i=0; i<3; i++)
	{
		if(pdPASS == SendDataToQueue(GetUsart2TXQueue(), GetUsart2TXMutex(),temp1, 9, 1, 500/portTICK_RATE_MS, EnableUsart2TXInterrupt))
		{
			p = temp2;
			while(pdPASS == ReceiveDataFromQueue(GetUsart2RXQueue(), GetUsart2RXMutex(), p+j, 1, 1, 50/portTICK_RATE_MS))
					j++;
			
			if(j > 0)
			{
				tempdata[1] = temp2[5];
				tempdata[1] = (tempdata[1]<<8) + temp2[4];
				j = 0;
				if(tempdata[1] == 0x0139)
				{
					result = My_Pass;
					goto END;
				}
			}
		}	
	}
	*/
	END:
		MyFree(temp1);
		MyFree(temp2);
		MyFree(tempdata);
		return result;
}
