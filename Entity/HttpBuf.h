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
	unsigned int sendDataLen;					//发送数据长度
	char recvBuf[HttpRecvBufLen];
	unsigned int recvDataLen;					//发送数据长度
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
	
	struct ip_addr server_ipaddr;				//服务器ip
	struct netconn *clientconn;				//当前客户端
	struct netbuf *tcpRecvBuf;						//接收缓冲区
	struct pbuf *q;								//接收数据的链表
}HttpBuffer;

#define	HttpBufferStructSize	sizeof(HttpBuffer)

#endif

/****************************************end of file************************************************/

