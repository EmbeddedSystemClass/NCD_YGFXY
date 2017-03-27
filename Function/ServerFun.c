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

/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************����********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

void CommunicateWithServerByLineNet(MyServerData * myServerData)
{
	err_t err;
	struct pbuf *p = NULL;
	unsigned char i=0;
	
	SetGB_LineNetStatus(0);
	
	IP4_ADDR(&myServerData->server_ipaddr,GB_ServerIp_1, GB_ServerIp_2, GB_ServerIp_3, GB_ServerIp_4);

	//��������
	myServerData->clientconn = netconn_new(NETCONN_TCP);
	//����ʧ��
	if(myServerData->clientconn == NULL)
		return;

	//�󶨱���ip
	err = netconn_bind(myServerData->clientconn, IP_ADDR_ANY, 0);
	//����ʧ��
	if(err != ERR_OK)
		goto END2;

	//��������Զ�̷�����
	err = netconn_connect(myServerData->clientconn, &myServerData->server_ipaddr, GB_ServerPort);
	//����ʧ��
	if(err != ERR_OK)
		goto END2;
		
	//���ý������ݳ�ʱʱ��100MS
	myServerData->clientconn->recv_timeout = 2000;
		
		//��������
	err = netconn_write(myServerData->clientconn, myServerData->sendBuf, myServerData->sendDataLen, NETCONN_COPY );
	//����ʧ��
	if(err != ERR_OK)
		goto END1;
		
	//��������
	while(ERR_OK == netconn_recv(myServerData->clientconn, &myServerData->recvbuf))
	{
		SetGB_LineNetStatus(1);
		//�����������GET������˵�������ع̼�����Ҫ����
		if(strstr(myServerData->sendBuf, "GET"))
		{
			p = myServerData->recvbuf->p;
			if(i == 0)
			{
				i++;
				WriteAppFile((unsigned char *)(p->payload)+229, p->len-229, true);
			}
			else
				WriteAppFile(p->payload, p->len, false);
			
			while(p->next)
			{
				p = p->next;
				WriteAppFile(p->payload, p->len, false);
			}
			
			vTaskDelay(10 / portTICK_RATE_MS);
		}
		else
		{
			myServerData->recvDataLen += netbuf_copy(myServerData->recvbuf, myServerData->recvBuf + myServerData->recvDataLen ,
				SERVERRECVBUFLEN - myServerData->recvDataLen);
		}
		
		netbuf_delete(myServerData->recvbuf);
	}
		
	END1:
		netconn_close(myServerData->clientconn);
		netconn_delete(myServerData->clientconn);
		return;
		
	END2:
		netconn_delete(myServerData->clientconn);
		return;
}

void CommunicateWithServerByWifi(MyServerData * myServerData)
{
	unsigned short i = 0;
	unsigned short readSize = 0;
	portTickType queueBlockTime;
	
	if(My_Pass == takeWifiMutex(1000 / portTICK_RATE_MS))
	{
		//��ն�������
		while(pdPASS == ReceiveDataFromQueue(GetUsart4RXQueue(), NULL, myServerData->recvBuf, 1000, 
				&readSize, 1, 10 / portTICK_RATE_MS, 1 / portTICK_RATE_MS));
		
		//��������
		if(My_Pass == SendDataToQueue(GetUsart4TXQueue(), NULL, myServerData->sendBuf, myServerData->sendDataLen,
			1, 1000 / portTICK_RATE_MS, 10 / portTICK_RATE_MS, EnableUsart4TXInterrupt))
		{
			
			//��������,��õȴ�1s
			
			while(pdPASS == ReceiveDataFromQueue(GetUsart4RXQueue(), NULL, myServerData->recvBuf, 1000, 
				&readSize, 1, 1000 / portTICK_RATE_MS, 1000 / portTICK_RATE_MS))
			{
				//�����������GET������˵�������ع̼�����Ҫ����
				if(strstr(myServerData->sendBuf, "GET"))
				{
					if(i == 0)
					{
						WriteAppFile(myServerData->recvBuf + 229, readSize-229, true);
						i++;
					}
					else
						WriteAppFile(myServerData->recvBuf, readSize, false);
					
					myServerData->recvDataLen += readSize;
				}
			}
		}
		
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
MyState_TypeDef UpLoadData(char *URL, void * sendBuf, unsigned short sendLen, void * recvBuf, unsigned short recvLen,
	char * sendType)
{
	MyState_TypeDef statues = My_Fail;
	MyServerData * myServerData = NULL;
	
	myServerData = MyMalloc(sizeof(MyServerData));

	if(myServerData)
	{
		memset(myServerData, 0, sizeof(MyServerData));
		if(strstr(sendType, "POST"))
			sprintf(myServerData->sendBuf, "POST %s HTTP/1.1\nHost: 116.62.108.201:8080\nConnection: keep-alive\nContent-Length: %d\nContent-Type:application/x-www-form-urlencoded;charset=GBK\nAccept-Language: zh-CN,zh;q=0.8\n\n%s", URL, sendLen, (char *)sendBuf);
		else
			sprintf(myServerData->sendBuf, "GET %s HTTP/1.1\nHost: 116.62.108.201:8080\nConnection: keep-alive\n\n", URL);
		
		myServerData->sendDataLen = strlen(myServerData->sendBuf);
			
		CommunicateWithServerByLineNet(myServerData);
		{
			if(strstr(sendType, "POST"))
			{
				myServerData->myp = strstr(myServerData->recvBuf, "success");
				if(myServerData->myp)
				{
					memset(recvBuf, 0, recvLen);
					memcpy(recvBuf, myServerData->myp, recvLen);

					statues = My_Pass;
					goto END1;
				}
			}
			else
			{
				if(My_Pass == checkNewFirmwareIsSuccessDownload())
				{
					statues = My_Pass;
					setIsSuccessDownloadFirmware(true);
					goto END1;
				}
			}
		}
		
		memset(myServerData->recvBuf, 0, SERVERRECVBUFLEN);
		CommunicateWithServerByWifi(myServerData);
		{
			if(strstr(sendType, "POST"))
			{
				myServerData->myp = strstr(myServerData->recvBuf, "success");
				if(myServerData->myp)
				{
					memset(recvBuf, 0, recvLen);
					memcpy(recvBuf, myServerData->myp, recvLen);
					statues = My_Pass;
				}
			}
			else
			{
				if(My_Pass == checkNewFirmwareIsSuccessDownload())
				{
					setIsSuccessDownloadFirmware(true);
					statues = My_Pass;
				}
			}
		}
		
		END1:
			;
	}
	
	MyFree(myServerData);
	
	return statues;
}

