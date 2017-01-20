/***************************************************************************************************
*FileName:ReTestPage
*Description:�ϻ����Խ���
*Author: xsx_kair
*Data:2016��8��15��17:36:56
***************************************************************************************************/

/***************************************************************************************************/
/******************************************Header List********************************************/
/***************************************************************************************************/
#include	"ReTestPage.h"
#include	"LCD_Driver.h"
#include	"Define.h"
#include	"MyMem.h"

#include	"SystemSetPage.h"
#include	"PlaySong_Task.h"
#include	"CardStatues_Data.h"
#include	"CodeScan_Task.h"
#include	"Test_Task.h"
#include	"SDFunction.h"
#include	"System_Data.h"
#include	"OutModel_Fun.h"
#include	"PlaySong_Task.h"
#include	"Ads8325_Driver.h"
#include	"TLV5617_Driver.h"
#include	"LEDCheck_Driver.h"
#include 	"MLX90614_Driver.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"
#include 	"stdlib.h"
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
static ReTestPageBuffer * S_ReTestPageBuffer = NULL;
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
static void activityStart(void);
static void activityInput(unsigned char *pbuf , unsigned short len);
static void activityFresh(void);
static void activityHide(void);
static void activityResume(void);
static void activityDestroy(void);
static MyState_TypeDef activityBufferMalloc(void);
static void activityBufferFree(void);

static void SetReTestCount(char *str, unsigned char len);
static void StartReTest(void);
static void StopReTest(void);
static void EndOneReTest(char *result, unsigned char len);
static void CheckQRCode(void);
static void CheckTestCard(void);
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/****************************************File Start*************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************
*FunctionName: createSelectUserActivity
*Description: ����ѡ������˽���
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:00:09
***************************************************************************************************/
MyState_TypeDef createReTestActivity(Activity * thizActivity, Intent * pram)
{
	if(NULL == thizActivity)
		return My_Fail;
	
	if(My_Pass == activityBufferMalloc())
	{
		InitActivity(thizActivity, "ReTestActivity\0", activityStart, activityInput, activityFresh, activityHide, activityResume, activityDestroy);
		
		return My_Pass;
	}
	
	return My_Fail;
}

/***************************************************************************************************
*FunctionName: activityStart
*Description: ��ʾ������
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:00:32
***************************************************************************************************/
static void activityStart(void)
{
	if(S_ReTestPageBuffer)
	{
		
		timer_set(&(S_ReTestPageBuffer->timer), 10);
		
		memset(S_ReTestPageBuffer->buf, 0, 100);
		sprintf(S_ReTestPageBuffer->buf, "Stopped");
		DisText(0x300c, S_ReTestPageBuffer->buf, strlen(S_ReTestPageBuffer->buf));
	}
	
	SelectPage(117);
}

/***************************************************************************************************
*FunctionName: activityInput
*Description: ��������
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:00:59
***************************************************************************************************/
static void activityInput(unsigned char *pbuf , unsigned short len)
{
	if(S_ReTestPageBuffer)
	{
		/*����*/
		S_ReTestPageBuffer->lcdinput[0] = pbuf[4];
		S_ReTestPageBuffer->lcdinput[0] = (S_ReTestPageBuffer->lcdinput[0]<<8) + pbuf[5];
		
		/*�˳�*/
		if(S_ReTestPageBuffer->lcdinput[0] == 0x3012)
		{
			if(S_ReTestPageBuffer->retestdata.reteststatus == 0)
			{
				backToFatherActivity();
			}
			else
				SendKeyCode(1);
		}
		/*��ʼ�ϻ�*/
		else if(S_ReTestPageBuffer->lcdinput[0] == 0x3010)
		{
			if(S_ReTestPageBuffer->retestdata.reteststatus == 0)
				StartReTest();
			else
				SendKeyCode(1);
		}
		/*ֹͣ�ϻ�*/
		else if(S_ReTestPageBuffer->lcdinput[0] == 0x3011)
		{
			if(S_ReTestPageBuffer->retestdata.reteststatus > 0)
				StopReTest();
		}
		/*��ȡ�ϻ�����*/
		else if(S_ReTestPageBuffer->lcdinput[0] == 0x3000)
		{
			SetReTestCount((char *)(&pbuf[7]), GetBufLen(&pbuf[7] , 2*pbuf[6]));
		}
	}
}

