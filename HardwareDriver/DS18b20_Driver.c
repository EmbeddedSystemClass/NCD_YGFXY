/***************************************************************************************************
*FileName:	DS18b20_Driver
*Description:	18b20��������
*Author: xsx_kair
*Data:	2017��1��18��11:04:09
***************************************************************************************************/

/***************************************************************************************************/
/******************************************Header List********************************************/
/***************************************************************************************************/

#include	"DS18b20_Driver.h"
#include	"Delay.h"

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
static void DS18B20_GPIO_Config(void);
static void DS18B20_Mode_IPU(void);
static void DS18B20_Mode_Out_PP(void);
static void DS18B20_Rst(void);
static uint8_t DS18B20_Presence(void);
static unsigned char CRC8(unsigned char * buf);
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/****************************************File Start*************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************
*FunctionName: DS18B20_Init
*Description: ��ʼ��18b20
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2017��1��18��11:13:23
***************************************************************************************************/
void DS18B20_Init(void)
{
	
	DS18B20_GPIO_Config();
}

/***************************************************************************************************
*FunctionName: DS18B20_GPIO_Config
*Description: IO��ʼ��
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2017��1��18��11:04:39
***************************************************************************************************/
static void DS18B20_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(DS18B20_CLK, ENABLE); 
														   
  	GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;	
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	GPIO_Init(DS18B20_PORT, &GPIO_InitStructure); 
}

/***************************************************************************************************
*FunctionName: DS18B20_Mode_IPU
*Description: ���ö˿�Ϊ��������
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2017��1��18��11:08:19
***************************************************************************************************/
static void DS18B20_Mode_IPU(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN ; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	
	GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);	 
}

/***************************************************************************************************
*FunctionName: DS18B20_Mode_Out_PP
*Description: ����Ϊ�������
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2017��1��18��11:08:49
***************************************************************************************************/
static void DS18B20_Mode_Out_PP(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);	 	 
}


/***************************************************************************************************
*FunctionName: DS18B20_Rst
*Description: ��λ18b20
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2017��1��18��11:10:32
***************************************************************************************************/
static void DS18B20_Rst(void)
{
	/* ��������Ϊ������� */
	DS18B20_Mode_Out_PP();
	
	DS18B20_DATA_OUT(DS18B20_LOW);
	
	/* �������ٲ���480us�ĵ͵�ƽ��λ�ź� */
	delay_us(750);

	/* �����ڲ�����λ�źź��轫�������� */
	DS18B20_DATA_OUT(DS18B20_HIGH);
	
	/*�ӻ����յ������ĸ�λ�źź󣬻���15~60us���������һ����������*/
	delay_us(15);
}

/***************************************************************************************************
*FunctionName: DS18B20_Presence
*Description: ���ӻ����������صĴ�������
*Input: 
*Output: 
*Return: 
 * 0���ɹ�
 * 1��ʧ��
*Author: xsx
*Date: 2017��1��18��11:11:08
***************************************************************************************************/
static uint8_t DS18B20_Presence(void)
{
	uint8_t pulse_time = 0;
	
	/* ��������Ϊ�������� */
	DS18B20_Mode_IPU();
	
	/* �ȴ���������ĵ�������������Ϊһ��60~240us�ĵ͵�ƽ�ź� 
	 * �����������û����������ʱ�����ӻ����յ������ĸ�λ�źź󣬻���15~60us���������һ����������
	 */
	while( DS18B20_DATA_IN() && pulse_time<200 )
	{
		pulse_time++;
		delay_us(1);
	}	
	/* ����100us�󣬴������嶼��û�е���*/
	if( pulse_time >=200 )
		return 1;
	else
		pulse_time = 0;
	
	/* �������嵽�����Ҵ��ڵ�ʱ�䲻�ܳ���240us */
	while( !DS18B20_DATA_IN() && pulse_time<240 )
	{
		pulse_time++;
		delay_us(1);
	}	
	if( pulse_time >=240 )
		return 1;
	else
		return 0;
}

/***************************************************************************************************
*FunctionName: DS18B20_Read_Bit
*Description: ��ȡ1bit
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2017��1��18��11:11:37
***************************************************************************************************/
static uint8_t DS18B20_Read_Bit(void)
{
	uint8_t dat;
	
	/* ��0�Ͷ�1��ʱ������Ҫ����60us */	
	DS18B20_Mode_Out_PP();
	/* ��ʱ�����ʼ���������������� >1us <15us �ĵ͵�ƽ�ź� */
	DS18B20_DATA_OUT(DS18B20_LOW);
	delay_us(5);
	
	/* ���ó����룬�ͷ����ߣ����ⲿ�������轫�������� */
	DS18B20_DATA_OUT(DS18B20_HIGH);
	DS18B20_Mode_IPU();
	delay_us(12);
	
	if( DS18B20_DATA_IN() == SET )
		dat = 1;
	else
		dat = 0;
	
	/* �����ʱ������ο�ʱ��ͼ */
	delay_us(45);
	
	return dat;
}

