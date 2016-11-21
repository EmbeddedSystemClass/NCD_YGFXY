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
#include	"System_Data.h"
#include	"RTC_Driver.h"

#include	"cJSON.h"
#include	"MyMem.h"
#include	"CRC16.h"

#include	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"
#include	"semphr.h"

#include	"stdio.h"
#include	"string.h"
/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/
static MyState_TypeDef ReadTime(void);
static MyState_TypeDef UpLoadDeviceInfo(void);
static MyState_TypeDef UpLoadTestData(void);
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************����********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

void UpLoadFunction(void)
{
	unsigned short i = 0;
	while(1)
	{
		if(My_Pass == ReadTime())
		{
			vTaskDelay(1000 / portTICK_RATE_MS);
			
			if(My_Pass == UpLoadDeviceInfo())
			{
				vTaskDelay(1000 / portTICK_RATE_MS);
				
				i=0;
				while(i <= 60)
				{
					if(My_Pass == UpLoadTestData())
						i++;
					else
						break;
					
					vTaskDelay(100 / portTICK_RATE_MS);
				}
				
				vTaskDelay((50-i)*1000 / portTICK_RATE_MS);
			}
			else
				vTaskDelay(60000 / portTICK_RATE_MS);
		}
		else
			vTaskDelay(60000 / portTICK_RATE_MS);
	}
}

static MyState_TypeDef ReadTime(void)
{
	char * buf = NULL;
	DeviceInfo * deviceinfo = NULL;
	MyState_TypeDef status = My_Fail;
	
	buf = MyMalloc(100);
	deviceinfo = MyMalloc(sizeof(DeviceInfo));
	
	if(buf && deviceinfo)
	{
		memset(buf, 0, 100);
		
		//��ȡ�豸��Ϣ
		GetGB_DeviceInfo(deviceinfo);
		
		sprintf(buf, "deviceid=%s", deviceinfo->deviceid);
		
		if(My_Pass == UpLoadData("/NCD_YGFXY_Server/rtime.action", buf, strlen(buf)))
		{
			//RTC_SetTimeData2(buf+10);
			status = My_Pass;
		}
	}
	MyFree(buf);
	MyFree(deviceinfo);
	
	return status;
}

static MyState_TypeDef UpLoadDeviceInfo(void)
{
	MyState_TypeDef status = My_Fail;

	DeviceInfo * deviceinfo = NULL;
	char * buf = NULL;
	
	
	if(true == GetDeviceInIsFresh())
	{
		deviceinfo = MyMalloc(sizeof(DeviceInfo));
		buf = MyMalloc(2048);
		
		if(deviceinfo && buf)
		{
			//��ȡ�豸��Ϣ
			GetGB_DeviceInfo(deviceinfo);
			
			memset(buf, 0, 2048);

			sprintf(buf, "dfo.id=%s&dfo.daddr=%s&dfo.dname=%s&dfo.dage=%s&dfo.dsex=%s&dfo.dphone=%s&dfo.djob=%s&dfo.ddesc=%s&dfo.disok=true",
				deviceinfo->deviceid,  deviceinfo->deviceunit, deviceinfo->deviceuser.user_name, deviceinfo->deviceuser.user_age, deviceinfo->deviceuser.user_sex,
				deviceinfo->deviceuser.user_phone, deviceinfo->deviceuser.user_job, deviceinfo->deviceuser.user_desc);

			if(My_Pass == UpLoadData("/NCD_YGFXY_Server/up_info.action", buf, strlen(buf)))
			{
				SetDeviceInIsFresh(false);
				status = My_Pass;
			}
		}
		
		MyFree(deviceinfo);
		MyFree(buf);
		
		return status;
	}
	else
		return My_Pass;
}

