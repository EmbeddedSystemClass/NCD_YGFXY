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
		EraseFlashSectors(getFlashSector(APPLICATION_ADDRESS), FLASH_Sector_11);
		
		for(i=0; i<100; i++)
		{
			memset(dataBuf, 0xff, 40*1024);
			if(My_Pass == ReadAppFile(flashWriteAddr - APPLICATION_ADDRESS, dataBuf, 40*1024, &readSize))
			{
				if(readSize != 0)
				{
					if(My_Pass == writeFlash(flashWriteAddr, dataBuf, readSize / 4))
					{
						flashWriteAddr += readSize/4*4;
					}
				}
				else
					break;
			}
		}
		
		MyFree(dataBuf);
		jumpToUserApplicationProgram();
	}
	//����ʧ��
	else
		jumpToUserApplicationProgram();
}


