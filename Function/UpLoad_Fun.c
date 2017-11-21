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
#include	"HttpBuf.h"
#include	"StringDefine.h"

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
static HttpBuffer * httpBuffer = NULL;
/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/
static MyState_TypeDef ReadTime(HttpBuffer * httpBuffer);
static void UpLoadDeviceInfo(HttpBuffer * httpBuffer);
static void UpLoadTestData(HttpBuffer * httpBuffer);
static void readRemoteFirmwareVersion(HttpBuffer * httpBuffer);
static void DownLoadFirmware(HttpBuffer * httpBuffer);
static void upLoadUserServer(void);
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
		httpBuffer = MyMalloc(HttpBufferStructSize);
		if(httpBuffer)
		{
			if(My_Pass == ReadTime(httpBuffer))
			{
				vTaskDelay(1000 / portTICK_RATE_MS);
				
				UpLoadDeviceInfo(httpBuffer);
				vTaskDelay(1000 / portTICK_RATE_MS);
				
				UpLoadTestData(httpBuffer);	
				vTaskDelay(1000 / portTICK_RATE_MS);
				
				readRemoteFirmwareVersion(httpBuffer);
				vTaskDelay(1000 / portTICK_RATE_MS);
				
				DownLoadFirmware(httpBuffer);
				vTaskDelay(1000 / portTICK_RATE_MS);
			}
		}
		
		MyFree(httpBuffer);
		
		vTaskDelay(30000 / portTICK_RATE_MS);
	}
}

static MyState_TypeDef ReadTime(HttpBuffer * httpBuffer)
{
	readDeviceId(httpBuffer->tempBuf);
	
	sprintf(httpBuffer->sendBuf, "POST %s HTTP/1.1\nHost: %d.%d.%d.%d:%d\nConnection: keep-alive\nContent-Length:[##]\nContent-Type:application/x-www-form-urlencoded;charset=GBK\nAccept-Language: zh-CN,zh;q=0.8\n\ndid=%s", 
		NcdServerReadTimeUrlStr, GB_ServerIp_1, GB_ServerIp_2, GB_ServerIp_3, GB_ServerIp_4, GB_ServerPort, httpBuffer->tempBuf);
	
	httpBuffer->tempP = strstr(httpBuffer->sendBuf, "zh;q=0.8\n\n");
	httpBuffer->sendDataLen = strlen(httpBuffer->tempP)-10;	
	httpBuffer->tempP = strstr(httpBuffer->sendBuf, "[##]");
	sprintf(httpBuffer->tempBuf, "%04d", httpBuffer->sendDataLen);
	memcpy(httpBuffer->tempP, httpBuffer->tempBuf, 4);
	httpBuffer->sendDataLen = strlen(httpBuffer->sendBuf);
	httpBuffer->isPost = true;
	
	if(My_Pass == UpLoadData(httpBuffer))
	{
		RTC_SetTimeData2(httpBuffer->tempP+7);
				
		return My_Pass;
	}
	else
		return My_Fail;
}

