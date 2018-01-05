#ifndef _REMOTESOFT_D_H__
#define _REMOTESOFT_D_H__


#include	"Define.h"

#pragma pack(1)
typedef struct
{
	unsigned short RemoteFirmwareVersion;
	char md5[32];
	char desc[200];
	MyBool isSuccessDownloadFirmware;
}RemoteSoftInfo;
#pragma pack()

#define	RemoteSoftInfoStructSize	sizeof(RemoteSoftInfo)

void setGbRemoteFirmwareVersion(unsigned short version);
unsigned short getGbRemoteFirmwareVersion(void);
void setIsSuccessDownloadFirmware(MyBool status);
MyBool getIsSuccessDownloadFirmware(void);
void setGbRemoteFirmwareMd5(char * md5);
MyBool checkMd5IsSame(char * sMd5, char * dMd5);

#endif

