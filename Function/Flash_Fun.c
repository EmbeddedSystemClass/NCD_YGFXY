/***************************************************************************************************
*FileName:Flash_Fun
*Description:оƬflash����
*Author:xsx
*Data:2017��2��17��09:57:36
***************************************************************************************************/


/***************************************************************************************************/
/******************************************ͷ�ļ�***************************************************/
/***************************************************************************************************/
#include	"Flash_Fun.h"
#include 	"stm32f4xx.h"
#include	"stm32f4xx_flash.h"

#include	"MyMem.h"

#include	"Define.h"

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
*FunctionName: getFlashSector
*Description: ���ݵ�ַ��ȡ��ַ����flash ���
*Input: startAddress -- ��ַ
*Output: 
*Return: 
*Author: xsx
*Date: 2017��2��17��10:14:39
***************************************************************************************************/
unsigned short getFlashSector(unsigned int startAddress)
{
  if((startAddress < ADDR_FLASH_SECTOR_1) && (startAddress >= ADDR_FLASH_SECTOR_0))
  {
    return FLASH_Sector_0;  
  }
  else if((startAddress < ADDR_FLASH_SECTOR_2) && (startAddress >= ADDR_FLASH_SECTOR_1))
  {
    return FLASH_Sector_1;  
  }
  else if((startAddress < ADDR_FLASH_SECTOR_3) && (startAddress >= ADDR_FLASH_SECTOR_2))
  {
    return FLASH_Sector_2;  
  }
  else if((startAddress < ADDR_FLASH_SECTOR_4) && (startAddress >= ADDR_FLASH_SECTOR_3))
  {
    return FLASH_Sector_3;  
  }
  else if((startAddress < ADDR_FLASH_SECTOR_5) && (startAddress >= ADDR_FLASH_SECTOR_4))
  {
    return FLASH_Sector_4;  
  }
  else if((startAddress < ADDR_FLASH_SECTOR_6) && (startAddress >= ADDR_FLASH_SECTOR_5))
  {
    return FLASH_Sector_5;  
  }
  else if((startAddress < ADDR_FLASH_SECTOR_7) && (startAddress >= ADDR_FLASH_SECTOR_6))
  {
    return FLASH_Sector_6;  
  }
  else if((startAddress < ADDR_FLASH_SECTOR_8) && (startAddress >= ADDR_FLASH_SECTOR_7))
  {
    return FLASH_Sector_7;  
  }
  else if((startAddress < ADDR_FLASH_SECTOR_9) && (startAddress >= ADDR_FLASH_SECTOR_8))
  {
    return FLASH_Sector_8;  
  }
  else if((startAddress < ADDR_FLASH_SECTOR_10) && (startAddress >= ADDR_FLASH_SECTOR_9))
  {
    return FLASH_Sector_9;  
  }
  else if((startAddress < ADDR_FLASH_SECTOR_11) && (startAddress >= ADDR_FLASH_SECTOR_10))
  {
    return FLASH_Sector_10;  
  }
  else
  {
    return FLASH_Sector_11;  
  }
}

/***************************************************************************************************
*FunctionName: EraseFlash
*Description: �����û���flash����
*Input: startSectorIndex -- ��ʼ��������
*		eraseNum -- ��������Ŀ
*Output: 
*Return: 
*Author: xsx
*Date: 2017��2��17��10:00:06
***************************************************************************************************/
MyState_TypeDef EraseFlashSectors(unsigned int startSectorIndex, unsigned int endSectorIndex)
{
	unsigned int index = 0U;
	FLASH_Status status = FLASH_COMPLETE;
	
	FLASH_Unlock();
	
	for(index = startSectorIndex; index <= endSectorIndex; )
	{
		status = FLASH_EraseSector(index, VoltageRange_3);
		if(FLASH_COMPLETE != status)
			break;
		
		index += 0x08;
	}
	
	FLASH_Lock();
	
	if(status == FLASH_COMPLETE)
		return My_Pass;
	else
		return My_Fail;
}

/***************************************************************************************************
*FunctionName: EraseFlashSector
*Description: ����һ����
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2017��2��24��15:28:46
***************************************************************************************************/
MyState_TypeDef EraseFlashSector(unsigned int sectorIndex)
{

	FLASH_Status status = FLASH_COMPLETE;
	
	FLASH_Unlock();
	
	status = FLASH_EraseSector(sectorIndex, VoltageRange_3);

	FLASH_Lock();
	
	if(status == FLASH_COMPLETE)
		return My_Pass;
	else
		return My_Fail;
}

/***************************************************************************************************
*FunctionName: writeFlash
*Description: дflash
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2017��2��17��11:02:43
***************************************************************************************************/
MyState_TypeDef writeFlash(unsigned int FlashAddress, void * Data , unsigned int DataLength)
{
	uint32_t i = 0;
	FLASH_Status status = FLASH_COMPLETE;
	unsigned int * p = (unsigned int *)Data;
	
	FLASH_Unlock();
	
	for (i = 0; (i < DataLength) && (FlashAddress <= (APPLICATION_END_ADDRESS-4)); i++)
	{
		status = FLASH_ProgramWord(FlashAddress, *p);
		if(status != FLASH_COMPLETE)
		{
			break;
		}
		
		FlashAddress += 4;
		
		p++;
	}
	
	FLASH_Lock();
	
	if(status == FLASH_COMPLETE)
		return My_Pass;
	else
		return My_Fail;
}

/***************************************************************************************************
*FunctionName: writeFlash
*Description: дflash
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2017��2��17��11:02:43
***************************************************************************************************/
void readFlash(unsigned int FlashAddress, void * Data , unsigned int DataLength)
{
	uint32_t i = 0;
	unsigned int * p = (unsigned int *)Data;
	
	for(i=0; i<DataLength; i++)
	{
		p[i] = *(unsigned int *)FlashAddress;
		FlashAddress += 4;
	}
}

void testFlashWriteAndReadFunction(void)
{
	unsigned char * dataBuf = NULL;
	unsigned int j = 0;
	unsigned int flashWriteAddr = APPLICATION_ADDRESS;
	
	dataBuf = MyMalloc(1024);
	
	if(dataBuf)
	{
		for(j=0; j<1024; j++)
			dataBuf[j] = j;
		
		//�����û�����
		EraseFlashSectors(getFlashSector(APPLICATION_ADDRESS), FLASH_Sector_11);
		
//		for(j=4; j<12; j++)
//		{
			if(My_Pass == writeFlash(FLASH_Sector_4, dataBuf, 256))
			{

			}
			
			if(My_Pass == writeFlash(FLASH_Sector_4+1024, dataBuf, 256))
			{
	
			}
//		}
		
		//�����û�����
		EraseFlashSectors(getFlashSector(APPLICATION_ADDRESS), FLASH_Sector_11);
	}
	
	MyFree(dataBuf);
}