/***************************************************************************************************
*FileName��LCD_Driver
*Description��Һ������һЩ���ܺ���
*Author��xsx
*Data��2016��4��29��10:13:01
***************************************************************************************************/

/***************************************************************************************************/
/******************************************ͷ�ļ�***************************************************/
/***************************************************************************************************/

#include	"LCD_Driver.h"
#include	"QueueUnits.h"

#include	"Time_Data.h"
#include	"Temperature_Data.h"

#include	"CRC16.h"
#include	"Define.h"
#include	"MyMem.h"

#include	"stdio.h"
#include	"string.h"

/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/
static char *txdat = NULL;
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
*FunctionName��SelectPage
*Description��ѡ����ʾ��ͼƬid
*Input��None
*Output��None
*Author��xsx
*Data��2016��4��30��17:02:39
***************************************************************************************************/
void SelectPage(unsigned short index)
{				
	char *q = NULL;
	
	txdat = MyMalloc(50);
	if(txdat == NULL)
		return;
	
	memset(txdat, 0, 50);
	q = txdat;
	
	*q++ = LCD_Head_1;
	*q++ = LCD_Head_2;
	*q++ = 6;
	*q++ = W_REGSITER;
	*q++ = 0x03;
	*q++ = (index>>8);
	*q++ = (index);
	CalModbusCRC16Fun2(txdat+3, 4, q);
	
	SendStrToQueue(GetUsart6TXQueue(), GetUsart6TXMutex(), txdat, txdat[2]+3, 50 * portTICK_RATE_MS, EnableUsart6TXInterrupt);

	MyFree(txdat);
}

/***************************************************************************************************
*FunctionName��DisText
*Description����ʾ�ı�
*Input��None
*Output��None
*Author��xsx
*Data��2016��4��30��17:02:53
***************************************************************************************************/
void DisText(unsigned short addr, void *data, unsigned short len)
{				
	char *q = NULL;
	unsigned char *p = (unsigned char *)data;
	unsigned char i=0;
	
	if(len > 240)
		len = 240;
	
	txdat = MyMalloc(len+20);
	if(txdat == NULL)
		return;
	
	memset(txdat, 0, len+20);
	q = txdat;
	
	*q++ = LCD_Head_1;
	*q++ = LCD_Head_2;
	*q++ = 5+len;
	*q++ = W_ADDRESS;

	*q++ = (addr>>8);
	*q++ = (addr);
	
//	sprintf((char *)q, "%s", p);
	for(i=0; i<len; i++)
		*q++ = *p++;
	
	CalModbusCRC16Fun2(txdat+3, 3+len, q);
	
	SendStrToQueue(GetUsart6TXQueue(), GetUsart6TXMutex(), txdat, txdat[2]+3, 50 * portTICK_RATE_MS, EnableUsart6TXInterrupt);

	MyFree(txdat);
}

void ClearText(unsigned short addr, unsigned short len)
{	
	txdat = MyMalloc(len);
	if(txdat == NULL)
		return;

	memset(txdat, 0xff, len);
	DisText(addr, txdat, len);
	MyFree(txdat);
}
void BasicUI(unsigned short addr , unsigned short cmd, unsigned short datapacknum, void *data , unsigned short strlen)
{
	unsigned char *p = (unsigned char *)data;
	char *q;
	unsigned char i=0;
	
	txdat = MyMalloc(strlen+50);
	q = txdat;
	
	*txdat++ = LCD_Head_1;
	*txdat++ = LCD_Head_2;
	*txdat++ = strlen+7+2;
	*txdat++ = W_ADDRESS;
	
	//����ͼ�α�����ַ
	*txdat++ = addr>>8;		*txdat++ = addr;
	
	//����ͼ��ָ��
	*txdat++ = cmd>>8;		*txdat++ = cmd;
	//����ͼ�����ݰ�����
	*txdat++ = datapacknum>>8;		*txdat++ = datapacknum;
	
	//����keil�ǵ��ֽ���ǰ��������������������Ҫ���ֽ���ǰ�����������ｫ�ߵͻ���
	for(i=0; i<strlen/2; i++)
	{
		*txdat++ = p[i*2+1];
		*txdat++ = p[i*2];
	}
	
	CalModbusCRC16Fun2(q+3, strlen+7, txdat);
	
	SendStrToQueue(GetUsart6TXQueue(), GetUsart6TXMutex(), q, q[2]+3, 50 * portTICK_RATE_MS, EnableUsart6TXInterrupt);

	MyFree(txdat);
	
}

