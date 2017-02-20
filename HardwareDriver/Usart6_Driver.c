/***************************************************************************************************
*FileName:Usart6_Driver
*Description:����6��������Ŀǰ����Һ��ͨ��
*Author:xsx
*Data:2016��4��29��11:26:51
***************************************************************************************************/

/***************************************************************************************************/
/******************************************ͷ�ļ�***************************************************/
/***************************************************************************************************/
#include 	"stm32f4xx.h"
#include 	"Usart6_Driver.h"

/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/

static void ConfigUsart6(void);

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************����********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************
*FunctionName��ConfigUsart6
*Description������6�Ķ˿ڳ�ʼ��������
*Input��None
*Output��None
*Author��xsx
*Data��2016��4��29��11:28:25
***************************************************************************************************/
static void ConfigUsart6(void)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	/* ����GPIO_D��ʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	/* ��������3��ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);

	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);

	USART_InitStructure.USART_BaudRate   = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits   = USART_StopBits_1;
	USART_InitStructure.USART_Parity     = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;

	USART_Init(USART6, &USART_InitStructure);
  
	/* ʹ�ܴ���3 */
	USART_Cmd(USART6, ENABLE);

}

/***************************************************************************************************
*FunctionName��Usart6_Init
*Description������6�ⲿ���ó�ʼ������
*Input��None
*Output��None
*Author��xsx
*Data��2016��4��29��11:28:56
***************************************************************************************************/
void Usart6_Init(void)
{
	ConfigUsart6();
}

/***************************************************************************************************
*FunctionName: USART6_SentData
*Description: ��������
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2017��2��16��16:11:22
***************************************************************************************************/
void USART6_SentData(char * dataBuf, unsigned short len)
{
	unsigned short i = 0;
	unsigned char data = 0;
	
	for(i=0; i<len; i++)
	{
		data = *dataBuf;
		
		USART_SendData(USART6, data);
		while(USART_GetFlagStatus(USART6, USART_FLAG_TXE) == RESET);
		
		dataBuf++;
	}
}

