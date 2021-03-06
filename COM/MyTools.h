#ifndef MYTOOLS_H_H
#define MYTOOLS_H_H

#include	"Define.h"

MyBool CheckStrIsSame(void *str1 , void * str2 , unsigned short len);
char * MyStrStr(const char *str1 , const char * str2 , unsigned short len);
float calculateDataCV(unsigned short * datas, unsigned short len, double sum);
void findFeng(unsigned short * datas, unsigned short startIndex, unsigned short midIndex, unsigned short endIndex, Point * myPoint);
MyState_TypeDef parseIpString(IP_Def * ip, char * ipStr);
MyState_TypeDef parsePortString(unsigned short * port, char * portStr);
	
#endif

