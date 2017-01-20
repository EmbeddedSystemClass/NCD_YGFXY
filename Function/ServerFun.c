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

MyState_TypeDef CommunicateWithServerByLineNet(void *sendnetbuf, void *recvnetbuf, unsigned char ip1, unsigned char ip2, 
	unsigned char ip3, unsigned char ip4, unsigned short maxrecvlen)
{
	err_t err;
	
	MyState_TypeDef status = My_Fail;

	
	MyLwipData * myclientdata = NULL;
	
	myclientdata = MyMalloc(sizeof(MyLwipData));
	
	if(myclientdata)
	{
		IP4_ADDR(&myclientdata->server_ipaddr,ip1,ip2,ip3,ip4);

		//��������
		myclientdata->clientconn = netconn_new(NETCONN_TCP);
		//����ʧ��
		if(myclientdata->clientconn == NULL)
			goto END;

		//�󶨱���ip
		err = netconn_bind(myclientdata->clientconn, IP_ADDR_ANY, 0);
		//����ʧ��
		if(err != ERR_OK)
			goto END2;

		//��������Զ�̷�����
		err = netconn_connect(myclientdata->clientconn, &myclientdata->server_ipaddr, 8080);
		//����ʧ��
		if(err != ERR_OK)
			goto END2;
		
		//���ý������ݳ�ʱʱ��100MS
		myclientdata->clientconn->recv_timeout = 500;
		
		//��������
		if(sendnetbuf)
		{
			myclientdata->s_mybuf = sendnetbuf;
			err = netconn_write(myclientdata->clientconn, myclientdata->s_mybuf->data, myclientdata->s_mybuf->datalen, NETCONN_COPY );
			//����ʧ��
			if(err != ERR_OK)
				goto END1;
		}
		
		//��������
		if(recvnetbuf)
		{
			myclientdata->s_mybuf = recvnetbuf;
			myclientdata->s_mybuf->datalen = 0;
			myclientdata->myp = myclientdata->s_mybuf->data;
			
			while(ERR_OK == netconn_recv(myclientdata->clientconn, &myclientdata->recvbuf))
			{
				myclientdata->s_mybuf->datalen += netbuf_copy(myclientdata->recvbuf, myclientdata->myp + myclientdata->s_mybuf->datalen ,
					maxrecvlen - myclientdata->s_mybuf->datalen);
				netbuf_delete(myclientdata->recvbuf);
				status = My_Pass;
			}
		}
		
		END1:
			netconn_close(myclientdata->clientconn);
		
		END2:
			netconn_delete(myclientdata->clientconn);
	}
	
	END:
		MyFree(myclientdata);
		return status;
}

MyState_TypeDef CommunicateWithServerByWifi(void *sendnetbuf, void *recvnetbuf, unsigned short maxrecvlen)
{
	MyWifiData *mywifidata;
	MyState_TypeDef status = My_Fail;
	
	mywifidata = MyMalloc(sizeof(MyWifiData));
	if(mywifidata)
	{
		
		//��մ�������
//		while(pdPASS == ReceiveDataFromQueue(GetUsart4RXQueue(), GetUsart4RXMutex(), mywifidata->s_mybuf->data, 500, 1, 10 / portTICK_RATE_MS))
	//		;
		
		//��������
		if(sendnetbuf)
		{
			mywifidata->s_mybuf = sendnetbuf;
			
			SendDataToQueue(GetUsart4TXQueue(), GetUsart4TXMutex(), mywifidata->s_mybuf->data, mywifidata->s_mybuf->datalen, 1, 1000 / portTICK_RATE_MS, EnableUsart4TXInterrupt);
		}
		
		//��������
		if(recvnetbuf)
		{
			mywifidata->rxcount = 0;
			mywifidata->s_mybuf = recvnetbuf;
			mywifidata->myp = mywifidata->s_mybuf->data;
			//memset(mywifidata->myp, 0, 100);
			while(pdPASS == ReceiveDataFromQueue(GetUsart4RXQueue(), GetUsart4RXMutex(), mywifidata->s_mybuf->data, 500, 1, 1000 / portTICK_RATE_MS))
				;

			//if(mywifidata->rxcount > 0)
				status = My_Pass;
		}
	}
	
	MyFree(mywifidata);
	return status;
}

/***************************************************************************************************
*FunctionName��
*Description��
*Input��None
*Output��None
*Author��xsx
*Data��
***************************************************************************************************/
MyState_TypeDef UpLoadData(char *URL, void * buf, unsigned short buflen)
{
	MyState_TypeDef statues = My_Fail;
	mynetbuf mybuf;
	mynetbuf recvbuf;
	char * temp;
	
	mybuf.data = MyMalloc(buflen+512);
	recvbuf.data = MyMalloc(1024);
	if(mybuf.data && recvbuf.data)
	{
		memset(mybuf.data, 0, buflen+512);
		sprintf(mybuf.data, "POST %s HTTP/1.1\nHost: 116.62.108.201:8080\nConnection: keep-alive\nContent-Length: %d\nContent-Type:application/x-www-form-urlencoded;charset=GBK\nAccept-Language: zh-CN,zh;q=0.8\n\n%s", URL, buflen, (char *)buf);
		mybuf.datalen = strlen(mybuf.data);
			
/*		memset(recvbuf.data, 0, 1024);
		if(My_Pass == CommunicateWithServerByLineNet(&mybuf, &recvbuf, 116, 62, 108, 201, 1000))
		{
			temp = strstr(recvbuf.data, "success");
			if(temp)
			{
				//USB_PutStr("\nlwip_mode ok\n", 13);
				memset(buf, 0, buflen);
				memcpy(buf, temp, strlen(temp));
				statues = My_Pass;
				goto END1;
			}
			else
				;//USB_PutStr("\nlwip_mode error\n", 16);
		}*/
		
		memset(recvbuf.data, 0, 1024);
		if(My_Pass == CommunicateWithServerByWifi(&mybuf, &recvbuf, 1000))
		{
			temp = strstr(recvbuf.data, "success");
			if(temp)
			{
				//USB_PutStr("wifi_mode ok\n", 13);
				memset(buf, 0, buflen);
				memcpy(buf, temp, strlen(temp));
				statues = My_Pass;
				goto END1;
			}
			else
				;//USB_PutStr("wifi_mode error\n", 16);
		}
		
		END1:
			;
	}
	MyFree(mybuf.data);
	MyFree(recvbuf.data);
	
	return statues;
}

