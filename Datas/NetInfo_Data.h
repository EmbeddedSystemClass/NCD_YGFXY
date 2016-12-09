/****************************************file start****************************************************/
#ifndef NETINFO_D_H
#define NETINFO_D_H

#include	"Define.h"

#pragma pack(1)
typedef struct
{
	IP_Def LineIP;										//������ip
	unsigned char LineMAC[6];							//������MAC
	unsigned char LineStatus;							//����������״̬��0 -- �����磬 1 -- ������
	
	IP_Def WifiIP;										//wifi��ip
	char WifiSSID[30];									//������wifi������
	unsigned char WifiMAC[6];							//wifiģ���mac
	unsigned char WifiIndicator;						//������wifi���ź�
}NetInfo_Type; 
#pragma pack()


void GetGB_NetInfo(NetInfo_Type * netinfo);
void SetGB_LineNetIP(unsigned int ip);
void SetGB_LineNetMac(unsigned char * mac);
void SetGB_LineNetStatus(unsigned char status);
void SetGB_WifiSSID(char * ssid);
void SetGB_WifiIP(IP_Def * ip);
void SetGB_WifiMAC(unsigned char * mac);
void SetGB_WifiIndicator(unsigned char indicator);
	
#endif

/****************************************end of file************************************************/
