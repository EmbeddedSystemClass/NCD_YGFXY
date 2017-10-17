#ifndef	_UPLOAD_F_H_H
#define _UPLOAD_F_H_H

#include	"Define.h"
#include	"TestDataDao.h"
#include	"SystemSet_Data.h"
#include	"RemoteSoft_Data.h"

#define	UPLOADSENDBUFLEN	4096
#define	UPLOADRECVBUFLEN	1024
#define	UPLOADTEMPBUFLEN	2048

#pragma pack(1)
typedef struct UpLoadDeviceDataBuffer_Tag
{
	SystemSetData systemSetData;
	char sendBuf[UPLOADSENDBUFLEN];
	char recvBuf[UPLOADRECVBUFLEN];
	char tempBuf[UPLOADTEMPBUFLEN];
	RemoteSoftInfo remoteSoftInfo;
	unsigned short tempValue;
	unsigned char * tempP;
	unsigned short i;
	unsigned short j;
}UpLoadDeviceDataBuffer;
#pragma pack()

#pragma pack(1)
typedef struct UpLoadTestDataBuffer_Tag
{
	PageRequest pageRequest;
	Page page;
	SystemSetData systemSetData;
	TestData * testData;
	char sendBuf[UPLOADSENDBUFLEN];
	char recvBuf[UPLOADRECVBUFLEN];
	char tempBuf[UPLOADTEMPBUFLEN];
	char tempbuf2[50];
	unsigned char k;
	unsigned short i;
	unsigned short j;
}UpLoadTestDataBuffer;
#pragma pack()

#define	UpLoadTestDataBufferStructSize	sizeof(UpLoadTestDataBuffer)

void UpLoadFunction(void);

#endif
