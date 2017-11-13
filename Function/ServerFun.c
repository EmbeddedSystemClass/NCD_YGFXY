/***************************************************************************************************
*FileName:
*Description:
*Author:
*Data:
***************************************************************************************************/

/***************************************************************************************************/
/******************************************头文件***************************************************/
/***************************************************************************************************/
#include	"ServerFun.h"
#include	"QueueUnits.h"
#include	"System_Data.h"
#include 	"Usart4_Driver.h"
#include	"SystemSet_Data.h"
#include	"NetInfo_Data.h"
#include	"AppFileDao.h"
#include	"IAP_Fun.h"
#include	"RemoteSoft_Data.h"
#include	"WifiFunction.h"
#include	"MyMem.h"

#include	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"
#include	"semphr.h"

#include	<string.h>
#include	"stdio.h"
#include 	"stdlib.h"
/***************************************************************************************************/
/**************************************局部变量声明*************************************************/
/***************************************************************************************************/
const char * fileStartStr = "i am zhangxiong^*^!";
/***************************************************************************************************/
/**************************************局部函数声明*************************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************正文********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

void CommunicateWithServerByLineNet(HttpBuffer * httpBuffer)
{
	err_t err;
	struct pbuf *p = NULL;
	unsigned char i=0;
	char * tempPoint = NULL;
	unsigned short tempValue = 0;
	
	SetGB_LineNetStatus(0);
	
	IP4_ADDR(&httpBuffer->server_ipaddr,GB_ServerIp_1, GB_ServerIp_2, GB_ServerIp_3, GB_ServerIp_4);
	//IP4_ADDR(&httpBuffer->server_ipaddr, 192,168,0,28);

	//创建连接
	httpBuffer->clientconn = netconn_new(NETCONN_TCP);
	//创建失败
	if(httpBuffer->clientconn == NULL)
		return;

	//绑定本地ip
	err = netconn_bind(httpBuffer->clientconn, IP_ADDR_ANY, 0);
	//连接失败
	if(err != ERR_OK)
		goto END2;

	//尝试连接远程服务器
	err = netconn_connect(httpBuffer->clientconn, &httpBuffer->server_ipaddr, GB_ServerPort);
	//连接失败
	if(err != ERR_OK)
		goto END2;
		
	//设置接收数据超时时间100MS
	httpBuffer->clientconn->recv_timeout = 3000;
		
		//发送数据
	err = netconn_write(httpBuffer->clientconn, httpBuffer->sendBuf, httpBuffer->sendDataLen, NETCONN_COPY );
	//发送失败
	if(err != ERR_OK)
		goto END1;
		
	//接收数据
	httpBuffer->recvDataLen = 0;
	while(ERR_OK == netconn_recv(httpBuffer->clientconn, &httpBuffer->tcpRecvBuf))
	{
		SetGB_LineNetStatus(1);
		//如果发生的是GET请求，则说明是下载固件，需要保存
		if(!httpBuffer->isPost)
		{
			p = httpBuffer->tcpRecvBuf->p;
			while(p)
			{
				if(i == 0)
				{
					//查找文件头
					tempPoint = strstr(p->payload, fileStartStr);
					if(tempPoint)
					{
						tempValue = tempPoint - ((char *)p->payload) + strlen(fileStartStr);
						WriteAppFile((char *)(p->payload) + tempValue, p->len-tempValue, true);
						i++;
					}
				}
				else
				{
					WriteAppFile(p->payload, p->len, false);
				}
				p = p->next;
			}
			
			vTaskDelay(10 / portTICK_RATE_MS);
		}
		else
		{
			httpBuffer->recvDataLen += netbuf_copy(httpBuffer->tcpRecvBuf, httpBuffer->recvBuf + httpBuffer->recvDataLen ,
				HttpRecvBufLen - httpBuffer->recvDataLen);
		}
		
		netbuf_delete(httpBuffer->tcpRecvBuf);
	}
		
	END1:
		netconn_close(httpBuffer->clientconn);
		netconn_delete(httpBuffer->clientconn);
		return;
		
	END2:
		netconn_delete(httpBuffer->clientconn);
		return;
}

void CommunicateWithServerByWifi(HttpBuffer * httpBuffer)
{
	unsigned short i = 0;
	unsigned short readSize = 0;
	unsigned short tempValue = 0;
	char * tempPoint = NULL;
	
	if(My_Pass == takeWifiMutex(1000 / portTICK_RATE_MS))
	{
		xQueueReset(GetUsart4RXQueue());
		
		//发送数据
		readSize = httpBuffer->sendDataLen;
		while(readSize > 800)
		{
			if(My_Pass == SendDataToQueue(GetUsart4TXQueue(), NULL, httpBuffer->sendBuf + (httpBuffer->sendDataLen - readSize), 
				800, 1, 1000 / portTICK_RATE_MS, 10 / portTICK_RATE_MS, EnableUsart4TXInterrupt))
			{
				readSize -= 800;
				vTaskDelay(100 / portTICK_RATE_MS);
			}
			else
				goto END;
		}
		
		if(readSize > 0)
		{
			if(My_Pass == SendDataToQueue(GetUsart4TXQueue(), NULL, httpBuffer->sendBuf + (httpBuffer->sendDataLen - readSize), 
				readSize, 1, 1000 / portTICK_RATE_MS, 10 / portTICK_RATE_MS, EnableUsart4TXInterrupt))
			{
				;
			}
			else
				goto END;
		}
		
		//接收数据,最好等待1s
		httpBuffer->recvDataLen = 0;
		while(pdPASS == ReceiveDataFromQueue(GetUsart4RXQueue(), NULL, httpBuffer->recvBuf, 1000, 
			&readSize, 1, 1000 / portTICK_RATE_MS, 1000 / portTICK_RATE_MS))
		{
			//如果发生的是GET请求，则说明是下载固件，需要保存
			if(!httpBuffer->isPost)
			{
				if(i == 0)
				{
					//查找文件头
					tempPoint = strstr(httpBuffer->recvBuf, fileStartStr);
					if(tempPoint)
					{
						tempValue = tempPoint - httpBuffer->recvBuf + strlen(fileStartStr);
						readSize -= tempValue;
						WriteAppFile(httpBuffer->recvBuf + tempValue, readSize, true);
						i++;
					}
				}
				else
				{
					WriteAppFile(httpBuffer->recvBuf, readSize, false);
				}
				
				
			}
			
			httpBuffer->recvDataLen += readSize;
		}
		
		END:
			giveWifixMutex();
	}
}

/***************************************************************************************************
*FunctionName：
*Description：
*Input：None
*Output：None
*Author：xsx
*Data：
***************************************************************************************************/
MyState_TypeDef UpLoadData(HttpBuffer * httpBuffer)
{
/*	memset(httpBuffer->recvBuf, 0, HttpRecvBufLen);
	//CommunicateWithServerByLineNet(httpBuffer);
	if(httpBuffer->isPost)
	{
		httpBuffer->tempP = strstr(httpBuffer->recvBuf, "success");
		if(httpBuffer->tempP)
			return My_Pass;
	}
	else if(My_Pass == checkNewFirmwareIsSuccessDownload())
	{
		setIsSuccessDownloadFirmware(true);
		return My_Pass;
	}
	*/	
	memset(httpBuffer->recvBuf, 0, HttpRecvBufLen);
	CommunicateWithServerByWifi(httpBuffer);
	if(httpBuffer->isPost)
	{
		httpBuffer->tempP = strstr(httpBuffer->recvBuf, "success");
		if(httpBuffer->tempP)
			return My_Pass;
	}
	else if(My_Pass == checkNewFirmwareIsSuccessDownload())
	{
		setIsSuccessDownloadFirmware(true);
		return My_Pass;
	}
		
	return My_Fail;
}

