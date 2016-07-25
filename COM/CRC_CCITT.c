/******************************************************************************************/
/*****************************************头文件*******************************************/

#include	"CRC_CCITT.h"

/******************************************************************************************/
/*****************************************局部变量声明*************************************/

/******************************************************************************************/
/*****************************************局部函数声明*************************************/

/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/

/************************************************************************
** 函数名:
** 功  能:
** 输  入:无
** 输  出:无
** 返  回：无
** 备  注：无
** 时  间:  
** 作  者：xsx                                                 
************************************************************************/
unsigned short CalCRCCCITT_Fun1(void* ptr, unsigned short len) 
{ 
	unsigned short crc = 0; 
	unsigned char *p = (unsigned char *)ptr;
	unsigned char i;
	
	while(len-- != 0) 
	{ 
		for(i = 0x80; i != 0; i /= 2)
		{ 
			crc *= 2; 
			if((crc&0x10000) !=0) //上一位crc乘2后，若首位是1，则除以0x11021
				crc ^= 0x11021; 
			
			if((*p&i) != 0) //如果本位是1，那么crc = 上一位crc+本位crc_ccitt
				crc ^= 0x1021; 
		} 

		p++; 
	} 
	
	return crc; 
}
void CalCRCCCITT_Fun2(void* ptr, unsigned short len, unsigned short *crc) 
{ 
	unsigned char *p = (unsigned char *)ptr;
	unsigned char i;
	
	while(len-- != 0) 
	{ 
		for(i = 0x80; i != 0; i /= 2)
		{ 
			*crc *= 2; 
			if((*crc&0x10000) !=0) //上一位crc乘2后，若首位是1，则除以0x11021
				*crc ^= 0x11021; 
			
			if((*p&i) != 0) //如果本位是1，那么crc = 上一位crc+本位crc_ccitt
				*crc ^= 0x1021; 
		} 

		p++; 
	} 
}

