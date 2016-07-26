/***************************************************************************************************
*FileName��TcpClient_Task
*Description��LWIP��tcp�ͻ�������
*Author��xsx
*Data��2016��4��21��17:13:42
***************************************************************************************************/

/***************************************************************************************************/
/******************************************ͷ�ļ�***************************************************/
/***************************************************************************************************/

#include	"LwipNCDClient_Task.h"

#include	"QueueUnits.h"
#include	"Net_Data.h"
#include	"MyMem.h"
#include	"SelfCheck_Data.h"

#include 	"FreeRTOS.h"
#include 	"task.h"

#include	"tcp.h"
#include 	"api.h" 

#include	<string.h>
/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/

#define LwipNCDClientTask_PRIORITY			2						//�ͻ����������ȼ�
const char * LwipNCDClientTaskName = "vLwipNCDClientTask";				//�ͻ���������

static struct netconn *tcp_clientconn;
static struct ip_addr server_ipaddr;


/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/

static void vLwipNCDClientTask( void *pvParameters );	//�ͻ����Զ���������
static void ClientTXHandle(struct netconn *pxNetCon);
static void ClientRXHandle(struct netconn *pxNetCon);
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************����********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************
*FunctionName��StartvTcpClientTask
*Description������tcp �ͻ�������
*Input��None
*Output��None
*Author��xsx
*Data��2016��4��21��17:15:52
***************************************************************************************************/
void StartvLwipNCDClientTask(void)
{
	xTaskCreate( vLwipNCDClientTask, LwipNCDClientTaskName, configMINIMAL_STACK_SIZE, NULL, LwipNCDClientTask_PRIORITY, NULL );
}

/***************************************************************************************************
*FunctionName��vTcpClientTask
*Description��tcp �ͻ�������
*Input��None
*Output��None
*Author��xsx
*Data��2016��4��21��17:15:39
***************************************************************************************************/
static void vLwipNCDClientTask( void *pvParameters )
{
	err_t err;
	
	while(1)
	{
		IP4_ADDR(&server_ipaddr,123,57,94,39);
		
		while(1)
		{
			if(Line_Mode == GetGB_NetCard())
			{
				tcp_clientconn = netconn_new(NETCONN_TCP);
				netconn_bind(tcp_clientconn , IP_ADDR_ANY , 9601);
				err = netconn_connect(tcp_clientconn,&server_ipaddr,80);
				tcp_clientconn->recv_timeout = 100;											//���ճ�ʱʱ��10ms
					
				if(err != ERR_OK)  
				{
					netconn_close( tcp_clientconn );
					netconn_delete(tcp_clientconn);
				}
				else if (err == ERR_OK)
				{
					SetGB_NCDServerLinkState(Link_Up);
					while(ESTABLISHED == tcp_clientconn->pcb.tcp->state)									//�������Ӻ��������
					{
						ClientTXHandle(tcp_clientconn);
						ClientRXHandle(tcp_clientconn);
					}
						
					SetGB_NCDServerLinkState(Link_Down);
					netconn_close( tcp_clientconn );
					netconn_delete(tcp_clientconn);
				}
			}
				
			vTaskDelay(10 / portTICK_RATE_MS);
		}
	}
}

/***************************************************************************************************
*FunctionName��ClientTXHandle
*Description���ͻ��˷������ݴ���
*Input��None
*Output��None
*Author��xsx
*Data��2016��4��22��16:17:11
***************************************************************************************************/
static void ClientTXHandle(struct netconn *pxNetCon)
{
	mynetbuf sendbuf;

	if(pdPASS == ReceiveDataFromQueue(GetGBNCDClientTXQueue(), NULL, &sendbuf, 1, 10*portTICK_RATE_MS))
	{
		netconn_write( pxNetCon, sendbuf.data, sendbuf.datalen, NETCONN_COPY );
		
		MyFree(sendbuf.data);
	}
}

/***************************************************************************************************
*FunctionName��ClientRXHandle
*Description���ͻ��˽������ݴ���
*Input��None
*Output��None
*Author��xsx
*Data��2016��4��22��16:17:28
***************************************************************************************************/
static void ClientRXHandle(struct netconn *pxNetCon)
{
	err_t err;
	struct netbuf *recvbuf;
	struct pbuf *q;
	unsigned char *myp = NULL;
	
	static mynetbuf myrecvbuf;

	err = netconn_recv(pxNetCon,&recvbuf);
	if(err == ERR_OK)
	{
		q=recvbuf->p;
		myrecvbuf.data = MyMalloc(q->tot_len);
		myrecvbuf.datalen = q->tot_len;
		
		if(myrecvbuf.data)
		{
			myp = myrecvbuf.data;
			
			for(; q!=NULL; q=q->next)
			{
				memcpy(myp, q->payload, q->len);
				myp += q->len;
			}
			
			/*����*/
			if(pdPASS != SendDataToQueue(GetGBNCDClientRXQueue(), NULL, &myrecvbuf, 1, 10 / portTICK_RATE_MS, NULL))
			//if(pdPASS != SendDataToQueue(GetGBUserClientTXQueue(), GetGBUserClientTXMutex(), &myrecvbuf, 1, 10*portTICK_RATE_MS, NULL))
				MyFree(myrecvbuf.data);
			//SendDataToQueue(GetGBNCDClientTXQueue(), GetGBNCDClientTXMutex(), &myrecvbuf, 1, 10*portTICK_RATE_MS, NULL)
		}
		
		netbuf_delete(recvbuf);
	}
}

