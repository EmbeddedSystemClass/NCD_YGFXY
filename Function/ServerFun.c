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
#include 	"usbd_cdc_vcp.h"

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
	myServerData->clientconn->recv_timeout = 500;
		
		//��������
	err = netconn_write(myServerData->clientconn, myServerData->sendBuf, myServerData->sendDataLen, NETCONN_COPY );
	//����ʧ��
	if(err != ERR_OK)
		goto END1;
		
	//��������
	while(ERR_OK == netconn_recv(myServerData->clientconn, &myServerData->recvbuf))
	{
		myServerData->recvDataLen += netbuf_copy(myServerData->recvbuf, myServerData->recvBuf + myServerData->recvDataLen ,
			SERVERRECVBUFLEN - myServerData->recvDataLen);
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
	//��������
	SendDataToQueue(GetUsart4TXQueue(), GetUsart4TXMutex(), myServerData->sendBuf, myServerData->sendDataLen,
		1, 1000 / portTICK_RATE_MS, EnableUsart4TXInterrupt);
		
	//��������
	while(pdPASS == ReceiveDataFromQueue(GetUsart4RXQueue(), GetUsart4RXMutex(), myServerData->recvBuf, 500, 1, 1000 / portTICK_RATE_MS))
		;
}

/***************************************************************************************************
*FunctionName��
*Description��
*Input��None
*Output��None
*Author��xsx
*Data��
***************************************************************************************************/
MyState_TypeDef UpLoadData(char *URL, void * sendBuf, unsigned short sendLen, void * recvBuf, unsigned short recvLen)
{
	MyState_TypeDef statues = My_Fail;
	MyServerData * myServerData = NULL;
	
	myServerData = MyMalloc(sizeof(MyServerData));

	if(myServerData)
	{
		memset(myServerData, 0, sizeof(MyServerData));
		sprintf(myServerData->sendBuf, "POST %s HTTP/1.1\nHost: 116.62.108.201:8080\nConnection: keep-alive\nContent-Length: %d\nContent-Type:application/x-www-form-urlencoded;charset=GBK\nAccept-Language: zh-CN,zh;q=0.8\n\n%s", URL, sendLen, (char *)sendBuf);
		myServerData->sendDataLen = strlen(myServerData->sendBuf);
			
/*		CommunicateWithServerByLineNet(myServerData);
		{
			myServerData->myp = strstr(myServerData->recvBuf, "success");
			if(myServerData->myp)
			{
				memset(recvBuf, 0, recvLen);
				memcpy(recvBuf, myServerData->myp, recvLen);
				statues = My_Pass;
				goto END1;
			}
		}*/
		
		memset(myServerData->recvBuf, 0, SERVERRECVBUFLEN);
		CommunicateWithServerByWifi(myServerData);
		{
			myServerData->myp = strstr(myServerData->recvBuf, "success");
			if(myServerData->myp)
			{
				memset(recvBuf, 0, recvLen);
				memcpy(recvBuf, myServerData->myp, recvLen);
				statues = My_Pass;
			}
		}
		
		END1:
			;
	}
	
	MyFree(myServerData);
	
	return statues;
}