/***************************************************************************************************
*FunctionName��BasicPic
*Description������ͼ��
*Input��addr -- �ؼ���ַ   �� datanum -- ���ݰ�����  �� soureid -- ͼƬԴҳ��id��  
*Output��None
*Author��xsx
*Data��
***************************************************************************************************/
void BasicPic(unsigned short addr,unsigned short datanum, unsigned short soureid ,unsigned short xs,unsigned short ys ,unsigned short xe,unsigned short ye ,
	unsigned short txs,unsigned short tys)
{			
	char *q = NULL;

	txdat = MyMalloc(100);
	if(txdat == NULL)
		return;
	
	memset(txdat, 0, 100);
	q = txdat;
	
	*q++ = LCD_Head_1;
	*q++ = LCD_Head_2;
	*q++ = 23;
	*q++ = W_ADDRESS;
		
	*q++ = addr>>8;		*q++ = addr;
		
	//cmd 0x0006
	*q++ = 0;		*q++ = 6;
		
	//���ݰ�����1
	*q++ = datanum>>8;		*q++ = datanum;
		
	//ԭҳ��id
	*q++ = soureid>>8;	*q++ = soureid;
	//ԭҳ��x
	*q++ = xs>>8;	*q++ = xs;
	//ԭҳ��x
	*q++ = ys>>8;	*q++ = ys;
	//ԭҳ��x
	*q++ = xe>>8;	*q++ = xe;
	//ԭҳ��x
	*q++ = ye>>8;	*q++ = ye;
	//ԭҳ��x
	*q++ = txs>>8;	*q++ = txs;
	//ԭҳ��x
	*q++ = tys>>8;	*q++ = tys;

	CalModbusCRC16Fun2(txdat+3, 21, q);
	
	SendStrToQueue(GetUsart6TXQueue(), GetUsart6TXMutex(), txdat, txdat[2]+3, 50 * portTICK_RATE_MS, EnableUsart6TXInterrupt);

	MyFree(txdat);
}

/***************************************************************************************************
*FunctionName��SendKeyCode
*Description�����ع���
*Input��None
*Output��None
*Author��xsx
*Data��2016��5��1��16:46:15
***************************************************************************************************/
void SendKeyCode(unsigned char keycode)
{				
	char *q = NULL;
	
	txdat = MyMalloc(50);
	if(txdat == NULL)
		return;
	
	memset(txdat, 0, 50);
	q = txdat;
	
	*q++ = LCD_Head_1;
	*q++ = LCD_Head_2;
	*q++ = 5;
	*q++ = W_REGSITER;
	*q++ = 0x4f;
	*q++ = (keycode);
	CalModbusCRC16Fun2(txdat+3, 3, q);
	
	SendStrToQueue(GetUsart6TXQueue(), GetUsart6TXMutex(), txdat, txdat[2]+3, 50 * portTICK_RATE_MS, EnableUsart6TXInterrupt);
	
	MyFree(txdat);
}

void WriteRegister(unsigned char w_r, unsigned char addr, void *data, unsigned char len)
{			
	char *q = NULL;
	unsigned char *p = (unsigned char *)data;
	
	txdat = MyMalloc(len + 10);
	if(txdat == NULL)
		return;
	
	memset(txdat, 0, len + 10);
	q = txdat;
	
	*q++ = LCD_Head_1;
	*q++ = LCD_Head_2;
	*q++ = len+3;
	
	if(w_r)
		*q++ = W_REGSITER;
	else
		*q++ = R_REGSITER;
	
	*q++ = addr;

	while(len--)
		*q++ = *p++;
	
	CalModbusCRC16Fun2(txdat+3, len + 2, q);
	
	SendStrToQueue(GetUsart6TXQueue(), GetUsart6TXMutex(), txdat, txdat[2]+3, 50 * portTICK_RATE_MS, EnableUsart6TXInterrupt);
	
	MyFree(txdat);
}

