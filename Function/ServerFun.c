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
#include	"StringDefine.h"

#include 	"usbd_hid_core.h"
#include 	"usbd_usr.h"
#include 	"usbd_desc.h"
#include 	"usb_conf.h" 

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
extern USB_OTG_CORE_HANDLE USB_OTG_dev;
extern uint8_t UsbHidReceiveComplete ;
/***************************************************************************************************/
/**************************************局部函数声明*************************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************正文********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

MyState_TypeDef CommunicateWithServerByLineNet(HttpBuffer * httpBuffer)
{
	err_t err;
	
	httpBuffer->status = My_Fail;
	
	IP4_ADDR(&httpBuffer->server_ipaddr, httpBuffer->serverSet.serverIP.ip_1, httpBuffer->serverSet.serverIP.ip_2, 
		httpBuffer->serverSet.serverIP.ip_3, httpBuffer->serverSet.serverIP.ip_4);

	//创建连接
	httpBuffer->clientconn = netconn_new(NETCONN_TCP);
	//创建失败
	if(httpBuffer->clientconn == NULL)
		return httpBuffer->status;

	//绑定本地ip
	err = netconn_bind(httpBuffer->clientconn, IP_ADDR_ANY, 0);
	//连接失败
	if(err != ERR_OK)
		goto END2;

	//尝试连接远程服务器
	err = netconn_connect(httpBuffer->clientconn, &httpBuffer->server_ipaddr, httpBuffer->serverSet.serverPort);
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
	memset(httpBuffer->recvBuf, 0, HttpRecvBufLen);
	while(ERR_OK == netconn_recv(httpBuffer->clientconn, &httpBuffer->tcpRecvBuf))
	{
		httpBuffer->recvDataLen += netbuf_copy(httpBuffer->tcpRecvBuf, httpBuffer->recvBuf + httpBuffer->recvDataLen ,
			HttpRecvBufLen - httpBuffer->recvDataLen);
		
		netbuf_delete(httpBuffer->tcpRecvBuf);
	}
	
	httpBuffer->tempP = strstr(httpBuffer->recvBuf, SuccessStr);
	if(httpBuffer->tempP)
		httpBuffer->status = My_Pass;
			
	END1:
		netconn_close(httpBuffer->clientconn);
		netconn_delete(httpBuffer->clientconn);
		return httpBuffer->status;
		
	END2:
		netconn_delete(httpBuffer->clientconn);
		return httpBuffer->status;
}

MyState_TypeDef CommunicateWithServerByWifi(HttpBuffer * httpBuffer)
{
	if(My_Pass == takeWifiMutex(1000 / portTICK_RATE_MS))
	{
		xQueueReset(GetUsart4RXQueue());
		memset(httpBuffer->recvBuf, 0, HttpRecvBufLen);
		//发送数据
		httpBuffer->tempInt1 = httpBuffer->sendDataLen;
		while(httpBuffer->tempInt1 > 800)
		{
			if(My_Pass == SendDataToQueue(GetUsart4TXQueue(), NULL, httpBuffer->sendBuf + (httpBuffer->sendDataLen - httpBuffer->tempInt1), 
				800, 1, 1000 / portTICK_RATE_MS, 10 / portTICK_RATE_MS, EnableUsart4TXInterrupt))
			{
				httpBuffer->tempInt1 -= 800;
				vTaskDelay(100 / portTICK_RATE_MS);
			}
			else
				goto END;
		}
		
		if(httpBuffer->tempInt1 > 0)
		{
			if(My_Pass == SendDataToQueue(GetUsart4TXQueue(), NULL, httpBuffer->sendBuf + (httpBuffer->sendDataLen - httpBuffer->tempInt1), 
				httpBuffer->tempInt1, 1, 1000 / portTICK_RATE_MS, 10 / portTICK_RATE_MS, EnableUsart4TXInterrupt))
			{
				;
			}
			else
				goto END;
		}
		
		//接收数据,最好等待1s
		httpBuffer->recvDataLen = 0;
		httpBuffer->i = 0;
		while(pdPASS == ReceiveDataFromQueue(GetUsart4RXQueue(), NULL, httpBuffer->recvBuf, 1000, 
			&httpBuffer->readSize, 1, 1000 / portTICK_RATE_MS, 1000 / portTICK_RATE_MS))
		{
			if(httpBuffer->isPost)
			{
				httpBuffer->recvDataLen += httpBuffer->readSize;
				break;
			}
			//如果发生的是GET请求，则说明是下载固件，需要保存
			else
			{
				if(httpBuffer->i == 0)
				{
					//查找文件头
					httpBuffer->tempP = strstr(httpBuffer->recvBuf, fileStartStr);
					if(httpBuffer->tempP)
					{
						httpBuffer->httpHeadLen = httpBuffer->tempP - httpBuffer->recvBuf + strlen(fileStartStr);
						httpBuffer->readSize -= httpBuffer->httpHeadLen;
						WriteAppFile(httpBuffer->recvBuf + httpBuffer->httpHeadLen, httpBuffer->readSize, TRUE);
						httpBuffer->recvDataLen += httpBuffer->readSize;
						httpBuffer->i++;
					}
				}
				else
				{
					WriteAppFile(httpBuffer->recvBuf, httpBuffer->readSize, FALSE);
					httpBuffer->recvDataLen += httpBuffer->readSize;
				}
			}
		}
		
		END:
			giveWifixMutex();
			if(httpBuffer->isPost)
			{
				httpBuffer->tempP = strstr(httpBuffer->recvBuf, SuccessStr);
				if(httpBuffer->tempP)
					return My_Pass;
			}
			else if(My_Pass == checkNewFirmwareIsSuccessDownload())
			{
				setIsSuccessDownloadFirmware(TRUE);
				return My_Pass;
			}
	}
	
	return My_Fail;
}

MyState_TypeDef CommunicateWithServerByUSB(HttpBuffer * httpBuffer)
{
	xQueueReset(GetUSBRXQueue());
	memset(httpBuffer->recvBuf, 0, HttpRecvBufLen);
	
	//start send data
	httpBuffer->tempP = httpBuffer->sendBuf;

	while(httpBuffer->sendDataLen > 0)
	{
		if(httpBuffer->sendDataLen > 64)
			httpBuffer->readSize = 64;
		else
			httpBuffer->readSize = httpBuffer->sendDataLen;
			
		USBD_HID_SendReport(&USB_OTG_dev, httpBuffer->tempP, 64);
		httpBuffer->sendDataLen -= httpBuffer->readSize;
		httpBuffer->tempP += httpBuffer->readSize;
	}
	
	//data send end ,wait host result
	ReceiveDataFromQueue(GetUSBRXQueue(), NULL, httpBuffer->recvBuf, 64, &httpBuffer->readSize, 1, 1000 / portTICK_RATE_MS, 1000 / portTICK_RATE_MS);
		
	httpBuffer->tempP = strstr(httpBuffer->recvBuf, SuccessStr);
	if(httpBuffer->tempP)
		return My_Pass;
	else
		return My_Fail;
}
