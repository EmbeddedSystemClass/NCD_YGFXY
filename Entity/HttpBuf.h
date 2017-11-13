/****************************************file start****************************************************/
#ifndef _HTTPBUF_E_H
#define	_HTTPBUF_E_H

#include	"Define.h"
#include	"RemoteSoft_Data.h"
#include	"SystemSet_Data.h"
#include	"TestDataDao.h"

#define	HttpSendBufLen	4096
#define	HttpRecvBufLen	1024

typedef struct HttpBuffer_Tag
{
	char deviceId[MaxDeviceIDLen];
	char sendBuf[HttpSendBufLen];
	unsigned int sendDataLen;					//�������ݳ���
	char recvBuf[HttpRecvBufLen];
	unsigned int recvDataLen;					//�������ݳ���
	char tempBuf[500];
	bool isPost;
	unsigned short tempValue;
	DeviceInfo * device;
	SystemSetData * systemSetData;
	RemoteSoftInfo * remoteSoftInfo;
	PageRequest pageRequest;
	Page * page;
	TestData * testData;
	char * tempP;
	unsigned int tempInt1;
	unsigned int tempInt2;
	unsigned short i;
	unsigned short k;
	
	struct ip_addr server_ipaddr;				//������ip
	struct netconn *clientconn;				//��ǰ�ͻ���
	struct netbuf *tcpRecvBuf;						//���ջ�����
	struct pbuf *q;								//�������ݵ�����
}HttpBuffer;

#define	HttpBufferStructSize	sizeof(HttpBuffer)

#endif

/****************************************end of file************************************************/

