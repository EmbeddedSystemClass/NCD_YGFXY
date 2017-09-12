/***************************************************************************************************
*FileName:
*Description:
*Author: xsx_kair
*Data:
***************************************************************************************************/

/***************************************************************************************************/
/******************************************Header List********************************************/
/***************************************************************************************************/
#include	"BackDoorData.h"
#include	"System_Data.h"

#include	"Define.h"
#include	"MyTools.h"

#include	<string.h>
#include	"stdio.h"
#include 	"stdlib.h"
#include	<math.h>
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
const BackDoorData s_BackDoorData[12]=
{
	//PCT
	{
		.data1 = 0.2,
		.data2 = {0.3, 2, 5, 10, 50, 100},
		.data3 = {0.5, 10, 50, 50},
		.data4 = {0.5, 10},
		.data5 = {0.5, 10}
	},
	//CRP
	{
		.data1 = 0.5,
		.data2 = {0.5, 3, 10, 20, 50, 200},
		.data3 = {1.03, 5.15, 41.2},
		.data4 = {3, 10},
		.data5 = {3, 10}
	},
	//D_Dimer
	{
		.data1 = 0.2,
		.data2 = {0.2, 0.5, 1, 2, 5, 15},
		.data3 = {0.5, 1, 5, 5},
		.data4 = {0.5, 10},
		.data5 = {0.5, 10}
	},
	//CYSC
	{
		.data1 = 0.5,
		.data2 = {0.5, 2, 3, 5, 6, 8},
		.data3 = {0.5, 1.49, 4.47, 4.47},
		.data4 = {2, 6},
		.data5 = {2, 6}
	},
	//??-HCG
	{
		.data1 = 2.35,
		.data2 = {2.5, 25, 250, 2500, 25000, 200000},
		.data3 = {5, 25, 240},
		.data4 = {2.5, 2500},
		.data5 = {2.5, 2500}
	},
	//PGI
	{
		.data1 = 10,
		.data2 = {10, 25, 50, 75, 100, 150},
		.data3 = {25, 50, 100, 100},
		.data4 = {75, 100},
		.data5 = {75, 100}
	},
	//PGII
	{
		.data1 = 6.25,
		.data2 = {6.25, 12.5, 25, 50, 75, 100},
		.data3 = {12.5, 25, 75, 75},
		.data4 = {12.5, 50},
		.data5 = {12.5, 50}
	},
	//NGAL
	{
		.data1 = 10,
		.data2 = {10, 300, 600, 900, 1200, 1500},
		.data3 = {10, 300, 900, 900},
		.data4 = {10, 300},
		.data5 = {10, 300}
	},
	//MPO
	{
		.data1 = 2,
		.data2 = {2, 20, 100, 150, 200, 300},
		.data3 = {20, 100, 200, 200},
		.data4 = {20, 150},
		.data5 = {20, 150}
	},
	//??2-MG
	{
		.data1 = 0.2,
		.data2 = {1, 2, 6, 9, 12, 18},
		.data3 = {2, 6, 9},
		.data4 = {2, 6},
		.data5 = {2, 6}
	},
	//H-FABP
	{
		.data1 = 2,
		.data2 = {2, 6.25, 12.5, 25, 50, 100},
		.data3 = {6.25, 25, 50},
		.data4 = {6.25, 25},
		.data5 = {6.25, 25}
	},
	//Lp-PLA2
	{
		.data1 = 6,
		.data2 = {10, 50, 200, 400, 600, 800},
		.data3 = {50, 200, 400, 400},
		.data4 = {10, 200},
		.data5 = {10, 200}
	}
};

static double GB_Value = 0.0;
static unsigned char S_ItemIndex = 0;									//???????
static unsigned char S_CategoryIndex = 0;								//??j??????
static unsigned char S_TestIndex = 0;									//??j??????J???

