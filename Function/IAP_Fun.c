/***************************************************************************************************
*FileName:IAP_Fun
*Description:IAP����
*Author:xsx
*Data:2017��2��16��16:27:25
***************************************************************************************************/


/***************************************************************************************************/
/******************************************ͷ�ļ�***************************************************/
/***************************************************************************************************/
#include	"IAP_Fun.h"
#include 	"stm32f4xx.h"
#include	"Flash_Fun.h"
#include	"IAPMessage_Fun.h"

#include	"LCD_Driver.h"

#include	"RemoteSoftDao.h"
#include	"AppFileDao.h"

#include	"MyMem.h"
#include	"Md5.h"
#include	"Define.h"
#include	"MyTools.h"
#include	"Delay.h"

#include	<string.h>
#include	"stdio.h"
#include 	"stdlib.h"

/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/
pFunction JumpToApplication;
uint32_t JumpAddress;
const unsigned int FLASH_SECTORS[12]=
{
	FLASH_Sector_0,
	FLASH_Sector_1,
	FLASH_Sector_2, 
	FLASH_Sector_3,  
	FLASH_Sector_4, 
	FLASH_Sector_5,
	FLASH_Sector_6,
	FLASH_Sector_7, 
	FLASH_Sector_8,  
	FLASH_Sector_9, 
	FLASH_Sector_10,
	FLASH_Sector_11
};
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
*FunctionName: jumpToUserApplicationProgram
*Description: ��ת��Ӧ�ó���
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2017��2��16��16:38:47
***************************************************************************************************/
void jumpToUserApplicationProgram(void)
{
	if (((*(__IO uint32_t*)APPLICATION_ADDRESS) & 0x2FFE0000 ) == 0x20000000)
    {
      /* Jump to user application */
      JumpAddress = *(__IO uint32_t*) (APPLICATION_ADDRESS + 4);
      JumpToApplication = (pFunction) JumpAddress;
      /* Initialize user application's Stack Pointer */
      __set_MSP(*(__IO uint32_t*) APPLICATION_ADDRESS);
      JumpToApplication();
    }
}

/***************************************************************************************************
*FunctionName: checkMd5
*Description: ���㲢�Ա�MD5
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2017��2��20��14:02:35
***************************************************************************************************/
MyState_TypeDef checkMd5(RemoteSoftInfo * remoteSoftInfo)
{
	
	char currentMd5[40];						//�����md5

	memset(currentMd5, 0, 40);
	md5sum(currentMd5);
	
	if(true == CheckStrIsSame(remoteSoftInfo->md5, currentMd5, 32))
		return My_Pass;
	else
		return My_Fail;
}

/***************************************************************************************************
*FunctionName: writeApplicationToFlash
*Description: ���¹̼���flash
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2017��2��16��17:06:02
***************************************************************************************************/
void writeApplicationToFlash(void)
{
	unsigned int fileSize = 0;
	unsigned int flashWriteAddr = APPLICATION_ADDRESS;
	unsigned short i = 0;
	unsigned short readSize = 0;
	unsigned char * dataBuf = NULL;
	unsigned int j = 0;
	char statusBuf[50];
	unsigned int hasWriteSize = 0;
	double tempValue = 0.0;
	
	dataBuf = MyMalloc(40*1024);
	
	if(dataBuf)
	{
		//�����û�����
		showIapStatus("Erase       \0");
		
		for(i=4; i<12; i++)
		{
			if(My_Pass == EraseFlashSector(FLASH_SECTORS[i]))
			{		
				if(i == 11)
					showIapStatus("-  Done ! Programming\0");
				else
					showIapStatus("-\0");
			}
			else
			{
				memset(statusBuf, 0, 50);
				sprintf(statusBuf, "%.*s\0", 12-i, "--------");
				strcat(statusBuf, "  Fail ! Programming\0");
				showIapStatus(statusBuf);
				break;
			}
		}

		
		for(i=0; i<100; i++)
		{
			memset(dataBuf, 0xff, 40*1024);
			
			readSize = 1;							//���������
			if(My_Pass == ReadAppFile(flashWriteAddr - APPLICATION_ADDRESS, dataBuf, 40*1024, &readSize, &fileSize))
			{
				if(readSize != 0)
				{
					if(My_Pass == writeFlash(flashWriteAddr, dataBuf, readSize / 4))
					{
						flashWriteAddr += readSize/4*4;
						
						hasWriteSize = (flashWriteAddr - APPLICATION_ADDRESS);
						tempValue = hasWriteSize;
						tempValue /= fileSize;
						tempValue *= 100;
						
						showIapProgess(tempValue);
						
						delay_ms(100);
					}
					else
						break;
				}
				else
					break;
			}
			else
				break;
		}
		
		MyFree(dataBuf);
		
		if(readSize == 0)
		{
			showIapStatus(" --------  Done ! Application Running   ...    \0");
			delay_ms(500);
			
			//deleteAppFileIfExist();
			
			jumpToUserApplicationProgram();
		}
		else
		{
			showIapStatus(" --------  Fail ! Restartting ...\0");
			
			//deleteAppFileIfExist();
			
			NVIC_SystemReset();
		}

	}
	//����ʧ��
	else
	{
		showIapStatus(" --------  Fail ! Restartting ...\0");
			
		//deleteAppFileIfExist();
			
		NVIC_SystemReset();
	}
}

void BootLoaderMainFunction(void)
{
	RemoteSoftInfo * remoteSoftInfo = NULL;		//��ȡ�Ĺ̼���Ϣ
	
	remoteSoftInfo = MyMalloc(sizeof(RemoteSoftInfo));
	if(remoteSoftInfo)
	{
		memset(remoteSoftInfo, 0, sizeof(RemoteSoftInfo));

		//����Ƿ����³���
		if(My_Pass == checkNewAppFileIsExist())
		{
			delay_ms(50);
			clearStatusText();		
			delay_ms(500);
			SelectPage(119);
			
			showIapStatus("New Firmware Detected !      Check MD5\0");

			delay_ms(100);
			if((My_Pass == ReadRemoteSoftInfo(remoteSoftInfo)) && (My_Pass == checkMd5(remoteSoftInfo)))
			{
				showIapVersion(remoteSoftInfo->RemoteFirmwareVersion);

				showIapStatus("   --------  OK !   \0");
				writeApplicationToFlash();
			}
			else
			{
				showIapStatus("   --------  Fail ! \0");
			}
		}
	}
	
	MyFree(remoteSoftInfo);
	
	jumpToUserApplicationProgram();
}


