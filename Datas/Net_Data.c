/***************************************************************************************************
*FileName:Net_Data
*Description:������������
*Author:xsx
*Data:2016��5��8��16:41:34
***************************************************************************************************/

/***************************************************************************************************/
/******************************************ͷ�ļ�***************************************************/
/***************************************************************************************************/
#include	"Net_Data.h"
#include	"MyMem.h"
#include	"CRC16.h"
#include 	"task.h"

#include	<string.h>
#include	"stdio.h"
/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/
static NetCard_Type GB_NetCard = NetNone;										//��ǰ����

static LinkState_TypeDef GB_LinkState = Link_Down;								//������״̬
static LinkState_TypeDef GB_WifiState = Link_Down;								//wifi����״̬

static LinkState_TypeDef GB_NCDServerLinkState = Link_Down;						//Ŧ���ȷ���������״̬

static NetData GB_NetData;														//�豸��������

/*��������*/
static xQueueHandle xRxQueue1 = NULL;											//���ն���
static xQueueHandle xTxQueue1 = NULL;											//���Ͷ���

/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/


/***************************************************************************************************/
/***************************************************************************************************/
/***************************************����********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************
*FunctionName��SetGB_NetCard,GetGB_NetCard
*Description����ǵ�ǰ���õ���������
*Input��None
*Output��None
*Author��xsx
*Data��2016��7��21��19:44:59
***************************************************************************************************/
void SetGB_NetCard(NetCard_Type netcard)
{
	GB_NetCard = netcard;
}

NetCard_Type GetGB_NetCard(void)
{
	return GB_NetCard;
}

/***************************************************************************************************
*FunctionName��SetGB_LinkState
*Description��������������״̬
*Input��None
*Output��None
*Author��xsx
*Data��2016��3��9��15:16:33
***************************************************************************************************/
void SetGB_LinkState(LinkState_TypeDef linkstate)
{
	GB_LinkState = linkstate;
}

LinkState_TypeDef GetGB_LinkState(void)
{
	return GB_LinkState;
}

/***************************************************************************************************
*FunctionName��SetGB_WifiState,GetGB_WifiState
*Description��wifi����״̬
*Input��None
*Output��None
*Author��xsx
*Data��2016��7��21��19:08:25
***************************************************************************************************/
void SetGB_WifiState(LinkState_TypeDef linkstate)
{
	GB_WifiState = linkstate;
}

LinkState_TypeDef GetGB_WifiState(void)
{
	return GB_WifiState;
}

/***************************************************************************************************
*FunctionName��SetGB_NCDServerLinkState�� GetGB_NCDServerLinkState
*Description��Ŧ���ȷ�����������״̬
*Input��None
*Output��None
*Author��xsx
*Data��2016��7��22��19:37:40
***************************************************************************************************/
void SetGB_NCDServerLinkState(LinkState_TypeDef linkstate)
{
	GB_NCDServerLinkState = linkstate;
}

LinkState_TypeDef GetGB_NCDServerLinkState(void)
{
	return GB_NCDServerLinkState;
}


/***************************************************************************************************
*FunctionName��SetGB_NetData,GetGB_NetData
*Description������������
*Input��None
*Output��None
*Author��xsx
*Data��
***************************************************************************************************/
void SetGB_NetData(NetData netdata)
{
	GB_NetData = netdata;
}

NetData * GetGB_NetData(void)
{
	return &GB_NetData;
}

NetIP_Type GetNteIPMode(void)
{
	if(GB_NetData.crc == CalModbusCRC16Fun1(&GB_NetData, sizeof(NetData)-2))
		return GB_NetData.ipmode;
	else
		return DHCP_Mode;
}

void GetMyIP(IP_Def * myip)
{
	if(GB_NetData.crc == CalModbusCRC16Fun1(&GB_NetData, sizeof(NetData)-2))
		memcpy(myip, &(GB_NetData.myip), sizeof(IP_Def));
	else
	{
		myip->ip_1 = 192;
		myip->ip_2 = 168;
		myip->ip_3 = 1;
		myip->ip_4 = 127;
	}
}
void GetServerIP(IP_Def * serverip)
{
	if(GB_NetData.crc == CalModbusCRC16Fun1(&GB_NetData, sizeof(NetData)-2))
		memcpy(serverip, &(GB_NetData.serverip), sizeof(IP_Def));
}


void CreateNetQueue(void)
{
	/*��������������*/
	if(xTxQueue1 == NULL)
		xTxQueue1 = xQueueCreate( 10, ( unsigned portBASE_TYPE ) sizeof( mynetbuf ) );
	
	if(xRxQueue1 == NULL)
		xRxQueue1 = xQueueCreate( 10, ( unsigned portBASE_TYPE ) sizeof( mynetbuf ) );
}

/*��ȡŦ���ȷ��������ݽӿ�*/
xQueueHandle GetGBNCDClientTXQueue(void)
{
	return xTxQueue1;
}

xQueueHandle GetGBNCDClientRXQueue(void)
{
	return xRxQueue1;
}



