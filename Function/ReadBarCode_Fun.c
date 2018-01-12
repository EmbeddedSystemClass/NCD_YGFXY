/***************************************************************************************************
*FileName: ReadBarCode_Fun.c
*Description:
*Author: xsx_kair
*Data:
***************************************************************************************************/

/***************************************************************************************************/
/******************************************Header List********************************************/
/***************************************************************************************************/
#include	"ReadBarCode_Fun.h"

#include	"Usart1_Driver.h"

#include	"QueueUnits.h"

#include	<string.h>
#include	"stdio.h"
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/****************************************File Start*************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************
*FunctionName: ReadBarCodeFunction
*Description: 读取条码枪的数据
*Input: codebuf -- 数据保存地址
*		len -- 要读取的数据长度
*Output: 
*Return: 成功读取数据的长度
*Author: xsx
*Date: 2016年12月16日16:00:36
***************************************************************************************************/
unsigned char ReadBarCodeFunction(char * codebuf, unsigned char len)
{
	unsigned char rxlen = 0;
	
	if(codebuf)
	{
		memset(codebuf, 0, len);
		
		ReceiveDataFromQueue(GetUsart1RXQueue(), NULL, codebuf, len, NULL, 1, 20 / portTICK_RATE_MS, 10 / portTICK_RATE_MS);	
		
		rxlen = strlen(codebuf);
		
		if(rxlen > 1)
		{
			if(codebuf[rxlen-1] == 0x0d)
			{
				codebuf[rxlen-1] = 0;
				rxlen -= 1;
				
				return rxlen;
			}
		}
	}
	
	return 0;
}

void clearBarQueue(void)
{
	xQueueReset(GetUsart1RXQueue());
}


/****************************************end of file************************************************/
