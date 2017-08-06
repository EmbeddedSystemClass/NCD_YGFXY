#ifndef MYTOOLS_H_H
#define MYTOOLS_H_H

#include	"Define.h"

bool CheckStrIsSame(void *str1 , void * str2 , unsigned short len);
float calculateDataCV(unsigned short * datas, unsigned short len, double sum);
void findFeng(unsigned short * datas, unsigned short startIndex, unsigned short midIndex, unsigned short endIndex, Point * myPoint);
#endif