static MyState_TypeDef UpLoadTestData(void)
{
	MyState_TypeDef statues = My_Fail;
	unsigned short i=0, j;
	
	UpLoadTestDataBuffer * myUpLoadTestDataBuffer = NULL;
	
	myUpLoadTestDataBuffer = MyMalloc(sizeof(UpLoadTestDataBuffer));
	
	if(myUpLoadTestDataBuffer)
	{
		memset(myUpLoadTestDataBuffer, 0, sizeof(UpLoadTestDataBuffer));
		
		//��ȡ�豸��Ϣ
		GetGB_DeviceInfo(&(myUpLoadTestDataBuffer->deviceinfo));
		
		//��ȡ��������ͷ,ʧ�����˳�
		if(My_Pass != ReadTestDataHead(&(myUpLoadTestDataBuffer->myTestDataSaveHead)))
			goto END;
		
		//�������ͷ�Ƿ�У����ȷ���������ݴ�����
		if((myUpLoadTestDataBuffer->myTestDataSaveHead.crc != CalModbusCRC16Fun1(&(myUpLoadTestDataBuffer->myTestDataSaveHead), sizeof(TestDataSaveHead)-2)) ||
				myUpLoadTestDataBuffer->myTestDataSaveHead.readindex >= myUpLoadTestDataBuffer->myTestDataSaveHead.datanum)
			goto END;
		
		//��ȡ��������,��ȡʧ�����˳�
		if(My_Pass != ReadTestData(&(myUpLoadTestDataBuffer->testdata), myUpLoadTestDataBuffer->myTestDataSaveHead.readindex, 1))
			goto END;
		
		//У���ȡ�����ݵ���ȷ��,�������ȷ�����ϴ�����+1���Թ�������
		if(myUpLoadTestDataBuffer->testdata.crc != CalModbusCRC16Fun1(&(myUpLoadTestDataBuffer->testdata), sizeof(TestData)-2))
		{
			statues = My_Pass;
			ReadIndexPlus(1);
			goto END;
		}
		
		//�ϴ���������
		memset(myUpLoadTestDataBuffer->sendbuf, 0, 1024);

		sprintf(myUpLoadTestDataBuffer->sendbuf, "tdata.cid=%s&tdata.did=%s&tdata.testd=20%02d-%02d-%02d&tdata.testt=%02d:%02d:%02d&tdata.e_t=%.1f&tdata.o_t=%.1f&tdata.outt=%d&tdata.c_l=%d&tdata.t_l=%d&tdata.b_l=%d&tdata.t_c_v=%.3f&tdata.a_p=%.3f&tdata.b_v=%.3f&tdata.a_v=%.3f&tdata.sid=%s",
			myUpLoadTestDataBuffer->testdata.temperweima.CardPiCi, myUpLoadTestDataBuffer->deviceinfo.deviceid, myUpLoadTestDataBuffer->testdata.TestTime.year, myUpLoadTestDataBuffer->testdata.TestTime.month, myUpLoadTestDataBuffer->testdata.TestTime.day,
			myUpLoadTestDataBuffer->testdata.TestTime.hour, myUpLoadTestDataBuffer->testdata.TestTime.min, myUpLoadTestDataBuffer->testdata.TestTime.sec, myUpLoadTestDataBuffer->testdata.TestTemp.E_Temperature, myUpLoadTestDataBuffer->testdata.TestTemp.O_Temperature,
			myUpLoadTestDataBuffer->testdata.time, myUpLoadTestDataBuffer->testdata.testline.C_Point[1], myUpLoadTestDataBuffer->testdata.testline.T_Point[1], myUpLoadTestDataBuffer->testdata.testline.B_Point[1], myUpLoadTestDataBuffer->testdata.testline.BasicBili,
			myUpLoadTestDataBuffer->testdata.tempadjust.parm, myUpLoadTestDataBuffer->testdata.testline.BasicResult, myUpLoadTestDataBuffer->testdata.testline.AdjustResult, myUpLoadTestDataBuffer->testdata.sampleid);

		if(My_Pass != UpLoadData("/NCD_YGFXY_Server/updata.action", myUpLoadTestDataBuffer->sendbuf, strlen(myUpLoadTestDataBuffer->sendbuf)))
			goto END;
		
		//�ϴ���⿨����
		memset(myUpLoadTestDataBuffer->sendbuf, 0, 1024);

		sprintf(myUpLoadTestDataBuffer->sendbuf, "tdata.cid=%s&tdata.c_item=%s&tdata.c_n_v=%.3f&tdata.c_l_v=%.3f&tdata.c_h_v=%.3f&tdata.c_dw=%s&tdata.c_t_l=%d&tdata.c_bq_n=%d&tdata.c_fend=%.3f&tdata.c_bq1_a=%.3f&tdata.c_bq1_b=%.3f&tdata.c_bq1_c=%.3f&tdata.c_bq2_a=%.3f&tdata.c_bq2_b=%.3f&tdata.c_bq2_c=%.3f&tdata.c_waitt=%d&tdata.c_c_l=%d&tdata.c_outt=20%02d-%02d-%02d",
			myUpLoadTestDataBuffer->testdata.temperweima.CardPiCi, myUpLoadTestDataBuffer->testdata.temperweima.ItemName, myUpLoadTestDataBuffer->testdata.temperweima.NormalResult, myUpLoadTestDataBuffer->testdata.temperweima.LowstResult, myUpLoadTestDataBuffer->testdata.temperweima.HighestResult,
			myUpLoadTestDataBuffer->testdata.temperweima.ItemMeasure, myUpLoadTestDataBuffer->testdata.temperweima.ItemLocation, myUpLoadTestDataBuffer->testdata.temperweima.ItemBiaoQuNum, myUpLoadTestDataBuffer->testdata.temperweima.ItemFenDuan, myUpLoadTestDataBuffer->testdata.temperweima.ItemBiaoQu[0][0],
			myUpLoadTestDataBuffer->testdata.temperweima.ItemBiaoQu[0][1], myUpLoadTestDataBuffer->testdata.temperweima.ItemBiaoQu[0][2],myUpLoadTestDataBuffer->testdata.temperweima.ItemBiaoQu[1][0],	myUpLoadTestDataBuffer->testdata.temperweima.ItemBiaoQu[1][1], myUpLoadTestDataBuffer->testdata.temperweima.ItemBiaoQu[1][2],
			myUpLoadTestDataBuffer->testdata.temperweima.CardWaitTime, myUpLoadTestDataBuffer->testdata.temperweima.CLineLocation, myUpLoadTestDataBuffer->testdata.temperweima.CardBaoZhiQi.year, myUpLoadTestDataBuffer->testdata.temperweima.CardBaoZhiQi.month, myUpLoadTestDataBuffer->testdata.temperweima.CardBaoZhiQi.day);

		if(My_Pass != UpLoadData("/NCD_YGFXY_Server/upcard.action", myUpLoadTestDataBuffer->sendbuf, strlen(myUpLoadTestDataBuffer->sendbuf)))
			goto END;
		
		//�ϴ���������
		for(i=0; i<4; i++)
		{
			memset(myUpLoadTestDataBuffer->sendbuf, 0, 1024);
			sprintf(myUpLoadTestDataBuffer->sendbuf, "testcardid=%s&index=%d&series=[",
				myUpLoadTestDataBuffer->testdata.temperweima.CardPiCi, i+1);
			
			for(j=0; j<110; j++)
			{
				if((i == 3) && (j >= 100))
					break;
				
				if(j == 0)
					sprintf(myUpLoadTestDataBuffer->tempbuf, "%d", myUpLoadTestDataBuffer->testdata.testline.TestPoint[i*110+j]);
				else
					sprintf(myUpLoadTestDataBuffer->tempbuf, ",%d", myUpLoadTestDataBuffer->testdata.testline.TestPoint[i*110+j]);
				strcat(myUpLoadTestDataBuffer->sendbuf, myUpLoadTestDataBuffer->tempbuf);
			}
			
			sprintf(myUpLoadTestDataBuffer->tempbuf, "]");
			strcat(myUpLoadTestDataBuffer->sendbuf, myUpLoadTestDataBuffer->tempbuf);
			

			if(My_Pass != UpLoadData("/NCD_YGFXY_Server/upseries.action", myUpLoadTestDataBuffer->sendbuf, strlen(myUpLoadTestDataBuffer->sendbuf)))
				goto END;
		}
		
		//�ϴ�������
		memset(myUpLoadTestDataBuffer->sendbuf, 0, 1024);

		sprintf(myUpLoadTestDataBuffer->sendbuf, "tdata.cid=%s&tdata.t_name=%s&tdata.t_age=%s&tdata.t_sex=%s&tdata.t_phone=%s&tdata.t_job=%s&tdata.t_desc=%s",
			myUpLoadTestDataBuffer->testdata.temperweima.CardPiCi, myUpLoadTestDataBuffer->testdata.user.user_name, myUpLoadTestDataBuffer->testdata.user.user_age, 
			myUpLoadTestDataBuffer->testdata.user.user_sex, myUpLoadTestDataBuffer->testdata.user.user_phone, myUpLoadTestDataBuffer->testdata.user.user_job,
			myUpLoadTestDataBuffer->testdata.user.user_desc);

		if(My_Pass != UpLoadData("/NCD_YGFXY_Server/uptester.action", myUpLoadTestDataBuffer->sendbuf, strlen(myUpLoadTestDataBuffer->sendbuf)))
			goto END;
		
		ReadIndexPlus(1);
		statues = My_Pass;
	}
	
	END:
		MyFree(myUpLoadTestDataBuffer);

	return statues;
}
