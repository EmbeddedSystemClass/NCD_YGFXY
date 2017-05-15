/***************************************************************************************************
*FileName:
*Description:
*Author:
*Data:
***************************************************************************************************/

/***************************************************************************************************/
/******************************************ͷ�ļ�***************************************************/
/***************************************************************************************************/
#include	"UpLoad_Fun.h"
#include	"ServerFun.h"
#include	"RTC_Driver.h"
#include	"SystemSet_Dao.h"
#include	"System_Data.h"
#include	"RemoteSoftDao.h"
#include	"RemoteSoft_Data.h"

#include	"MyMem.h"
#include	"CRC16.h"

#include	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"
#include	"semphr.h"

#include	<string.h>
#include	"stdio.h"
#include 	"stdlib.h"
/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/
static MyState_TypeDef ReadTime(void);
static void UpLoadDeviceInfo(void);
static void UpLoadTestData(void);
static void readRemoteFirmwareVersion(void);
static void DownLoadFirmware(void);
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************����********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

void UpLoadFunction(void)
{
	while(1)
	{
		if(My_Pass == ReadTime())
		{
			vTaskDelay(1000 / portTICK_RATE_MS);
			
			UpLoadDeviceInfo();
			vTaskDelay(1000 / portTICK_RATE_MS);
			
			readRemoteFirmwareVersion();
			vTaskDelay(1000 / portTICK_RATE_MS);
			
			DownLoadFirmware();
			vTaskDelay(1000 / portTICK_RATE_MS);
				
			UpLoadTestData();	
			vTaskDelay(1000 / portTICK_RATE_MS);
		}

		vTaskDelay(60000 / portTICK_RATE_MS);
	}
}

static MyState_TypeDef ReadTime(void)
{
	UpLoadDeviceDataBuffer * upLoadDeviceDataBuffer = NULL;
	MyState_TypeDef status = My_Fail;
	
	upLoadDeviceDataBuffer = MyMalloc(sizeof(UpLoadDeviceDataBuffer));
	
	if(upLoadDeviceDataBuffer)
	{
		memset(upLoadDeviceDataBuffer, 0, sizeof(UpLoadDeviceDataBuffer));
		
		copyGBSystemSetData(&(upLoadDeviceDataBuffer->systemSetData));
		
		if(upLoadDeviceDataBuffer->systemSetData.crc == CalModbusCRC16Fun1(&(upLoadDeviceDataBuffer->systemSetData), sizeof(SystemSetData) - 2))
		{
			sprintf(upLoadDeviceDataBuffer->sendBuf, "did=%s\0", upLoadDeviceDataBuffer->systemSetData.deviceInfo.deviceid);
		
			if(My_Pass == UpLoadData("/NCD_Server/up_dtime", upLoadDeviceDataBuffer->sendBuf, strlen(upLoadDeviceDataBuffer->sendBuf),
				upLoadDeviceDataBuffer->recvBuf, SERVERRECVBUFLEN, "POST"))
			{
				RTC_SetTimeData2(upLoadDeviceDataBuffer->recvBuf+7);
				
				status = My_Pass;
			}
		}
	}
	MyFree(upLoadDeviceDataBuffer);
	
	return status;
}
static void UpLoadDeviceInfo(void)
{
	UpLoadDeviceDataBuffer * upLoadDeviceDataBuffer = NULL;
	
	upLoadDeviceDataBuffer = MyMalloc(sizeof(UpLoadDeviceDataBuffer));
	
	if(upLoadDeviceDataBuffer)
	{
		memset(upLoadDeviceDataBuffer, 0, sizeof(UpLoadDeviceDataBuffer));
		
		copyGBSystemSetData(&(upLoadDeviceDataBuffer->systemSetData));
		
		if(upLoadDeviceDataBuffer->systemSetData.crc == CalModbusCRC16Fun1(&(upLoadDeviceDataBuffer->systemSetData), sizeof(SystemSetData) - 2))
		{
			if(upLoadDeviceDataBuffer->systemSetData.deviceInfo.isnew)
			{
				sprintf(upLoadDeviceDataBuffer->sendBuf, "did=%s&dversion=%d&addr=%s&name=%s&age=%s&sex=%s&phone=%s&job=%s&dsc=%s&status=ok\0",
					upLoadDeviceDataBuffer->systemSetData.deviceInfo.deviceid,  GB_SoftVersion, upLoadDeviceDataBuffer->systemSetData.deviceInfo.deviceunit, 
					upLoadDeviceDataBuffer->systemSetData.deviceInfo.deviceuser.user_name, upLoadDeviceDataBuffer->systemSetData.deviceInfo.deviceuser.user_age, 
					upLoadDeviceDataBuffer->systemSetData.deviceInfo.deviceuser.user_sex,	upLoadDeviceDataBuffer->systemSetData.deviceInfo.deviceuser.user_phone, 
					upLoadDeviceDataBuffer->systemSetData.deviceInfo.deviceuser.user_job, upLoadDeviceDataBuffer->systemSetData.deviceInfo.deviceuser.user_desc);
					
				if(My_Pass == UpLoadData("/NCD_Server/up_device", upLoadDeviceDataBuffer->sendBuf, strlen(upLoadDeviceDataBuffer->sendBuf), upLoadDeviceDataBuffer->recvBuf,
					SERVERRECVBUFLEN, "POST"))
				{
					copyGBSystemSetData(&(upLoadDeviceDataBuffer->systemSetData));
					
					upLoadDeviceDataBuffer->systemSetData.deviceInfo.isnew = false ;
					
					//���沢�����ڴ��е�����
					SaveSystemSetData(&(upLoadDeviceDataBuffer->systemSetData));
				}
			}
		}
	}
	
	MyFree(upLoadDeviceDataBuffer);
}

