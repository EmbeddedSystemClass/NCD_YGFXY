/***************************************************************************************************
*FileName��LwipDHCP_Task
*Description��LWIP��̬��ȡIP����
*Author��xsx
*Data��2016��3��9��14:38:36
***************************************************************************************************/

/***************************************************************************************************/
/******************************************ͷ�ļ�***************************************************/
/***************************************************************************************************/

#include	"LwipDHCP_Task.h"
#include	"LwIPConfig.h"

#include	"MyMem.h"

#include 	"FreeRTOS.h"
#include 	"task.h"

/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/

#define LwipDHCPTask_PRIORITY			2			//���Ź��������ȼ�
const char * LwipDHCPTaskName = "vLwipDhcpTask";		//���Ź�������

static DHCP_State_TypeDef GB_DHCPState = DHCP_START;
/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/

static void vLwipDHCPTask( void *pvParameters );	//���Ź�����

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************����********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************
*FunctionName��StartvSysLedTask
*Description������ϵͳָʾ������
*Input��None
*Output��None
*Author��xsx
*Data��2015��8��26��16:58:13
***************************************************************************************************/
void StartvLwipDHCPTask(void * pram)
{
	SetGB_DHCPState(DHCP_START);
	
	xTaskCreate( vLwipDHCPTask, LwipDHCPTaskName, configMINIMAL_STACK_SIZE, pram, LwipDHCPTask_PRIORITY, NULL );
}

/***************************************************************************************************
*FunctionName��GetGB_DHCPState
*Description����ȡ���µ�DHCP״̬
*Input��None
*Output��DHCP״̬
*Author��xsx
*Data��2016��3��9��14:49:52
***************************************************************************************************/
DHCP_State_TypeDef GetGB_DHCPState(void)
{
	return GB_DHCPState;
}

/***************************************************************************************************
*FunctionName��SetGB_DHCPState
*Description������DHCP״̬
*Input��NewState -- ���µ�DHCP״̬
*Output��None
*Author��xsx
*Data��2016��3��9��14:50:32
***************************************************************************************************/
void SetGB_DHCPState(DHCP_State_TypeDef NewState)
{
	GB_DHCPState = NewState;
}

/***************************************************************************************************
*FunctionName��vSysLedTask
*Description��ϵͳָʾ����˸���������������
*Input��None
*Output��None
*Author��xsx
*Data��2015��8��26��16:58:46
***************************************************************************************************/
static void vLwipDHCPTask( void *pvParameters )
{
	struct ip_addr ipaddr;
	struct ip_addr netmask;
	struct ip_addr gw;
	unsigned int IPaddress;
	struct netif * tempnetif = (struct netif *)pvParameters;
  
	while(1)
	{
		switch (GetGB_DHCPState())
		{
			case DHCP_START:
				{
					IPaddress = 0;
					
					dhcp_start(tempnetif);
					
					SetGB_DHCPState(DHCP_WAIT_ADDRESS);
				}
				break;

			case DHCP_WAIT_ADDRESS:
				{
					/* Read the new IP address */
					IPaddress = tempnetif->ip_addr.addr;
//					printf("���ڻ�ȡIP.....\r\n");
					if (IPaddress!=0) 
					{
						SetGB_DHCPState(DHCP_ADDRESS_ASSIGNED);					

						/* Stop DHCP */
						dhcp_stop(tempnetif);
					}
					else
					{
						/* DHCP timeout */
						if (tempnetif->dhcp->tries > MAX_DHCP_TRIES)
						{
							SetGB_DHCPState(DHCP_TIMEOUT);
//							printf("IP��ȡʧ��.....\r\n");
							/* Stop DHCP */
							dhcp_stop(tempnetif);

							/* Static address used */
							IP4_ADDR(&ipaddr, IP_ADDR0 ,IP_ADDR1 , IP_ADDR2 , IP_ADDR3 );
							IP4_ADDR(&netmask, NETMASK_ADDR0, NETMASK_ADDR1, NETMASK_ADDR2, NETMASK_ADDR3);
							IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
							netif_set_addr(tempnetif, &ipaddr , &netmask, &gw);
					  
//							printf("ʹ�þ�̬IP %d.%d.%d.%d\r\n", IP_ADDR0 ,IP_ADDR1 , IP_ADDR2 , IP_ADDR3);
						}
					}
				}
				break;

			default: break;
		}
    
		/* wait 250 ms */
		vTaskDelay(250 / portTICK_RATE_MS);
	}   
}


