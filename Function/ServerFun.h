#ifndef	_SERVER_F_H_H
#define _SERVER_F_H_H

#include	"Define.h"
#include	"tcp.h"
#include 	"api.h" 

#define	SERVERSENDBUFLEN	1024
#define	SERVERRECVBUFLEN	1024

typedef struct
{
	void *myp;									//��ʱָ��
	char sendBuf[SERVERSENDBUFLEN];						//�������ݻ�����
	unsigned short sendDataLen;					//�������ݳ���
	char recvBuf[SERVERRECVBUFLEN];						//�������ݻ�����
	unsigned int recvDataLen;					//�������ݳ���
	
	struct ip_addr server_ipaddr;				//������ip
	struct netconn *clientconn;				//��ǰ�ͻ���
	struct netbuf *recvbuf;						//���ջ�����
	struct pbuf *q;								//�������ݵ�����
}MyServerData;

void CommunicateWithServerByLineNet(MyServerData * myServerData);
void CommunicateWithServerByWifi(MyServerData * myServerData);

MyState_TypeDef UpLoadData(char *URL, void * sendBuf, unsigned short sendLen, void * recvBuf, unsigned short recvLen,
	char * sendType);

#endif