/***************************************************************************************************
*FunctionName: UpLoadTestData
*Description: �ϴ��������ݣ����ݺ����ߣ�
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2017��2��20��16:43:44
***************************************************************************************************/
static void UpLoadTestData(void)
{
	UpLoadTestDataBuffer * upLoadTestDataBuffer = NULL;
	
	upLoadTestDataBuffer = MyMalloc(sizeof(UpLoadTestDataBuffer));
	
	if(upLoadTestDataBuffer)
	{
		memset(upLoadTestDataBuffer, 0, sizeof(UpLoadTestDataBuffer));
		
		//��ȡ�豸��Ϣ
		copyGBSystemSetData(&(upLoadTestDataBuffer->systemSetData));
		
		//�������ͷ�Ƿ�У����ȷ���������ݴ�����
		if(upLoadTestDataBuffer->systemSetData.upLoadIndex >= upLoadTestDataBuffer->systemSetData.testDataNum)
			goto END;
		
		//��ȡ��������,��ȡʧ�����˳�
		upLoadTestDataBuffer->pageRequest.startElementIndex = upLoadTestDataBuffer->systemSetData.upLoadIndex;
		upLoadTestDataBuffer->pageRequest.orderType = DESC;
		upLoadTestDataBuffer->pageRequest.pageSize = 1;

		if(My_Pass != ReadTestData(&(upLoadTestDataBuffer->pageRequest), &(upLoadTestDataBuffer->page), &(upLoadTestDataBuffer->systemSetData)))
			goto END;
		
		upLoadTestDataBuffer->testData = upLoadTestDataBuffer->page.testData;
		for(upLoadTestDataBuffer->k=0; upLoadTestDataBuffer->k< upLoadTestDataBuffer->page.ElementsSize; upLoadTestDataBuffer->k++)
		{
			//���crcУ����ȷ����ʼ�ϴ�
			if(upLoadTestDataBuffer->testData->crc == CalModbusCRC16Fun1(upLoadTestDataBuffer->testData, sizeof(TestData)-2))
			{
				//�ϴ���������
				if(upLoadTestDataBuffer->testData->testResultDesc != ResultIsOK)
					sprintf(upLoadTestDataBuffer->tempBuf, "Error\0");
				else
					sprintf(upLoadTestDataBuffer->tempBuf, "Ok\0");
				
				sprintf(upLoadTestDataBuffer->sendBuf, "cnum=%s&cid=%s&did=%s&t_name=%s&sid=%s&testtime=20%d-%d-%d %d:%d:%d&e_t=%.1f&o_t=%.1f&outt=%d&c_l=%d&t_l=%d&b_l=%d&t_c_v=%.3f&a_p=%.3f&b_v=%.*f&a_v=%.*f&t_re=%s\0",
					upLoadTestDataBuffer->testData->temperweima.piNum, upLoadTestDataBuffer->testData->temperweima.PiHao, 
					upLoadTestDataBuffer->systemSetData.deviceInfo.deviceid, upLoadTestDataBuffer->testData->user.user_name, 
					upLoadTestDataBuffer->testData->sampleid, upLoadTestDataBuffer->testData->TestTime.year, 
					upLoadTestDataBuffer->testData->TestTime.month, upLoadTestDataBuffer->testData->TestTime.day, 
					upLoadTestDataBuffer->testData->TestTime.hour, upLoadTestDataBuffer->testData->TestTime.min, 
					upLoadTestDataBuffer->testData->TestTime.sec, upLoadTestDataBuffer->testData->TestTemp.E_Temperature, 
					upLoadTestDataBuffer->testData->TestTemp.O_Temperature, upLoadTestDataBuffer->testData->time, 
					upLoadTestDataBuffer->testData->testline.C_Point[1], upLoadTestDataBuffer->testData->testline.T_Point[1],
					upLoadTestDataBuffer->testData->testline.B_Point[1], upLoadTestDataBuffer->testData->testline.BasicBili, 
					upLoadTestDataBuffer->testData->tempadjust.parm, upLoadTestDataBuffer->testData->temperweima.itemConstData.pointNum, 
					upLoadTestDataBuffer->testData->testline.BasicResult, upLoadTestDataBuffer->testData->temperweima.itemConstData.pointNum, 
					upLoadTestDataBuffer->testData->testline.AdjustResult, upLoadTestDataBuffer->tempBuf);

				if(My_Pass != UpLoadData("/NCD_Server/up_testdata", upLoadTestDataBuffer->sendBuf, strlen(upLoadTestDataBuffer->sendBuf), 
					upLoadTestDataBuffer->recvBuf, UPLOADRECVBUFLEN, "POST"))
					break;
				
				//�ϴ���������
				sprintf(upLoadTestDataBuffer->sendBuf, "cnum=%s&cid=%s&serie_a=[\0", upLoadTestDataBuffer->testData->temperweima.piNum, upLoadTestDataBuffer->testData->temperweima.PiHao);
					
				for(upLoadTestDataBuffer->j=0; upLoadTestDataBuffer->j<300; upLoadTestDataBuffer->j++)
				{
					if(upLoadTestDataBuffer->j == 0)
						sprintf(upLoadTestDataBuffer->tempBuf, "%d\0", upLoadTestDataBuffer->testData->testline.TestPoint[upLoadTestDataBuffer->i*100 + upLoadTestDataBuffer->j]);
					else
						sprintf(upLoadTestDataBuffer->tempBuf, ",%d\0", upLoadTestDataBuffer->testData->testline.TestPoint[upLoadTestDataBuffer->i*100 + upLoadTestDataBuffer->j]);
					strcat(upLoadTestDataBuffer->sendBuf, upLoadTestDataBuffer->tempBuf);
				}
					
				sprintf(upLoadTestDataBuffer->tempBuf, "]\0");
				strcat(upLoadTestDataBuffer->sendBuf, upLoadTestDataBuffer->tempBuf);
					
				if(My_Pass != UpLoadData("/NCD_Server/up_series", upLoadTestDataBuffer->sendBuf, strlen(upLoadTestDataBuffer->sendBuf),
					upLoadTestDataBuffer->recvBuf, UPLOADRECVBUFLEN, "POST"))
					goto END1;
			}
			
			upLoadTestDataBuffer->systemSetData.upLoadIndex++;
			upLoadTestDataBuffer->testData++;
		}
		
		//�ϴ�����ʧ�ܣ��������˴�����ǰ�����´������ϴ�
		END1:
		
		//����ϵͳ���������е��ϴ���������
		setUpLoadIndex(upLoadTestDataBuffer->systemSetData.upLoadIndex);
		//�־û�ϵͳ��������
		SaveSystemSetData(getGBSystemSetData());
	}
	
	END:
		MyFree(upLoadTestDataBuffer);

}