void WriteData(unsigned short addr, void *data, unsigned char len)
{			
	char *q = NULL;
	unsigned char *p = (unsigned char *)data;
	unsigned char i=0;
	
	txdat = MyMalloc(len + 10);
	if(txdat == NULL)
		return;
	
	memset(txdat, 0, len + 10);
	q = txdat;
	
	*q++ = LCD_Head_1;
	*q++ = LCD_Head_2;
	*q++ = len+5;

	*q++ = W_ADDRESS;
	
	*q++ = addr>>8;
	*q++ = addr;

	for(i=0; i<len; i++)
		*q++ = *p++;
	
	CalModbusCRC16Fun2(txdat+3, len + 3, q);
	
	SendStrToQueue(GetUsart6TXQueue(), GetUsart6TXMutex(), txdat, txdat[2]+3, 50 * portTICK_RATE_MS, EnableUsart6TXInterrupt);

	MyFree(txdat);
}

void ReadData(unsigned short addr, unsigned char len)
{				
	char *q = NULL;
	
	txdat = MyMalloc(100);
	if(txdat == NULL)
		return;
	
	memset(txdat, 0, 100);
	q = txdat;
	
	*q++ = LCD_Head_1;
	*q++ = LCD_Head_2;
	*q++ = 6;

	*q++ = R_ADDRESS;
	
	*q++ = addr>>8;
	*q++ = addr;

	*q++ = len;
	
	CalModbusCRC16Fun2(txdat+3, 4, q);
	
	SendStrToQueue(GetUsart6TXQueue(), GetUsart6TXMutex(), txdat, txdat[2]+3, 50 * portTICK_RATE_MS, EnableUsart6TXInterrupt);
	
	MyFree(txdat);
}

/************************************************************************
** ������:GetBufLen
** ��  ��:���������ص��ַ�����0xff��β
** ��  ��:��
** ��  ��:��
** ��  �أ���
** ��  ע����
** ʱ  ��:  
** ��  �ߣ�xsx                                                 
************************************************************************/
unsigned short GetBufLen(unsigned char *p ,unsigned short len)
{
	unsigned short i ;
	for(i=0; i<len; i++)
			{
				if(p[i] == 0xff)
				{
					return i;
				}
			}
		return 0;
}

void DspTimeAndTempData(void)
{
	MyTime_Def * time = NULL;
	char *buftime = NULL;
	
	buftime = MyMalloc(200);
	time = MyMalloc(sizeof(MyTime_Def));
	
	if(buftime && time)
	{
		memset(buftime, 0, 200);
		memset(time, 0, sizeof(MyTime_Def));
		
		/*��ȡ��ǰʱ��*/
		GetGBTimeData(time);
		
		/*��ʾʱ��*/
		sprintf(buftime, " 20%02d-%02d-%02d %02d:%02d:%02d", time->year, time->month, time->day,
			time->hour, time->min, time->sec);
	
		DisText(0x1000, buftime, strlen(buftime));
	}

	MyFree(time);
	MyFree(buftime);
}

/************************************************************************
** ������:
** ��  ��:
** ��  ��:pram----����
** ��  ��:��
** ��  �أ���
** ��  ע����
** ʱ  ��:  
** ��  �ߣ�xsx                                                 
************************************************************************/
void DisPlayLine(unsigned char channel , void * data , unsigned char datalen)
{
	unsigned short * p = (unsigned short *)data;
				
	char *q = NULL;
	unsigned char i = 0;
	unsigned short tempdat = 0;
		
	txdat = MyMalloc(datalen*2 + 50);
	if(txdat == NULL)
		return;
	
	memset(txdat, 0, datalen*2 + 50);
	q = txdat;

	*q++ = LCD_Head_1;
	*q++ = LCD_Head_2;
	*q++ = (2+datalen*2+2);
	*q++ = W_LINE;
	*q++ = (0x01 << channel);
		
	for(i=0; i<datalen; i++)
	{
		tempdat = *p++;
		*q++ = (unsigned char)(tempdat>>8);
		*q++ = (unsigned char)tempdat;
	}
	
	CalModbusCRC16Fun2(txdat+3, datalen*2 + 2, q);
	
	SendStrToQueue(GetUsart6TXQueue(), GetUsart6TXMutex(), txdat, txdat[2]+3, 50 * portTICK_RATE_MS, EnableUsart6TXInterrupt);

	MyFree(txdat);
}

