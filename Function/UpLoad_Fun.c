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
#include	"SDFunction.h"
#include	"Net_Data.h"
#include 	"Usart4_Driver.h"
#include	"QueueUnits.h"
#include	"BackDoorData.h"
#include	"WifiFunction.h"

#include	"cJSON.h"
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
static void UpLoadDeviceInfo(void);
static MyState_TypeDef UpLoadTestData(void);
static void PostDataByWifi(void);
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************����********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

void UpLoadFunction(void)
{
	static unsigned short count = 0;
	unsigned char buf[10];
	while(1)
	{
/*		if(Link_Up == GetGB_NCDServerLinkState())
		{
			if((count % 10) == 0)
				UpLoadDeviceInfo();
			
			if(My_Pass != UpLoadTestData())
				vTaskDelay(10000);
		}
		count++;*/
		if(getwifiBusy() == 0)
			PostDataByWifi();
		vTaskDelay(1000 / portTICK_RATE_MS);
	}
}


static void UpLoadDeviceInfo(void)
{
	DeviceInfo * deviceinfo = NULL;
	char * buf = NULL;
	
	deviceinfo = MyMalloc(sizeof(DeviceInfo));
	buf = MyMalloc(2048);
	
	if(deviceinfo && buf)
	{
		if((My_Pass == ReadDeviceInfo(deviceinfo)) && (deviceinfo->crc == CalModbusCRC16Fun1(deviceinfo, sizeof(DeviceInfo)-2)) &&
			(deviceinfo->isfresh == 1))
		{
			memset(buf, 0, 2048);
			
			sprintf(buf, "id1=%s&name=ӫ�����߷�����&manufacture=�人Ŧ��������Ƽ��ɷ����޹�˾&tel=1234567890&status=����&address=%s&needmainten=��",
				deviceinfo->deviceid, deviceinfo->deviceunit);

			if(My_Pass == UpLoadData("http://123.57.94.39/api/myFluorescenceInfo/", buf, strlen(buf)))
			{
				deviceinfo->isfresh = 0;
				SaveDeviceInfo(deviceinfo);
			}
		}
	}
	
	MyFree(deviceinfo);
	MyFree(buf);
}

static MyState_TypeDef UpLoadTestData(void)
{
	TestData * testdata = NULL;
	char * sendbuf = NULL;
	DeviceInfo * deviceinfo = NULL;
	
	char *tempbuf = NULL;
	UpLoadIndex uploadindex;
	char *linebuf = NULL;
	unsigned short i = 0;
	
	MyState_TypeDef statues = My_Fail;
	
	deviceinfo = MyMalloc(sizeof(DeviceInfo));
	testdata = MyMalloc(sizeof(TestData));
	sendbuf = MyMalloc(4096);
	tempbuf = MyMalloc(2500);
	linebuf = MyMalloc(10);
	
	if(testdata && sendbuf && deviceinfo && linebuf)
	{
		if(My_Pass == ReadUpLoadIndex(&uploadindex))
		{
			if((My_Pass == ReadTestData(testdata, uploadindex.index, 1)) && (testdata->crc == CalModbusCRC16Fun1(testdata, sizeof(TestData)-2)) &&
				(My_Pass == ReadDeviceInfo(deviceinfo)) && (deviceinfo->crc == CalModbusCRC16Fun1(deviceinfo, sizeof(DeviceInfo)-2)) )
			{
				memset(sendbuf, 0, 4096);
				memset(tempbuf, 0, 2500);
				
				for(i=0; i<MaxPointLen; i++)
				{
					memset(linebuf, 0, 10);
					sprintf(linebuf, "%d#", testdata->testline.TestPoint[i]);
					strcat(tempbuf, linebuf);
				}
				
				if(tempbuf)
				{
					sprintf(sendbuf, "test_reaction_time=%d&temperature=%2.1f&temperature2=%2.1f&fluorescence_data=%s&Cposition=%d&Baseposition=%d&Tposition=%d&resultratio=%.3f&resultprimitive=%.3f&resultcalibration=%.3f&testSampleID=%s&DeviceID=%.5s&testCardID=%.5s",
						testdata->time, testdata->TestTemp.E_Temperature, testdata->TestTemp.O_Temperature, tempbuf, testdata->testline.C_Point[1], testdata->testline.B_Point[1],
						testdata->testline.T_Point[1],testdata->testline.BasicBili, testdata->testline.BasicResult, testdata->testline.AdjustResult, testdata->sampleid, deviceinfo->deviceid, testdata->temperweima.CardPiCi);

					if(My_Pass == UpLoadData("http://123.57.94.39/api/myFluorescenceData/", sendbuf, strlen(sendbuf)))
					{
						uploadindex.index++;
						WriteUpLoadIndex(uploadindex.index);
						statues = My_Pass;
					}
				}
			}
		}
	}
	
	MyFree(deviceinfo);
	MyFree(testdata);
	MyFree(tempbuf);
	MyFree(sendbuf);
	MyFree(linebuf);
	
	return statues;
}

static void PostDataByWifi(void)
{
	double data = 0;
	char * tempBuf = NULL;
	
	tempBuf = MyMalloc(512);
	if(tempBuf)
	{
		memset(tempBuf, 0, 512);
		
		if(pdPASS == ReceiveDataFromQueue(GetUsart4RXQueue(), NULL, tempBuf, 500, 1, 100 / portTICK_RATE_MS))
		{
			data = strtod(tempBuf , NULL);
			setwifiResult(data);
		}
	}
	
	MyFree(tempBuf);
}
