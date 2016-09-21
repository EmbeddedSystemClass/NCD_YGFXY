#ifndef	_SERVER_F_H_H
#define _SERVER_F_H_H

#include	"Define.h"
#include	"tcp.h"
#include 	"api.h" 


typedef struct
{
	struct ip_addr server_ipaddr;				//������ip
	struct netconn *clientconn;				//��ǰ�ͻ���
	struct netbuf *recvbuf;						//���ջ�����
	struct pbuf *q;								//�������ݵ�����
	unsigned char *myp;							//��ʱָ��
	mynetbuf * s_mybuf;							//��ʱ�������ݰ�
}MyLwipData;


#pragma pack(1)
typedef struct
{
	unsigned char *myp;							//��ʱָ��
	mynetbuf * s_mybuf;							//��ʱ�������ݰ�
	unsigned short rxcount;						//�������ݳ���
}MyWifiData;
#pragma pack()

MyState_TypeDef CommunicateWithServerByLineNet(void *sendnetbuf, void *recvnetbuf, unsigned char ip1, unsigned char ip2, 
	unsigned char ip3, unsigned char ip4, unsigned short maxrecvlen);
MyState_TypeDef CommunicateWithServerByWifi(void *sendnetbuf, void *recvnetbuf, unsigned short maxrecvlen);

MyState_TypeDef UpLoadData(char *URL, void * buf, unsigned short buflen);

#endif
