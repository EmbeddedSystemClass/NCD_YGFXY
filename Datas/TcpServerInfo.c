/***************************************************************************************************
*FileName:TcpServerInfo
*Description:TCP��������Ϣ��ip���˿ڵ�
*Author:xsx
*Data:2016��4��21��17:20:59
***************************************************************************************************/

/***************************************************************************************************/
/******************************************ͷ�ļ�***************************************************/
/***************************************************************************************************/
#include	"TcpServerInfo.h"

#include 	"api.h"
/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/
static ServerInfo_TypeDef GB_ServerInfo1;						//�ڲ�������

static ServerInfo_TypeDef GB_ServerInfo2;						//��������

/*ֻ�п����Ÿ���һ��*/
static struct ip_addr GB_Server_ipaddr1;						//��ǰ����ʹ�õķ�����ip
static unsigned short GB_Server_port1;							//��ǰ����ʹ�õķ������˿�
static struct ip_addr GB_server_ipaddr2;						//��ǰ����ʹ�õķ�����ip
static unsigned short GB_Server_port2;							//��ǰ����ʹ�õķ������˿�
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
*FunctionName��SetDefaultServerInfo
*Description������Ĭ�Ϸ�������Ϣ
*Input��None
*Output��None
*Author��xsx
*Data��2016��4��21��17:40:55
***************************************************************************************************/
void SetDefaultServerInfo(void)
{
	SetServerInfo(InsideServerInfo, Default_ipaddr1, Default_ipaddr2, Default_ipaddr3, Default_ipaddr4, Default_ipport);
	SetServerInfo(OutsideServerInfo, Default_ipaddr1, Default_ipaddr2, Default_ipaddr3, Default_ipaddr4, Default_ipport);
}

/***************************************************************************************************
*FunctionName��SetServerInfo
*Description�����÷�������Ϣ
*Input��servertype -- ���������,  (myipaddr1��myipaddr4)������ip,   myport -- �������˿ں�
*Output��None
*Author��xsx
*Data��2016��4��21��17:37:03
***************************************************************************************************/
void SetServerInfo(ServerInfoType servertype, unsigned short myipaddr1, unsigned short myipaddr2, unsigned short myipaddr3, 
	unsigned short myipaddr4, unsigned short myport)
{
	ServerInfo_TypeDef * tempserverinfo;
	
	if(servertype == InsideServerInfo)
		tempserverinfo = &GB_ServerInfo1;
	else
		tempserverinfo = &GB_ServerInfo2;
	
	tempserverinfo->ipaddr1 = myipaddr1;
	tempserverinfo->ipaddr2 = myipaddr2;
	tempserverinfo->ipaddr3 = myipaddr3;
	tempserverinfo->ipaddr4 = myipaddr4;
	tempserverinfo->port = myport;
}

/***************************************************************************************************
*FunctionName��GetServerInfo
*Description����ȡ��������Ϣ
*Input��servertype -- ���������
*Output����������Ҫ�ķ�������Ϣ��ַ
*Author��xsx
*Data��2016��4��21��17:38:50
***************************************************************************************************/
ServerInfo_TypeDef * GetServerInfo(ServerInfoType servertype)
{
	ServerInfo_TypeDef * tempserverinfo;
	
	if(servertype == InsideServerInfo)
		tempserverinfo = &GB_ServerInfo1;
	else
		tempserverinfo = &GB_ServerInfo2;
	
	return tempserverinfo;
}