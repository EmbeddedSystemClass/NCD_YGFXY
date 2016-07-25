/***************************************************************************************************
*FileName:SDFunction
*Description:һЩSD���Ĳ�������
*Author:xsx
*Data:2016��4��30��16:06:36
***************************************************************************************************/


/***************************************************************************************************/
/******************************************ͷ�ļ�***************************************************/
/***************************************************************************************************/
#include	"SDFunction.h"
#include	"Net_Data.h"

#include	"CRC16.h"
#include	"MyMem.h"
#include	"MyTools.h"

#include 	"FreeRTOS.h"
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





/***************************************************************************************************/
/***************************************************************************************************/
/***************************************����********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************
*FunctionName��CheckSDFunction
*Description�����SD���Ƿ�����
*Input��None
*Output��None
*Author��xsx
*Data��2016��4��30��16:38:48
***************************************************************************************************/
MyState_TypeDef CheckSDFunction(void)
{
	FatfsFileInfo_Def * myfile = NULL;
	MyState_TypeDef statues = My_Fail;
	
	myfile = MyMalloc(sizeof(FatfsFileInfo_Def));

	if(myfile)
	{
		memset(myfile, 0, sizeof(FatfsFileInfo_Def));

		myfile->res = f_open(&(myfile->file), "0:/SDCheck.ncd", FA_READ);
			
		if(FR_OK == myfile->res)
		{
			myfile->size = f_size(&(myfile->file));
			
			if(myfile->size == 4078)
				statues = My_Pass;

			f_close(&(myfile->file));
		}
	}
	
	MyFree(myfile);
	
	return statues;
}

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************��д�豸��Ϣ************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************
*FunctionName��SaveDeviceInfo
*Description��save device info
*Input��deviceinfo -- �豸��Ϣ��ַ
*Output��My_Fail -- ����ʧ��
*		 My_Pass -- ����ɹ�
*Author��xsx
*Data��2016��7��8��17:03:15
***************************************************************************************************/
MyState_TypeDef SaveDeviceInfo(DeviceInfo * deviceinfo)
{
	FatfsFileInfo_Def * myfile = NULL;
	MyState_TypeDef statues = My_Fail;
	
	myfile = MyMalloc(sizeof(FatfsFileInfo_Def));
	
	if(myfile && deviceinfo)
	{
		memset(myfile, 0, sizeof(FatfsFileInfo_Def));

		myfile->res = f_open(&(myfile->file), "0:/DeviceInfo.ncd", FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
			
		if(FR_OK == myfile->res)
		{
			f_lseek(&(myfile->file), 0);
			
			deviceinfo->crc = CalModbusCRC16Fun1(deviceinfo, sizeof(DeviceInfo)-2);
			
			myfile->res = f_write(&(myfile->file), deviceinfo, sizeof(DeviceInfo), &(myfile->bw));
				
			if((FR_OK == myfile->res)&&(myfile->bw == sizeof(DeviceInfo)))
				statues = My_Pass;
			
			f_close(&(myfile->file));
		}
	}
	
	MyFree(myfile);
	
	return statues;
}

/***************************************************************************************************
*FunctionName��ReadDeviceInfo
*Description����ȡ�豸��Ϣ
*Input��None
*Output��None
*Author��xsx
*Data��2016��7��8��17:03:33
***************************************************************************************************/
MyState_TypeDef ReadDeviceInfo(DeviceInfo * deviceinfo)
{
	FatfsFileInfo_Def * myfile = NULL;
	MyState_TypeDef statues = My_Fail;
	
	myfile = MyMalloc(sizeof(FatfsFileInfo_Def));
	
	if(myfile && deviceinfo)
	{
		memset(myfile, 0, sizeof(FatfsFileInfo_Def));

		myfile->res = f_open(&(myfile->file), "0:/DeviceInfo.ncd", FA_READ);
		
		if(FR_OK == myfile->res)
		{
			f_lseek(&(myfile->file), 0);
					
			myfile->res = f_read(&(myfile->file), deviceinfo, sizeof(DeviceInfo), &(myfile->br));
			if((FR_OK == myfile->res)&&(myfile->br == sizeof(DeviceInfo)))
				statues = My_Pass;

			f_close(&(myfile->file));
		}
	}
	MyFree(myfile);
	
	return statues;
}

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************��д������**************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
MyState_TypeDef SaveUserData(User_Type * user)
{
	FatfsFileInfo_Def * myfile = NULL;
	MyState_TypeDef statues = My_Fail;
	
	myfile = MyMalloc(sizeof(FatfsFileInfo_Def));
	
	if(myfile)
	{
		memset(myfile, 0, sizeof(FatfsFileInfo_Def));

		myfile->res = f_open(&(myfile->file), "0:/Testers.ncd", FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
			
		if(FR_OK == myfile->res)
		{
			f_lseek(&(myfile->file), 0);
			
			myfile->res = f_write(&(myfile->file), user, sizeof(User_Type)*MaxUserNum, &(myfile->bw));
				
			if((FR_OK == myfile->res)&&(myfile->bw == sizeof(User_Type)*MaxUserNum))
				statues = My_Pass;
				
			f_close(&(myfile->file));
		}
	}
	
	MyFree(myfile);
	
	return statues;
}

void ReadUserData(User_Type * user)
{
	FatfsFileInfo_Def * myfile = NULL;
	unsigned char i = 0;

	myfile = MyMalloc(sizeof(FatfsFileInfo_Def));

	if(myfile)
	{
		memset(myfile, 0, sizeof(FatfsFileInfo_Def));

		myfile->res = f_open(&(myfile->file), "0:/Testers.ncd", FA_READ);
		
		if(FR_OK == myfile->res)
		{
			for(i=0; i<MaxUserNum; i++)
			{
				f_lseek(&(myfile->file), i*sizeof(User_Type));
					
				myfile->res = f_read(&(myfile->file), user, sizeof(User_Type), &(myfile->br));
				if((FR_OK == myfile->res)&&(myfile->br == sizeof(User_Type)))
				{
					if(user->crc == CalModbusCRC16Fun1(user, sizeof(User_Type)-2))
					{
						user++;
					}
				}
			}
			f_close(&(myfile->file));
		}
	}	
	MyFree(myfile);
}

/***************************************************************************************************/
/***************************************************************************************************/
/********************************************��������***********************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************
*FunctionName��SaveTestData
*Description�������������
*Input��None
*Output��None
*Author��xsx
*Data��2016��7��16��19:43:40
***************************************************************************************************/
MyState_TypeDef SaveTestData(TestData *tempdata)
{
	FatfsFileInfo_Def * myfile = NULL;
	char *buf = NULL;
	MyState_TypeDef statues = My_Fail;
	
	myfile = MyMalloc(sizeof(FatfsFileInfo_Def));
	buf = MyMalloc(100);
	
	if(myfile && buf)
	{
		memset(myfile, 0, sizeof(FatfsFileInfo_Def));
		memset(buf, 0, 100);

		myfile->res = f_open(&(myfile->file), "0:/TestData.ncd", FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
			
		if(FR_OK == myfile->res)
		{
			myfile->size = f_size(&(myfile->file));
			f_lseek(&(myfile->file), myfile->size);
				
			tempdata->crc = CalModbusCRC16Fun1(tempdata, sizeof(TestData)-2);
			myfile->res = f_write(&(myfile->file), tempdata, sizeof(TestData), &(myfile->bw));
			if((FR_OK == myfile->res)&&(myfile->bw == sizeof(TestData)))
				statues = My_Pass;
				
			f_close(&(myfile->file));
		}
	}
	
	MyFree(myfile);
	MyFree(buf);
	
	return statues;
}

MyState_TypeDef UpDataManegerFile(TestData *tempdata)
{
	
}
/***************************************************************************************************
*FunctionName��ReadTestData
*Description����ȡ��������
*Input��filename -- ����ȡ���ݵ��ļ���
*		tempdata -- ��ŵ�ַ���˵�ַ���ٰ���һҳ���ݵĿռ�
*		index -- �ӵڼ������ݿ�ʼ��ȡ������һҳ����
*Output��None
*Author��xsx
*Data��
***************************************************************************************************/
MyState_TypeDef ReadTestData(const char * filename, TestData *tempdata, unsigned short index)
{
	FatfsFileInfo_Def * myfile = NULL;
	MyState_TypeDef statues = My_Fail;
	unsigned char i=0;
	
	myfile = MyMalloc(sizeof(FatfsFileInfo_Def));
	
	if(myfile)
	{
		myfile->res = f_open(&(myfile->file), filename, FA_READ);
		if(FR_OK == myfile->res)
		{
			myfile->size = f_size(&(myfile->file));
			
			/*�����ȡ��ĩβ*/
			if(index*DataNumInPage*sizeof(TestData) < myfile->size)
			{
				myfile->res = f_lseek(&(myfile->file), index*DataNumInPage*sizeof(TestData));
				if(FR_OK == myfile->res)
				{
					memset(tempdata, 0, sizeof(TestData)*DataNumInPage);
					for(i=0; i<DataNumInPage; i++)
					{
						myfile->res = f_read(&(myfile->file), tempdata, sizeof(TestData), &(myfile->br));
						if((FR_OK == myfile->res) && (sizeof(TestData) == myfile->br) && (tempdata->crc == CalModbusCRC16Fun1(tempdata, sizeof(TestData)-2)))
							tempdata++;
						else
							break;
					}
					if(i > 0)
						statues = My_Pass;
				}
			}
			
			f_close(&(myfile->file));
		}
	}
	
	MyFree(myfile);
	
	return statues;
}


/***************************************************************************************************/
/***************************************************************************************************/
/*************************************IP����********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
MyState_TypeDef SaveNetData(NetData * netdata)
{
	FatfsFileInfo_Def * myfile = NULL;
	MyState_TypeDef statues = My_Fail;
	
	myfile = MyMalloc(sizeof(FatfsFileInfo_Def));

	if(myfile)
	{
		memset(myfile, 0, sizeof(FatfsFileInfo_Def));

		myfile->res = f_open(&(myfile->file), "0:/NetSet.ncd", FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
			
		if(FR_OK == myfile->res)
		{
			f_lseek(&(myfile->file), 0);
				
			netdata->crc = CalModbusCRC16Fun1(netdata, sizeof(NetData)-2);
				
			myfile->res = f_write(&(myfile->file), netdata, sizeof(NetData), &(myfile->bw));
				
			if((FR_OK == myfile->res)&&(myfile->bw == sizeof(NetData)))
				statues = My_Pass;

			myfile->res = f_close(&(myfile->file));
		}
	}
	
	MyFree(myfile);
	
	return statues;
}

MyState_TypeDef ReadNetData(NetData * netdata)
{
	FatfsFileInfo_Def * myfile = NULL;
	MyState_TypeDef statues = My_Fail;
	
	myfile = MyMalloc(sizeof(FatfsFileInfo_Def));
	
	if(myfile)
	{
		memset(myfile, 0, sizeof(FatfsFileInfo_Def));

		myfile->res = f_open(&(myfile->file), "0:/NetSet.ncd", FA_READ);
		
		if(FR_OK == myfile->res)
		{
			f_lseek(&(myfile->file), 0);
					
			myfile->res = f_read(&(myfile->file), netdata, sizeof(NetData), &(myfile->br));
			if((FR_OK == myfile->res)&&(myfile->br == sizeof(NetData))&&(netdata->crc == CalModbusCRC16Fun1(netdata, sizeof(NetData)-2)))
			{
				statues = My_Pass;
			}

			myfile->res = f_close(&(myfile->file));
		}
	}
	MyFree(myfile);
	
	return statues;
}
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************wifi���뱣��************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
MyState_TypeDef SaveWifiData(WIFI_Def * wifi)
{
	FatfsFileInfo_Def * myfile = NULL;
	WIFI_Def * tempwifi = NULL;
	MyState_TypeDef statues = My_Fail;
	unsigned char i=0;
	
	myfile = MyMalloc(sizeof(FatfsFileInfo_Def));
	tempwifi = MyMalloc(sizeof(WIFI_Def));
	if(myfile && tempwifi)
	{
		memset(myfile, 0, sizeof(FatfsFileInfo_Def));
		memset(tempwifi, 0, sizeof(WIFI_Def));

		myfile->res = f_open(&(myfile->file), "0:/WifiSet.ncd", FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
			
		if(FR_OK == myfile->res)
		{
			myfile->size = f_size(&(myfile->file));

			for(i=0; i<255; i++)
			{
				f_lseek(&(myfile->file), i*sizeof(WIFI_Def));
				myfile->res = f_read(&(myfile->file), tempwifi, sizeof(WIFI_Def), &(myfile->br));
				if((FR_OK == myfile->res)&&(myfile->br == sizeof(WIFI_Def))&&(tempwifi->crc == CalModbusCRC16Fun1(tempwifi, sizeof(WIFI_Def)-2)))
				{
					if(pdPASS == CheckStrIsSame(wifi->ssid, tempwifi->ssid, MaxSSIDLen))
					{
						break;
					}
				}
				else
					break;
			}
			if(i<255)
			{
				f_lseek(&(myfile->file), i*sizeof(WIFI_Def));
				wifi->crc = CalModbusCRC16Fun1(wifi, sizeof(WIFI_Def)-2);
				myfile->res = f_write(&(myfile->file), wifi, sizeof(WIFI_Def), &(myfile->bw));
				if((FR_OK == myfile->res)&&(myfile->bw == sizeof(WIFI_Def)))
					statues = My_Pass;
			}

			f_close(&(myfile->file));
		}
	}
	MyFree(tempwifi);
	MyFree(myfile);
	
	return statues;
}

MyState_TypeDef ReadWifiData(WIFI_Def * wifi)
{
	FatfsFileInfo_Def * myfile = NULL;
	MyState_TypeDef statues = My_Fail;
	unsigned char i = 0;
	WIFI_Def * tempwifi = NULL;
	
	myfile = MyMalloc(sizeof(FatfsFileInfo_Def));
	tempwifi = MyMalloc(sizeof(WIFI_Def));
	
	if(myfile && tempwifi)
	{
		memset(myfile, 0, sizeof(FatfsFileInfo_Def));
		memset(tempwifi, 0, sizeof(WIFI_Def));
		
		myfile->res = f_open(&(myfile->file), "0:/WifiSet.ncd", FA_READ);
		
		if(FR_OK == myfile->res)
		{
			myfile->size = f_size(&(myfile->file));
			
			for(i=0; i<(myfile->size / sizeof(WIFI_Def)); i++)
			{
				f_lseek(&(myfile->file), i*sizeof(WIFI_Def));
				
				memset(tempwifi, 0, sizeof(WIFI_Def));
				myfile->res = f_read(&(myfile->file), tempwifi, sizeof(WIFI_Def), &(myfile->br));
				if((FR_OK == myfile->res)&&(myfile->br == sizeof(WIFI_Def)))
				{
					if((tempwifi->crc == CalModbusCRC16Fun1(tempwifi, sizeof(WIFI_Def)-2))&&(pdPASS == CheckStrIsSame(wifi->ssid, tempwifi->ssid, MaxSSIDLen)))
					{
						memcpy(wifi->key, tempwifi->key, MaxKEYLen);
						statues = My_Pass;
						break;
					}
				}
			}

			f_close(&(myfile->file));
		}
	}
	
	MyFree(tempwifi);
	MyFree(myfile);
	
	return statues;
}

MyState_TypeDef ClearWifiData(WIFI_Def * wifi)
{
	FatfsFileInfo_Def * myfile = NULL;
	WIFI_Def * tempwifi = NULL;
	MyState_TypeDef statues = My_Fail;
	unsigned short i=0;
	
	myfile = MyMalloc(sizeof(FatfsFileInfo_Def));
	tempwifi = MyMalloc(sizeof(WIFI_Def));
	
	if(myfile && tempwifi)
	{
		memset(myfile, 0, sizeof(FatfsFileInfo_Def));
		memset(tempwifi, 0, sizeof(WIFI_Def));

		myfile->res = f_open(&(myfile->file), "0:/WifiSet.ncd", FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
			
		if(FR_OK == myfile->res)
		{
			myfile->size = f_size(&(myfile->file));
			
			for(i=0; i<(myfile->size / sizeof(WIFI_Def)); i++)
			{
				f_lseek(&(myfile->file), i*sizeof(WIFI_Def));
				
				memset(tempwifi, 0, sizeof(WIFI_Def));
				myfile->res = f_read(&(myfile->file), tempwifi, sizeof(WIFI_Def), &(myfile->br));
				if((FR_OK == myfile->res)&&(myfile->br == sizeof(WIFI_Def)))
				{
					if((tempwifi->crc == CalModbusCRC16Fun1(tempwifi, sizeof(WIFI_Def)-2))&&(pdPASS == CheckStrIsSame(wifi->ssid, tempwifi->ssid, MaxSSIDLen)))
					{
						tempwifi->crc = 0;
						myfile->res = f_write(&(myfile->file), tempwifi, sizeof(WIFI_Def), &(myfile->bw));
						if((FR_OK == myfile->res)&&(myfile->bw == sizeof(WIFI_Def)))
							statues = My_Pass;
						break;
					}
				}
			}

			f_close(&(myfile->file));
		}
	}
	MyFree(tempwifi);
	MyFree(myfile);
	
	return statues;
}