/***************************************************************************************************
*FunctionName: activityFresh
*Description: ����ˢ��
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:01:16
***************************************************************************************************/
static void activityFresh(void)
{
	if(S_ReTestPageBuffer)
	{
		if(S_ReTestPageBuffer->retestdata.reteststatus == 1)
		{
			if(GetCardState() == CardIN)
			{
				//��⵽�忨���򱣳��¶Ⱥ�ʱ��
				S_ReTestPageBuffer->retestdata.testdata.TestTemp.O_Temperature = GetCardTemperature();
					
				GetGB_Time(&(S_ReTestPageBuffer->retestdata.testdata.TestTime));
				
				//�ϻ�AD,DA
				SetGB_LedValue(100);
				vTaskDelay(100 / portTICK_RATE_MS);
				S_ReTestPageBuffer->retestdata.advalue1 = ADS8325();
				
				SetGB_LedValue(200);
				vTaskDelay(100 / portTICK_RATE_MS);
				S_ReTestPageBuffer->retestdata.advalue2 = ADS8325();
				
				SetGB_LedValue(300);
				vTaskDelay(100 / portTICK_RATE_MS);
				S_ReTestPageBuffer->retestdata.advalue3 = ADS8325();
				
				S_ReTestPageBuffer->retestdata.ledstatus = ReadLEDStatus();
				
				S_ReTestPageBuffer->retestdata.reteststatus = 2;
				StartScanQRCode(&(S_ReTestPageBuffer->retestdata.testdata.temperweima));
			}
			else if(TimeOut == timer_expired(&(S_ReTestPageBuffer->timer)))
				EndOneReTest("�ȴ���⿨��ʱ", 14);
		}
		else if(S_ReTestPageBuffer->retestdata.reteststatus == 2)
			CheckQRCode();
		else if(S_ReTestPageBuffer->retestdata.reteststatus == 3)
			CheckTestCard();

		
		S_ReTestPageBuffer->playstatus = getPlayStatus();
		if(S_ReTestPageBuffer->playstatus == 1)
		{
			//���Ŵ���+1
			S_ReTestPageBuffer->retestdata.playcount++;
					
			//��ʼ����ǰ����ʱ��
			timer_set(&(S_ReTestPageBuffer->retestdata.oneplaytimer), 999999);
					
			GetGB_Time(&(S_ReTestPageBuffer->retestdata.startplayTime));
		}
		else if(S_ReTestPageBuffer->playstatus == 0)
		{
			GetGB_Time(&(S_ReTestPageBuffer->retestdata.endplayTime));
					
			//���浱ǰ������Ϣ
			if(My_Pass == SaveReTestData(&(S_ReTestPageBuffer->retestdata), 1))
			{
				if(S_ReTestPageBuffer->retestdata.reteststatus > 0)
					AddNumOfSongToList(55, 3);
			}
			else
			{
				//״̬
				memset(S_ReTestPageBuffer->buf, 0, 100);
				sprintf(S_ReTestPageBuffer->buf, "Failed");
				DisText(0x300c, S_ReTestPageBuffer->buf, strlen(S_ReTestPageBuffer->buf));
					
				StopReTest();
			}
		}
	}
}

/***************************************************************************************************
*FunctionName: activityHide
*Description: ���ؽ���ʱҪ������
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:01:40
***************************************************************************************************/
static void activityHide(void)
{

}

/***************************************************************************************************
*FunctionName: activityResume
*Description: ����ָ���ʾʱҪ������
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:01:58
***************************************************************************************************/
static void activityResume(void)
{

}

/***************************************************************************************************
*FunctionName: activityDestroy
*Description: ��������
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:02:15
***************************************************************************************************/
static void activityDestroy(void)
{
	activityBufferFree();
}

/***************************************************************************************************
*FunctionName: activityBufferMalloc
*Description: ���������ڴ�����
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 
***************************************************************************************************/
static MyState_TypeDef activityBufferMalloc(void)
{
	if(NULL == S_ReTestPageBuffer)
	{
		S_ReTestPageBuffer = MyMalloc(sizeof(ReTestPageBuffer));
		
		if(S_ReTestPageBuffer)
		{
			memset(S_ReTestPageBuffer, 0, sizeof(ReTestPageBuffer));
	
			return My_Pass;
		}
		else
			return My_Fail;
	}
	else
		return My_Pass;
}

