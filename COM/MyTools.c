/***************************************************************************************************
*FileName:
*Description:
*Author:xsx
*Data:
***************************************************************************************************/


/***************************************************************************************************/
/******************************************头文件***************************************************/
/***************************************************************************************************/

#include	"MyTools.h"

#include	"Define.h"

#include	"math.h"
#include	<string.h>
#include	"stdio.h"
#include 	"stdlib.h"
/***************************************************************************************************/
/**************************************局部变量声明*************************************************/
/***************************************************************************************************/





/***************************************************************************************************/
/**************************************局部函数声明*************************************************/
/***************************************************************************************************/







/***************************************************************************************************/
/***************************************************************************************************/
/***************************************正文********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/************************************************************************
** 函数名:CheckStrIsSame
** 功  能:比较字符串是否相同
** 输  入:无
** 输  出:无
** 返  回：无
** 备  注：无
** 时  间:  
** 作  者：xsx                                                 
************************************************************************/
MyBool CheckStrIsSame(void *str1 , void * str2 , unsigned short len)
{
	unsigned char *p = (unsigned char *)str1;
	unsigned char *q = (unsigned char *)str2;
	unsigned short i=0;
	
	if((NULL == p) || (NULL == q))
		return FALSE;
	
	for(i=0; i<len; i++)
	{
		if(*q++ != *p++)
			return FALSE;
	}
	
	return TRUE;
}

/***************************************************************************************************
*FunctionName: calculateDataCV
*Description: 计算cv值
*Input: datas -- 数据
*		len -- 数据长度
*		sum -- 数据累加和，如果为0，则需要计算，否则直接使用
*Output: 
*Return: 
*Author: xsx
*Date: 2017年8月2日 14:06:33
***************************************************************************************************/
float calculateDataCV(unsigned short * datas, unsigned short len, double sum)
{
	double average = 0;
	double tempV1 = 0;
	double tempV2 = 0;
	unsigned short i=0;
	unsigned short *p = datas;
	
	if(sum == 0)
	{
		for(i=0; i<len; i++)
			average += *p++;
	}
	else
		average = sum;
	average /= len;
	
	p = datas;
	for(i=0; i<len; i++)
	{
		tempV1 = *p++;
		tempV1 -= average;
		tempV1 *= tempV1;
		tempV2 += tempV1;
	}
	
	tempV2 /= len;
	tempV2 = sqrt(tempV2);

	return tempV2 / average;
}

void findFeng(unsigned short * datas, unsigned short startIndex, unsigned short midIndex, unsigned short endIndex, Point * myPoint)
{
	unsigned short i=0, j=0;
	double tempv1 = 0;
	Point tempPoint;
		
	myPoint->x = 0;
	myPoint->y = 0;
	for(i=startIndex; i<endIndex; i++)
	{
		tempPoint.x = 0;
		tempPoint.y = 0;
		for(j=i-15; j<15+i; j++) 
		{
			tempv1 = datas[j];
			if(tempPoint.y < tempv1) 
			{
				tempPoint.y = tempv1;
				tempPoint.x = j;
			}
		}

		for(j=0; j<10; j++) 
		{
			if(datas[tempPoint.x-j] < datas[tempPoint.x-j-1])
				break;

			if(tempPoint.x+j+1 < MaxPointLen)
			{
				if(datas[tempPoint.x+j] < datas[tempPoint.x+j+1])
					break;
			}		
		}

		if(j < 10)
		{
			i += 10;
			continue;
		}
		else 
		{
			if(tempPoint.y > myPoint->y)
			{
				myPoint->x = tempPoint.x;
				myPoint->y = tempPoint.y;
			}
			i = (tempPoint.x + 15);
		}
	}
}

MyState_TypeDef parseIpString(IP_Def * ip, char * ipStr)
{
	char * tempP = NULL;
		
	if(ip == NULL || ipStr == NULL)
		return My_Fail;
	
	tempP = strtok(ipStr, ".");
	if(tempP)
	{
		ip->ip_1 = strtol(tempP, NULL, 10);
		if(ip->ip_1 > 255)
			return My_Fail;
	}
	else
		return My_Fail;
	
	tempP = strtok(NULL, ".");
	if(tempP)
	{
		ip->ip_2 = strtol(tempP, NULL, 10);
		if(ip->ip_2 > 255)
			return My_Fail;
	}
	else
		return My_Fail;
	
	tempP = strtok(NULL, ".");
	if(tempP)
	{
		ip->ip_3 = strtol(tempP, NULL, 10);
		if(ip->ip_3 > 255)
			return My_Fail;
	}
	else
		return My_Fail;
	
	tempP = strtok(NULL, ".");
	if(tempP)
	{
		ip->ip_4 = strtol(tempP, NULL, 10);
		if(ip->ip_4 > 255)
			return My_Fail;
	}
	else
		return My_Fail;
	
	return My_Pass;
}

MyState_TypeDef parsePortString(unsigned short * port, char * portStr)
{
	if(port == NULL || portStr == NULL)
		return My_Fail;
	
	*port = strtol(portStr, NULL, 10);
	if(*port > 65535)
		return My_Fail;
	
	return My_Pass;
}
