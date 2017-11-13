/***************************************************************************************************
*FileName:
*Description:
*Author:
*Data:
***************************************************************************************************/

/***************************************************************************************************/
/******************************************ͷ�ļ�***************************************************/
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
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/
const char * fileStartStr = "i am zhangxiong^*^!";
/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************����********************************************************/
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

	//��������
	httpBuffer->clientconn = netconn_new(NETCONN_TCP);
	//����ʧ��
	if(httpBuffer->clientconn == NULL)
		return;

	//�󶨱���ip
	err = netconn_bind(httpBuffer->clientconn, IP_ADDR_ANY, 0);
	//����ʧ��
	if(err != ERR_OK)
		goto END2;

	//��������Զ�̷�����
	err = netconn_connect(httpBuffer->clientconn, &httpBuffer->server_ipaddr, GB_ServerPort);
	//����ʧ��
	if(err != ERR_OK)
		goto END2;
		
	//���ý������ݳ�ʱʱ��100MS
	httpBuffer->clientconn->recv_timeout = 3000;
		
		//��������
	err = netconn_write(httpBuffer->clientconn, httpBuffer->sendBuf, httpBuffer->sendDataLen, NETCONN_COPY );
	//����ʧ��
	if(err != ERR_OK)
		goto END1;
		
	//��������
	httpBuffer->recvDataLen = 0;
	while(ERR_OK == netconn_recv(httpBuffer->clientconn, &httpBuffer->tcpRecvBuf))
	{
		SetGB_LineNetStatus(1);
		//�����������GET������˵�������ع̼�����Ҫ����
		if(!httpBuffer->isPost)
		{
			p = httpBuffer->tcpRecvBuf->p;
			while(p)
			{
				if(i == 0)
				{
					//�����ļ�ͷ
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
		
		//��������
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
		
		//��������,��õȴ�1s
		httpBuffer->recvDataLen = 0;
		while(pdPASS == ReceiveDataFromQueue(GetUsart4RXQueue(), NULL, httpBuffer->recvBuf, 1000, 
			&readSize, 1, 1000 / portTICK_RATE_MS, 1000 / portTICK_RATE_MS))
		{
			//�����������GET������˵�������ع̼�����Ҫ����
			if(!httpBuffer->isPost)
			{
				if(i == 0)
				{
					//�����ļ�ͷ
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
*FunctionName��
*Description��
*Input��None
*Output��None
*Author��xsx
*Data��
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