/***************************************************************************************************
*FunctionName: activityBufferFree
*Description: �����ڴ��ͷ�
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:03:10
***************************************************************************************************/
static void activityBufferFree(void)
{
	MyFree(S_ReTestPageBuffer);
	S_ReTestPageBuffer = NULL;
}

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
static void SetReTestCount(char *str, unsigned char len)
{
	if(S_ReTestPageBuffer)
	{
		if(S_ReTestPageBuffer->retestdata.reteststatus == 0)
		{
			memset(S_ReTestPageBuffer->buf, 0, 100);
			memcpy(S_ReTestPageBuffer->buf, str, len);
			S_ReTestPageBuffer->retestdata.retestcount = strtol(S_ReTestPageBuffer->buf, NULL, 10);
			if(S_ReTestPageBuffer->retestdata.retestcount > 60000)
				S_ReTestPageBuffer->retestdata.retestcount = 60000;
		}
		
		memset(S_ReTestPageBuffer->buf, 0, 100);
		sprintf(S_ReTestPageBuffer->buf, "%d", S_ReTestPageBuffer->retestdata.retestcount);
		DisText(0x3000, S_ReTestPageBuffer->buf, strlen(S_ReTestPageBuffer->buf));
	}
}
static void StartReTest(void)
{
	if(S_ReTestPageBuffer)
	{
		if(S_ReTestPageBuffer->retestdata.retestcount == 0)
			return;
		
		//���SD�����ϻ�����

		
		//�����ϻ�����״̬
		memset(S_ReTestPageBuffer->buf, 0, 100);
		sprintf(S_ReTestPageBuffer->buf, "Testing");
		DisText(0x300c, S_ReTestPageBuffer->buf, strlen(S_ReTestPageBuffer->buf));
		
		//�������ϻ�����
		S_ReTestPageBuffer->retestdata.retestedcount = 0;
		DspNum(0x3006 , S_ReTestPageBuffer->retestdata.retestedcount, 4);
		
		//����ʣ���ϻ�����
		S_ReTestPageBuffer->retestdata.retestsurpluscount = S_ReTestPageBuffer->retestdata.retestcount - S_ReTestPageBuffer->retestdata.retestedcount;
		DspNum(0x3008 , S_ReTestPageBuffer->retestdata.retestsurpluscount, 4);
		
		//��ʼ��������ʱ����ʱ��
		timer_set(&(S_ReTestPageBuffer->retestdata.retesttimer), 999999);
		//�����ϻ�ʱ��
		DspNum(0x300a , timer_Count(&(S_ReTestPageBuffer->retestdata.retesttimer)), 4);
		
		//��ʼ����ǰ���Ե�ʱ����ʱ��
		timer_set(&(S_ReTestPageBuffer->retestdata.oneretesttimer), 999999);
		
		S_ReTestPageBuffer->retestdata.reteststatus = 1;
		
		for(S_ReTestPageBuffer->buf[0]=0; S_ReTestPageBuffer->buf[0]<PaiDuiWeiNum; S_ReTestPageBuffer->buf[0]++)
			UpOneModelData(S_ReTestPageBuffer->buf[0], R_ON_G_OFF, 5);
			
		//��ʼ������Ƶ
		AddNumOfSongToList(55, 3);
		//��ʼ��������ʱ��
		timer_set(&(S_ReTestPageBuffer->retestdata.playtimer), 999999);
	}
}

static void StopReTest(void)
{
	if(S_ReTestPageBuffer)
	{
		S_ReTestPageBuffer->retestdata.reteststatus = 0;

		memset(S_ReTestPageBuffer->buf, 0, 100);
		sprintf(S_ReTestPageBuffer->buf, "%d", S_ReTestPageBuffer->retestdata.retestcount);
		DisText(0x3000, S_ReTestPageBuffer->buf, strlen(S_ReTestPageBuffer->buf));
		
		OutModel_Init();
	}
}