void SetChartSize(unsigned short add , unsigned char num)
{
	char *q = NULL;
	
	txdat = MyMalloc(20);
	if(txdat == NULL)
		return;
	
	memset(txdat, 0, 20);	
	q = txdat;
	
	add += 0x08;
	
		*q++ = LCD_Head_1;
		*q++ = LCD_Head_2;
		*q++ = 7;
		*q++ = W_ADDRESS;
		*q++ = (add>>8);
		*q++ = (add);
		*q++ = 0;
		*q++ = num;
	
	CalModbusCRC16Fun2(txdat+3, 5, q);
	
	SendStrToQueue(GetUsart6TXQueue(), GetUsart6TXMutex(), txdat, txdat[2]+3, 50 * portTICK_RATE_MS, EnableUsart6TXInterrupt);

	MyFree(txdat);
}

void DspNum(unsigned short addr , unsigned short num)
{
	char *q = NULL;
	
	txdat = MyMalloc(20);
	if(txdat == NULL)
		return;
	
	memset(txdat, 0, 20);	
	q = txdat;
	
		*q++ = LCD_Head_1;
		*q++ = LCD_Head_2;
		*q++ = 7;
		*q++ = W_ADDRESS;
		*q++ = (addr>>8);
		*q++ = (addr);
		*q++ = (num>>8);
		*q++ = (num);
	
	CalModbusCRC16Fun2(txdat+3, 5, q);
	
	SendStrToQueue(GetUsart6TXQueue(), GetUsart6TXMutex(), txdat, txdat[2]+3, 50* portTICK_RATE_MS, EnableUsart6TXInterrupt);
	
	MyFree(txdat);
}

void ClearLine(unsigned char Command)
{
	char *q = NULL;
	
	txdat = MyMalloc(20);
	if(txdat == NULL)
		return;
	
	memset(txdat, 0, 20);	
	q = txdat;
	
		*q++ = LCD_Head_1;
		*q++ = LCD_Head_2;
		*q++ = 5;
		*q++ = W_REGSITER;
		*q++ = 0xEB;
		*q++ = Command;
	
	CalModbusCRC16Fun2(txdat+3, 3, q);
	
	SendStrToQueue(GetUsart6TXQueue(), GetUsart6TXMutex(), txdat, txdat[2]+3, 50 * portTICK_RATE_MS, EnableUsart6TXInterrupt);
	
	MyFree(txdat);

}

/************************************************************************
** ������:SetLEDLight
** ��  ��:num -- ��Ļ���ȣ�1 -- 1%�� 100--100%
** ��  ��:��
** ��  ��:��
** ��  �أ���
** ��  ע����
** ʱ  ��:  
** ��  �ߣ�xsx                                                 
************************************************************************/
void SetLEDLight(unsigned char num)
{
	char *q = NULL;
	
	txdat = MyMalloc(50);
	if(txdat == NULL)
		return;
	
	memset(txdat, 0, 50);
	q = txdat;
	
	*q++ = LCD_Head_1;
	*q++ = LCD_Head_2;
	*q++ = 5;
	*q++ = W_REGSITER;
	*q++ = 0x01;
	*q++ = (num);

	CalModbusCRC16Fun2(txdat+3, 3, q);
	
	SendStrToQueue(GetUsart6TXQueue(), GetUsart6TXMutex(), txdat, txdat[2]+3, 50 * portTICK_RATE_MS, EnableUsart6TXInterrupt);

	MyFree(txdat);
}