/***************************************************************************************************
*FunctionName: 
*Description: 
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 
***************************************************************************************************/
static void readRemoteFirmwareVersion(void)
{
	UpLoadDeviceDataBuffer * upLoadDeviceDataBuffer = NULL;
	MyState_TypeDef status = My_Fail;
	
	upLoadDeviceDataBuffer = MyMalloc(sizeof(UpLoadDeviceDataBuffer));
	
	if(upLoadDeviceDataBuffer)
	{
		memset(upLoadDeviceDataBuffer, 0, sizeof(UpLoadDeviceDataBuffer));
		
		sprintf(upLoadDeviceDataBuffer->sendBuf, "a");
		
		if(My_Pass == UpLoadData("/NCD_Server/deviceSoftInfo", upLoadDeviceDataBuffer->sendBuf, strlen(upLoadDeviceDataBuffer->sendBuf),
			upLoadDeviceDataBuffer->recvBuf, SERVERRECVBUFLEN, "POST"))
		{
			//�������¹̼��汾
			upLoadDeviceDataBuffer->remoteSoftInfo.RemoteFirmwareVersion = strtol(upLoadDeviceDataBuffer->recvBuf+16, NULL, 10);
			
			//�����ȡ���İ汾�����ڵ�ǰ�汾���Ҵ��ڵ�ǰ���������Զ�̰汾����˴ζ�ȡ�������µ�
			if((upLoadDeviceDataBuffer->remoteSoftInfo.RemoteFirmwareVersion > GB_SoftVersion) &&
				(upLoadDeviceDataBuffer->remoteSoftInfo.RemoteFirmwareVersion > getGbRemoteFirmwareVersion()))
			{
				//�������¹̼�MD5
				upLoadDeviceDataBuffer->tempP = strtok(upLoadDeviceDataBuffer->recvBuf, "#");
				if(upLoadDeviceDataBuffer->tempP)
				{
					upLoadDeviceDataBuffer->tempP = strtok(NULL, "#");
					
					memcpy(upLoadDeviceDataBuffer->remoteSoftInfo.md5, upLoadDeviceDataBuffer->tempP+4, 32);
					
					if(My_Pass == WriteRemoteSoftInfo(&(upLoadDeviceDataBuffer->remoteSoftInfo)))
					{
						//md5����ɹ��󣬲Ÿ������°汾�ţ��������¹̼��汾
						setGbRemoteFirmwareVersion(upLoadDeviceDataBuffer->remoteSoftInfo.RemoteFirmwareVersion);
						setGbRemoteFirmwareMd5(upLoadDeviceDataBuffer->remoteSoftInfo.md5);
						
						setIsSuccessDownloadFirmware(false);
					}
				}
			}	
		}
	}
	MyFree(upLoadDeviceDataBuffer);
}

static void DownLoadFirmware(void)
{
	UpLoadDeviceDataBuffer * upLoadDeviceDataBuffer = NULL;
	MyState_TypeDef status = My_Fail;
	
	//����Ƿ��и��£���δ�ɹ����أ�����Ҫ����
	if((getGbRemoteFirmwareVersion() > GB_SoftVersion) && (false == getIsSuccessDownloadFirmware()))
	{
		upLoadDeviceDataBuffer = MyMalloc(sizeof(UpLoadDeviceDataBuffer));
	
		if(upLoadDeviceDataBuffer)
		{
			memset(upLoadDeviceDataBuffer, 0, sizeof(UpLoadDeviceDataBuffer));
			
			UpLoadData("/NCD_Server/deviceCodeDownload", NULL, 0, NULL, 0, "GET");
		}
		MyFree(upLoadDeviceDataBuffer);
	}
}
