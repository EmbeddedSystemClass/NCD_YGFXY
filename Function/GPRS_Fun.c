/***************************************************************************************************
*FileName:WifiFunction
*Description:一些WIFI模块的操作函数,函数只能在线程中使用
*Author:xsx
*Data:2016年4月30日16:06:36
***************************************************************************************************/


/***************************************************************************************************/
/******************************************头文件***************************************************/
/***************************************************************************************************/
#include	"Define.h"

#if (DEVICE_CON_TYPE == DEVICE_GPRS)

#include	"GPRS_Fun.h"

#include	"ServerFun.h"
#include 	"Usart4_Driver.h"
#include	"QueueUnits.h"
#include	"MyTools.h"

#include	"CRC16.h"
#include	"MyMem.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"
#include	"semphr.h"

#include	<string.h>
#include	"stdio.h"
#include 	"stdlib.h"

/***************************************************************************************************/
/**************************************局部变量声明*************************************************/
/***************************************************************************************************/


/***************************************************************************************************/
/**************************************局部函数声明*************************************************/
/***************************************************************************************************/
static MyState_TypeDef ComWithWIFI(char * cmd, const char *strcmp, char *buf, unsigned short buflen, portTickType xBlockTime);
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************正文********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/


/***************************************************************************************************
*FunctionName: ComWithWIFI与wifi模块通信
*Description: 
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2017年3月7日09:41:31
***************************************************************************************************/
static MyState_TypeDef ComWithWIFI(char * cmd, const char *strcmp, char *buf, unsigned short buflen, portTickType xBlockTime)
{
	unsigned char errorCnt = 0;
    unsigned short readSize = 0;
        
    //清空队列数据
    while(pdPASS == ReceiveDataFromQueue(GetUsart4RXQueue(), NULL, buf, buflen, NULL, 1, 10 / portTICK_RATE_MS, 1 / portTICK_RATE_MS));
        
	for(errorCnt = 0; errorCnt < 3; errorCnt++)
	{

		if(pdPASS == SendDataToQueue(GetUsart4TXQueue(), NULL, cmd, strlen(cmd), 1, 500 * portTICK_RATE_MS, 0, EnableUsart4TXInterrupt))
		{
			if(buf)
			{
				memset(buf, 0, buflen);
					
				ReceiveDataFromQueue(GetUsart4RXQueue(), NULL, buf, buflen, &readSize, 1, xBlockTime, 0);
                
                if(readSize > 0)
                {
                    if(strcmp)
                    {
                        if(MyStrStr(buf, strcmp, readSize))
                            return My_Pass;
                    }
                    else
                        return My_Pass;
                }
				
			}
		}

	}

	return My_Fail;
}


MyState_TypeDef GPRSInit(void)
{
    char *txbuf = NULL;
    unsigned short buflen = 500;
    MyState_TypeDef status = My_Fail;
    
    txbuf = MyMalloc(buflen);
    if(txbuf == NULL)
        goto END;
    
	if(My_Fail == ComWithWIFI("+++", "a", txbuf, buflen, FREERTOS_DELAY_500_MS))
    {
        if(strstr(txbuf, "ERROR:58") == NULL)
            goto END;
    }
    else
    {
        if(My_Fail == ComWithWIFI("a", "+ok", txbuf, buflen, FREERTOS_DELAY_2_S))
            goto END;
    }

   // ComWithWIFI("AT+CLEAR\r", "USR-GM3 V3.2.0", txbuf, buflen, FREERTOS_DELAY_20_S);
    //ComWithWIFI("AT+SOCKASL\r", NULL, txbuf, buflen, FREERTOS_DELAY_2_S);
    if(My_Fail == ComWithWIFI("AT+WKMOD=\"NET\"\r", "OK", txbuf, buflen, FREERTOS_DELAY_2_S))
        goto END;
    if(My_Fail == ComWithWIFI("AT+SOCKAEN=\"on\"\r", "OK", txbuf, buflen, FREERTOS_DELAY_2_S))
        goto END;
    if(My_Fail == ComWithWIFI("AT+SOCKA=\"TCP\",\"116.62.108.201\",8080\r", "OK", txbuf, buflen, FREERTOS_DELAY_2_S))
        goto END;
    if(My_Fail == ComWithWIFI("AT+S\r", "USR-GM3 V3.2.0", txbuf, buflen, FREERTOS_DELAY_20_S))
        goto END;

    vTaskDelay(5000 / portTICK_RATE_MS);
    status = My_Pass;
    
    END:
        MyFree(txbuf);
        return status;
	
}

#endif  //DEVICE_GPRS