static void UpLoadDeviceInfo(HttpBuffer * httpBuffer)
{
	httpBuffer->device = (DeviceInfo *)httpBuffer->tempBuf;
	
	getDeviceInfo(httpBuffer->device);
	
	if(httpBuffer->device->isnew)
	{
		sprintf(httpBuffer->sendBuf, "POST %s HTTP/1.1\nHost: %d.%d.%d.%d:%d\nConnection: keep-alive\nContent-Length:[##]\nContent-Type:application/x-www-form-urlencoded;charset=GBK\nAccept-Language: zh-CN,zh;q=0.8\n\ndid=%s&dversion=%d&addr=%s&name=%s&age=%s&sex=%s&phone=%s&job=%s&dsc=%s&status=ok", 
			NcdServerUpDeviceUrlStr, GB_ServerIp_1, GB_ServerIp_2, GB_ServerIp_3, GB_ServerIp_4, GB_ServerPort, httpBuffer->device->deviceid,  
			GB_SoftVersion, httpBuffer->device->deviceunit, httpBuffer->device->deviceuser.user_name, 
			httpBuffer->device->deviceuser.user_age, httpBuffer->device->deviceuser.user_sex,	
			httpBuffer->device->deviceuser.user_phone, httpBuffer->device->deviceuser.user_job, 
			httpBuffer->device->deviceuser.user_desc);
		
		httpBuffer->tempP = strstr(httpBuffer->sendBuf, "zh;q=0.8\n\n");
		httpBuffer->sendDataLen = strlen(httpBuffer->tempP)-10;	
		httpBuffer->tempP = strstr(httpBuffer->sendBuf, "[##]");
		sprintf(httpBuffer->tempBuf, "%04d", httpBuffer->sendDataLen);
		memcpy(httpBuffer->tempP, httpBuffer->tempBuf, 4);
		httpBuffer->sendDataLen = strlen(httpBuffer->sendBuf);
		httpBuffer->isPost = true;
		
		if(My_Pass == UpLoadData(httpBuffer))
		{
			httpBuffer->systemSetData = (SystemSetData *)httpBuffer->sendBuf;
			copyGBSystemSetData(httpBuffer->systemSetData);
			httpBuffer->systemSetData->deviceInfo.isnew = false ;
			SaveSystemSetData(httpBuffer->systemSetData);
		}
	}
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
static void UpLoadTestData(HttpBuffer * httpBuffer)
{
	httpBuffer->tempInt1 = getTestDataTotalNum();
	httpBuffer->tempInt2 = getUpLoadIndex();
	
	//is have data not to update ?
	if(httpBuffer->tempInt1 > httpBuffer->tempInt2)
	{
		httpBuffer->page = MyMalloc(PageStructSize);
		if(httpBuffer->page)
		{
			//read datas from sd
			httpBuffer->pageRequest.startElementIndex = httpBuffer->tempInt2;
			httpBuffer->pageRequest.orderType = DESC;
			httpBuffer->pageRequest.pageSize = 5;

			memset(httpBuffer->page, 0, PageStructSize);
			
			if(My_Pass == ReadTestData(&httpBuffer->pageRequest, httpBuffer->page, httpBuffer->tempInt1))
			{
				httpBuffer->testData = httpBuffer->page->testData;
				for(httpBuffer->k=0; httpBuffer->k< httpBuffer->page->ElementsSize; httpBuffer->k++)
				{
					//���crcУ����ȷ����ʼ�ϴ�
					if(httpBuffer->testData->crc == CalModbusCRC16Fun1(httpBuffer->testData, TestDataStructCrcSize))
					{
						//�ϴ���������
						if(httpBuffer->testData->testResultDesc != ResultIsOK)
							sprintf(httpBuffer->tempBuf, "false");
						else
							sprintf(httpBuffer->tempBuf, "true");
						
						if(httpBuffer->testData->TestTime.month == 0 || httpBuffer->testData->TestTime.day == 0)
						{
							httpBuffer->testData->TestTime.year = 0;
							httpBuffer->testData->TestTime.month = 1;
							httpBuffer->testData->TestTime.day = 1;
							httpBuffer->testData->TestTime.hour = 0;
							httpBuffer->testData->TestTime.min = 0;
							httpBuffer->testData->TestTime.sec = 0;
						}
						
						//read device id
						readDeviceId(httpBuffer->tempBuf+10);
						
						sprintf(httpBuffer->sendBuf, "POST %s HTTP/1.1\nHost: %d.%d.%d.%d:%d\nConnection: keep-alive\nContent-Length:[##]\nContent-Type:application/x-www-form-urlencoded;charset=GBK\nAccept-Language: zh-CN,zh;q=0.8\n\ncardnum=%s&qrdata.cid=%s&device.did=%s&tester=%s&sampleid=%s&testtime=20%02d-%d-%d %d:%d:%d&overtime=%d&cline=%d&tline=%d&bline=%d&t_c_v=%.3f&testv=%.*f&serialnum=%s-%s&t_isok=%s&cparm=%d\0",
							NcdServerUpTestDataUrlStr, GB_ServerIp_1, GB_ServerIp_2, GB_ServerIp_3, GB_ServerIp_4, GB_ServerPort, 
							httpBuffer->testData->temperweima.piNum, httpBuffer->testData->temperweima.PiHao, 
							httpBuffer->tempBuf+10, httpBuffer->testData->user.user_name, 
							httpBuffer->testData->sampleid, httpBuffer->testData->TestTime.year, 
							httpBuffer->testData->TestTime.month, httpBuffer->testData->TestTime.day, 
							httpBuffer->testData->TestTime.hour, httpBuffer->testData->TestTime.min, 
							httpBuffer->testData->TestTime.sec, httpBuffer->testData->time, 
							httpBuffer->testData->testline.C_Point.x, httpBuffer->testData->testline.T_Point.x,
							httpBuffer->testData->testline.B_Point.x, httpBuffer->testData->testline.BasicBili, 
							httpBuffer->testData->temperweima.itemConstData.pointNum, httpBuffer->testData->testline.BasicResult, 
							httpBuffer->testData->temperweima.PiHao, httpBuffer->testData->temperweima.piNum, httpBuffer->tempBuf,
							httpBuffer->testData->testline.CMdifyNum);

						for(httpBuffer->i=0; httpBuffer->i<100; httpBuffer->i++)
						{
							if(httpBuffer->i == 0)
								sprintf(httpBuffer->tempBuf, "&series=[%d,%d,%d", httpBuffer->testData->testline.TestPoint[httpBuffer->i*3],
									httpBuffer->testData->testline.TestPoint[httpBuffer->i*3+1], httpBuffer->testData->testline.TestPoint[httpBuffer->i*3+2]);
							else
								sprintf(httpBuffer->tempBuf, ",%d,%d,%d", httpBuffer->testData->testline.TestPoint[httpBuffer->i*3],
									httpBuffer->testData->testline.TestPoint[httpBuffer->i*3+1], httpBuffer->testData->testline.TestPoint[httpBuffer->i*3+2]);
							strcat(httpBuffer->sendBuf, httpBuffer->tempBuf);
						}
							
						strcat(httpBuffer->sendBuf, "]");
						
						httpBuffer->tempP = strstr(httpBuffer->sendBuf, "zh;q=0.8\n\n");
						httpBuffer->sendDataLen = strlen(httpBuffer->tempP)-10;	
						httpBuffer->tempP = strstr(httpBuffer->sendBuf, "[##]");
						sprintf(httpBuffer->tempBuf, "%04d", httpBuffer->sendDataLen);
						memcpy(httpBuffer->tempP, httpBuffer->tempBuf, 4);
						httpBuffer->sendDataLen = strlen(httpBuffer->sendBuf);
						httpBuffer->isPost = true;
						
						if(My_Pass != UpLoadData(httpBuffer))
							break;
					}

					httpBuffer->tempInt2++;
					httpBuffer->testData++;
				}
				
				httpBuffer->systemSetData = (SystemSetData *)httpBuffer->sendBuf;
				copyGBSystemSetData(httpBuffer->systemSetData);
				httpBuffer->systemSetData->upLoadIndex = httpBuffer->tempInt2;
				SaveSystemSetData(httpBuffer->systemSetData);
			}
		}
		
		MyFree(httpBuffer->page);
	}
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
static void readRemoteFirmwareVersion(HttpBuffer * httpBuffer)
{
	memset(httpBuffer->tempBuf, 0, 100);
	readDeviceId(httpBuffer->tempBuf);
	
	sprintf(httpBuffer->sendBuf, "POST %s HTTP/1.1\nHost: %d.%d.%d.%d:%d\nConnection: keep-alive\nContent-Length:[##]\nContent-Type:application/x-www-form-urlencoded;charset=GBK\nAccept-Language: zh-CN,zh;q=0.8\n\nsoftName=%s&lang=%s", 
		NcdServerQuerySoftUrlStr, GB_ServerIp_1, GB_ServerIp_2, GB_ServerIp_3, GB_ServerIp_4, GB_ServerPort, httpBuffer->tempBuf, DeviceLanguageString);
	
	httpBuffer->tempP = strstr(httpBuffer->sendBuf, "zh;q=0.8\n\n");
	httpBuffer->sendDataLen = strlen(httpBuffer->tempP)-10;	
	httpBuffer->tempP = strstr(httpBuffer->sendBuf, "[##]");
	sprintf(httpBuffer->tempBuf, "%04d", httpBuffer->sendDataLen);
	memcpy(httpBuffer->tempP, httpBuffer->tempBuf, 4);
	httpBuffer->sendDataLen = strlen(httpBuffer->sendBuf);
	httpBuffer->isPost = true;
	
	if(My_Pass == UpLoadData(httpBuffer))
	{
		//�������¹̼��汾
		httpBuffer->remoteSoftInfo = (RemoteSoftInfo *)httpBuffer->sendBuf;
		memset(httpBuffer->remoteSoftInfo, 0, RemoteSoftInfoStructSize);
		httpBuffer->remoteSoftInfo->RemoteFirmwareVersion = strtol(httpBuffer->tempP+16, NULL, 10);
			
			//�����ȡ���İ汾�����ڵ�ǰ�汾���Ҵ��ڵ�ǰ���������Զ�̰汾����˴ζ�ȡ�������µ�
			if((httpBuffer->remoteSoftInfo->RemoteFirmwareVersion > GB_SoftVersion) &&
				(httpBuffer->remoteSoftInfo->RemoteFirmwareVersion > getGbRemoteFirmwareVersion()))
			{
				//�������¹̼�MD5
				httpBuffer->tempP = strtok(httpBuffer->recvBuf, "#");
				if(httpBuffer->tempP)
				{
					httpBuffer->tempP = strtok(NULL, "#");
					
					memcpy(httpBuffer->remoteSoftInfo->md5, httpBuffer->tempP+4, 32);
					
					if(My_Pass == WriteRemoteSoftInfo(httpBuffer->remoteSoftInfo))
					{
						//md5����ɹ��󣬲Ÿ������°汾�ţ��������¹̼��汾
						setGbRemoteFirmwareVersion(httpBuffer->remoteSoftInfo->RemoteFirmwareVersion);
						setGbRemoteFirmwareMd5(httpBuffer->remoteSoftInfo->md5);
						
						setIsSuccessDownloadFirmware(false);
					}
				}
			}	
	}
}

static void DownLoadFirmware(HttpBuffer * httpBuffer)
{
	//����Ƿ��и��£���δ�ɹ����أ�����Ҫ����
	if((getGbRemoteFirmwareVersion() > GB_SoftVersion) && (false == getIsSuccessDownloadFirmware()))
	{
		memset(httpBuffer->tempBuf, 0, 100);
		readDeviceId(httpBuffer->tempBuf);
		
		sprintf(httpBuffer->sendBuf, "GET %s?softName=%s&lang=%s HTTP/1.1\nHost: %d.%d.%d.%d:%d\nConnection: keep-alive\n\n", 
			NcdServerDownSoftUrlStr, httpBuffer->tempBuf, DeviceLanguageString, GB_ServerIp_1, GB_ServerIp_2, GB_ServerIp_3, GB_ServerIp_4, GB_ServerPort);
		
		httpBuffer->sendDataLen = strlen(httpBuffer->sendBuf);
		httpBuffer->isPost = false;
		
		UpLoadData(httpBuffer);
	}
}

static void upLoadUserServer(void)
{
	
}