static double wifiResult = 0;											//��wifi���յĽ��
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/****************************************File Start*************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

void SetS_TestItemName(void * name)
{
	if(CheckStrIsSame("PCT", name, strlen(name)))
		SetS_ItemIndex(0);
	else if(CheckStrIsSame("CRP", name, strlen(name)))
		SetS_ItemIndex(1);
	else if(CheckStrIsSame("D-Dimer", name, strlen(name)))
		SetS_ItemIndex(2);
	else if(CheckStrIsSame("CysC", name, strlen(name)))
		SetS_ItemIndex(3);
	else if(CheckStrIsSame("��-HCG", name, strlen(name)))
		SetS_ItemIndex(4);
	else if(CheckStrIsSame("PGI", name, strlen(name)))
		SetS_ItemIndex(5);
	else if(CheckStrIsSame("PGII", name, strlen(name)))
		SetS_ItemIndex(6);
	else if(CheckStrIsSame("NGAL", name, strlen(name)))
		SetS_ItemIndex(7);
	else if(CheckStrIsSame("MPO", name, strlen(name)))
		SetS_ItemIndex(8);
	else if(CheckStrIsSame("��2-MG", name, strlen(name)))
		SetS_ItemIndex(9);
	else if(CheckStrIsSame("H-FABP", name, strlen(name)))
		SetS_ItemIndex(10);
	else if(CheckStrIsSame("LP-PLA2", name, strlen(name)))
		SetS_ItemIndex(11);
}

void SetS_ItemIndex(unsigned char index)
{
	S_ItemIndex = index;
}

unsigned char GetS_ItemIndex(void)
{
	return S_ItemIndex;
}

void SetS_CategoryIndex(unsigned char index)
{
	S_CategoryIndex = index;
}

unsigned char GetS_CategoryIndex(void)
{
	return S_CategoryIndex;
}

void SetS_TestIndex(unsigned char index)
{
	if(index > 0)
		index --;
	S_TestIndex = index;
}

unsigned char GetS_TestIndex(void)
{
	return S_TestIndex;
}


void SetGB_Value(double value)
{
	GB_Value = value;
}

double GetGB_Value(void)
{
	return GB_Value;
}

double GetCurrentData(void)
{	
	double tempresult = 0, tempresult1;
	int a=0;
	int b = 0;
	int c=0;
	int d = 0;
	unsigned char i=0;
	
	MyTime_Def time;
	
//	if(GB_Value != 0.0)
//		return GB_Value;
	
	GetGB_Time(&time);
	
	srand(time.sec+time.min);
	a = pow(-1, ((rand()%2)+1));
	
	switch(S_CategoryIndex)
	{
		case 0:
			tempresult = s_BackDoorData[S_ItemIndex].data1;
			a = -1; c = 500000; d = 150000;
				break;
		case 1:
			tempresult = s_BackDoorData[S_ItemIndex].data2[S_TestIndex / data2_t];
			c = 150000;
				break;
		case 2:
			tempresult = s_BackDoorData[S_ItemIndex].data3[S_TestIndex / data3_t];
			c = 130000;
			break;
		case 3:
			tempresult = s_BackDoorData[S_ItemIndex].data4[S_TestIndex / data4_t];
			c = 130000;
			break;
		case 4:
			tempresult = s_BackDoorData[S_ItemIndex].data5[S_TestIndex / data5_t];
			c = 130000;
			break;
	}
	
	i=0;
	while((b <= d) || (b > c))
	{
		b = (rand()%1000001);
		i++;
		if(i>50)
		{
			b = c/2;
			break;
		}
	}
	
	tempresult1 = b;
	tempresult1 *= 0.000001;
	tempresult1 *= tempresult;
	
	tempresult1 *= a;
	
	return (tempresult + tempresult1);
}

void setwifiResult(double data)
{
	wifiResult = data;
}

double getwifiResult(void)
{
	return wifiResult;
}
/****************************************end of file************************************************/