static void EndOneReTest(char *result, unsigned char len)
{
	if(S_ReTestPageBuffer)
	{
		memset(S_ReTestPageBuffer->retestdata.result, 0, 30);
		memcpy(S_ReTestPageBuffer->retestdata.result, result, len);
		
		//���浱ǰ���Խ��
		if(My_Pass == SaveReTestData(&(S_ReTestPageBuffer->retestdata), 0))
		{
			//����ϴβ�������
			memset(&(S_ReTestPageBuffer->retestdata.testdata), 0, sizeof(TestData));
			
			if(S_ReTestPageBuffer->retestdata.retestedcount < S_ReTestPageBuffer->retestdata.retestcount)
				S_ReTestPageBuffer->retestdata.retestedcount++;
			
			S_ReTestPageBuffer->retestdata.retestsurpluscount = S_ReTestPageBuffer->retestdata.retestcount - S_ReTestPageBuffer->retestdata.retestedcount;
			S_ReTestPageBuffer->retestdata.reteststatus = 1;
			
			//�Ѳ�
			DspNum(0x3006 , S_ReTestPageBuffer->retestdata.retestedcount, 4);
			//ʣ��
			DspNum(0x3008 , S_ReTestPageBuffer->retestdata.retestsurpluscount, 4);
			//�Ѳ�ʱ��
			DspNum(0x300a , timer_Count(&(S_ReTestPageBuffer->retestdata.retesttimer)), 4);
			//״̬
			memset(S_ReTestPageBuffer->buf, 0, 100);
			if(S_ReTestPageBuffer->retestdata.retestsurpluscount == 0)
				sprintf(S_ReTestPageBuffer->buf, "Stopped");
			else
				sprintf(S_ReTestPageBuffer->buf, "Testing");
			DisText(0x300c, S_ReTestPageBuffer->buf, strlen(S_ReTestPageBuffer->buf));
			
			if(S_ReTestPageBuffer->retestdata.retestedcount >= S_ReTestPageBuffer->retestdata.retestcount)
				StopReTest();
			
			//��ʼ����ǰ���Ե�ʱ����ʱ��
			timer_set(&(S_ReTestPageBuffer->retestdata.oneretesttimer), 999999);
			
			//��ʼ���忨��ʱ
			timer_set(&(S_ReTestPageBuffer->timer), 10);

		}
		else
		{
			//״̬
			memset(S_ReTestPageBuffer->buf, 0, 100);
			sprintf(S_ReTestPageBuffer->buf, "Failed");
			DisText(0x300c, S_ReTestPageBuffer->buf, strlen(S_ReTestPageBuffer->buf));
			
			StopReTest();
		}
	}
}

static void CheckQRCode(void)
{
	if((S_ReTestPageBuffer) && (My_Pass == TakeScanQRCodeResult(&(S_ReTestPageBuffer->scancode))))
	{
		if(S_ReTestPageBuffer->scancode == CardCodeScanFail)
		{
			EndOneReTest("��ά���ȡʧ��", 14);
		}
		else if(S_ReTestPageBuffer->scancode == CardCodeCardOut)
		{
			EndOneReTest("��ά���⿨�γ�", 16);
		}
		else if(S_ReTestPageBuffer->scancode == CardCodeScanTimeOut)
		{
			EndOneReTest("��ά���ȡ��ʱ", 14);
		}
		else if(S_ReTestPageBuffer->scancode == CardCodeCRCError)
		{
			EndOneReTest("��ά��У�����", 14);
		}
		else
		{
			//��ȡУ׼����
			memcpy(S_ReTestPageBuffer->retestdata.testdata.tempadjust.ItemName, S_ReTestPageBuffer->retestdata.testdata.temperweima.ItemName, ItemNameLen);
			if(My_Fail == ReadAdjustData(&(S_ReTestPageBuffer->retestdata.testdata.tempadjust)))
				memset(&(S_ReTestPageBuffer->retestdata.testdata.tempadjust), 0, sizeof(AdjustData));

			S_ReTestPageBuffer->retestdata.reteststatus = 3;
			StartTest(&(S_ReTestPageBuffer->retestdata.testdata));
		}
	}
}

static void CheckTestCard(void)
{
	if((S_ReTestPageBuffer) && (My_Pass == TakeTestResult(&(S_ReTestPageBuffer->cardpretestresult))))
	{
		if(S_ReTestPageBuffer->cardpretestresult == ResultIsOK)
		{
			EndOneReTest("��������", 8);
		}
		else if(S_ReTestPageBuffer->cardpretestresult == NoSample)
		{
			EndOneReTest("δ����", 6);
		}
		else
		{
			EndOneReTest("��⿨�γ�", 10);
		}
	}
}
/****************************************end of file************************************************/
