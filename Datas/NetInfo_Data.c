/***************************************************************************************************
*FileName:NetInfo_Data
*Description: ����ʵʱ��������wifi״̬
*Author: xsx_kair
*Data:2016��12��5��16:11:49
***************************************************************************************************/

/***************************************************************************************************/
/******************************************Header List********************************************/
/***************************************************************************************************/
#include	"NetInfo_Data.h"

#include 	"FreeRTOS.h"
#include 	"task.h"

#include	<string.h>
#include	"stdio.h"
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
static NetInfo_Type GB_NetInfo;												//ϵͳʵʱ������Ϣ
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/****************************************File Start*************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************
*FunctionName: GetGB_NetInfo
*Description: ��ȡ���µ�ϵͳ������Ϣ
*Input: netinfo -- ������Ϣ��ŵ�ַ
*Output: None
*Return: None
*Author: xsx
*Date: 2016��12��5��16:26:56
***************************************************************************************************/
void GetGB_NetInfo(NetInfo_Type * netinfo)
{
	vTaskSuspendAll();
	
	memcpy(netinfo, &GB_NetInfo, sizeof(NetInfo_Type));
	
	xTaskResumeAll();
}

void SetGB_LineNetIP(unsigned int ip)
{
	vTaskSuspendAll();
	
	GB_NetInfo.LineIP.ip_4 = (ip >> 24)&0xff;
	GB_NetInfo.LineIP.ip_3 = (ip >> 16)&0xff;
	GB_NetInfo.LineIP.ip_2 = (ip >> 8)&0xff;
	GB_NetInfo.LineIP.ip_1 = (ip >> 0)&0xff;
	
	xTaskResumeAll();
}

void SetGB_LineNetMac(unsigned char * mac)
{
	vTaskSuspendAll();
	
	memcpy(GB_NetInfo.LineMAC, mac, 6);
	
	xTaskResumeAll();
}

void SetGB_LineNetStatus(unsigned char status)
{
	vTaskSuspendAll();
	
	GB_NetInfo.LineStatus = status;
	
	xTaskResumeAll();
}

void SetGB_WifiSSID(char * ssid)
{
	vTaskSuspendAll();
	
	memcpy(GB_NetInfo.WifiSSID, ssid, strlen(ssid));
	
	xTaskResumeAll();
}

void SetGB_WifiIP(IP_Def * ip)
{
	vTaskSuspendAll();
	
	memcpy(&(GB_NetInfo.WifiIP), ip, sizeof(IP_Def));
	
	xTaskResumeAll();
}

void SetGB_WifiMAC(unsigned char * mac)
{
	vTaskSuspendAll();
	
	memcpy(GB_NetInfo.WifiMAC, mac, 6);
	
	xTaskResumeAll();
}

void SetGB_WifiIndicator(unsigned char indicator)
{	
	vTaskSuspendAll();
	
	GB_NetInfo.WifiIndicator = indicator;
	
	xTaskResumeAll();
}
/****************************************end of file************************************************/