/***************************************************************************************************
*FunctionName: DS18B20_Read_Byte
*Description: ��DS18B20��һ���ֽڣ���λ����
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2017��1��18��11:12:04
***************************************************************************************************/
uint8_t DS18B20_Read_Byte(void)
{
	uint8_t i, j, dat = 0;	
	
	for(i=0; i<8; i++) 
	{
		j = DS18B20_Read_Bit();		
		dat = (dat) | (j<<i);
	}
	
	return dat;
}

/***************************************************************************************************
*FunctionName: DS18B20_Write_Byte
*Description: дһ���ֽڵ�DS18B20����λ����
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2017��1��18��11:12:39
***************************************************************************************************/
void DS18B20_Write_Byte(uint8_t dat)
{
	uint8_t i, testb;
	DS18B20_Mode_Out_PP();
	
	for( i=0; i<8; i++ )
	{
		testb = dat&0x01;
		dat = dat>>1;		
		/* д0��д1��ʱ������Ҫ����60us */
		if (testb)
		{			
			DS18B20_DATA_OUT(DS18B20_LOW);
			/* 1us < �����ʱ < 15us */
			delay_us(8);
			
			DS18B20_DATA_OUT(DS18B20_HIGH);
			delay_us(58);
		}		
		else
		{			
			DS18B20_DATA_OUT(DS18B20_LOW);
			/* 60us < Tx 0 < 120us */
			delay_us(70);
			
			DS18B20_DATA_OUT(DS18B20_HIGH);			
			/* 1us < Trec(�ָ�ʱ��) < �����*/
			delay_us(2);
		}
	}
}


/*
 * �洢���¶���16 λ�Ĵ�������չ�Ķ����Ʋ�����ʽ
 * ��������12λ�ֱ���ʱ������5������λ��7������λ��4��С��λ
 *
 *         |---------����----------|-----С�� �ֱ��� 1/(2^4)=0.0625----|
 * ���ֽ�  | 2^3 | 2^2 | 2^1 | 2^0 | 2^(-1) | 2^(-2) | 2^(-3) | 2^(-4) |
 *
 *
 *         |-----����λ��0->��  1->��-------|-----------����-----------|
 * ���ֽ�  |  s  |  s  |  s  |  s  |    s   |   2^6  |   2^5  |   2^4  |
 *
 * 
 * �¶� = ����λ + ���� + С��*0.0625
 */
float DS18B20_Get_Temp(void)
{
	uint8_t value[9], i=0;
	
	short s_tem;
	float f_tem;
	
	DS18B20_Rst();	   
	DS18B20_Presence();	 
	DS18B20_Write_Byte(0XCC);				/* ���� ROM */
	DS18B20_Write_Byte(0X44);				/* ��ʼת�� */
	
	DS18B20_Rst();
	DS18B20_Presence();
	DS18B20_Write_Byte(0XCC);				/* ���� ROM */
	DS18B20_Write_Byte(0XBE);				/* ���¶�ֵ */
	
	for(i=0; i<9; i++)
		value[i] = DS18B20_Read_Byte();
	
	if(value[8] == CRC8(value))
	{
		
		s_tem = value[1]<<8;
		s_tem = s_tem | value[0];
		
		if( s_tem < 0 )		/* ���¶� */
			f_tem = (~s_tem+1) * 0.0625;	
		else
			f_tem = s_tem * 0.0625;
	}
	else
		f_tem = -200;							//���crc���󣬽��¶����ó�-200��������ȡʧ�ܣ�18b20������¶�Ϊ-55
	
	return f_tem; 	
}

 /**
  * @brief  ��ƥ�� ROM ����»�ȡ DS18B20 �¶�ֵ 
  * @param  ds18b20_id�����ڴ�� DS18B20 ���кŵ�������׵�ַ
  * @retval ��
  */
void DS18B20_ReadId ( uint8_t * ds18b20_id )
{
	uint8_t uc;
	
	
	DS18B20_Write_Byte(0x33);       //��ȡ���к�
	
	for ( uc = 0; uc < 8; uc ++ )
	  ds18b20_id [ uc ] = DS18B20_Read_Byte();
	
}

static unsigned char CRC8(unsigned char * buf)
{     
	unsigned char i,x; 
	unsigned char crcbuff;  

	unsigned char crc=0;  

	for(x = 0; x <8; x++)  
	{  
		crcbuff = buf[x];  

		for(i = 0; i < 8; i++)   
		{   
			if(((crc ^ crcbuff)&0x01)==0)   
				crc >>= 1;   
			else 
			{   
				crc ^= 0x18;   //CRC=X8+X5+X4+1  

				crc >>= 1;   

				crc |= 0x80;   

			}           
		
			crcbuff >>= 1;         

		}
	}  

	return crc; 
} 
/****************************************end of file************************************************/





