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

#include	"LCD_Driver.h"
#include 	"Iwdg_Driver.h"

#include	"AppFileDao.h"

#include	"MyMem.h"

#include	"Define.h"

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
	unsigned int startAddr = 0;
	unsigned int flashWriteAddr = APPLICATION_ADDRESS;
	unsigned short i = 0;
	unsigned short readSize = 0;
	unsigned char * dataBuf = NULL;
	unsigned int j = 0;
	
	dataBuf = MyMalloc(40*1024);
	
	if(dataBuf)
	{
		//�����û�����
		for(i=4; i<12; i++)
		{
			EraseFlashSectors(FLASH_SECTORS[i], FLASH_SECTORS[i]);
			IWDG_Feed();
		}

		for(i=0; i<100; i++)
		{
			memset(dataBuf, 0xff, 40*1024);
			IWDG_Feed();
			
			readSize = 1;							//���������
			if(My_Pass == ReadAppFile(flashWriteAddr - APPLICATION_ADDRESS, dataBuf, 40*1024, &readSize))
			{
				IWDG_Feed();
				
				if(readSize != 0)
				{
					if(My_Pass == writeFlash(flashWriteAddr, dataBuf, readSize / 4))
					{
						IWDG_Feed();
						flashWriteAddr += readSize/4*4;
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
		
		IWDG_Feed();
		MyFree(dataBuf);
		
		if(readSize == 0)
		{
			DisText(0x2a00, "���³ɹ�!", 10);
			
			deleteAppFileIfExist();
			
			jumpToUserApplicationProgram();
		}
		else
		{
			DisText(0x2a00, "����ʧ��!", 10);
			
			NVIC_SystemReset();
		}

	}
	//����ʧ��
	else
	{
		DisText(0x2a00, "����ʧ��!", 10);
			
		NVIC_SystemReset();
	}
}


