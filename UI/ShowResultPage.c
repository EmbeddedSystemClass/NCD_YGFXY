/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"ShowResultPage.h"
#include	"Define.h"
#include	"LCD_Driver.h"
#include	"UI_Data.h"
#include	"MyMem.h"
#include	"Test_Fun.h"

#include	"MyTest_Data.h"
#include	"Temperature_Data.h"
#include	"Time_Data.h"
#include	"Define.h"
#include	"SDFunction.h"
#include	"RecordPage.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"

/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static ShowPageBuffer * GB_ShowPageBuffer = NULL;

static unsigned int TestLineHigh = 76800;	//��������������ʾ����߶��йأ�������治�ģ���������
/******************************************************************************************/
/*****************************************�ֲ���������*************************************/

static void Input(unsigned char *pbuf , unsigned short len);
static void PageUpDate(void);

static void ClearPage(void);
static void RefreshText(void);
static void DspLine(void);
static void dspIco(void);

static MyState_TypeDef PageInit(void *  parm);
static MyState_TypeDef PageBufferMalloc(void);
static MyState_TypeDef PageBufferFree(void);
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/

unsigned char DspShowResultPage(void *  parm)
{	
	SetGBSysPage(DspShowResultPage, DspRecordPage, NULL, Input, PageUpDate, PageInit, PageBufferMalloc, PageBufferFree);
	
	GBPageInit(parm);

	return 0;
}


static void Input(unsigned char *pbuf , unsigned short len)
{
	unsigned short *pdata = NULL;
	
	pdata = MyMalloc((len/2)*sizeof(unsigned short));
	if(pdata == NULL)
		return;
	
	/*����*/
	pdata[0] = pbuf[4];
	pdata[0] = (pdata[0]<<8) + pbuf[5];
	
	/*�˳�*/
	if(0x2b01 == pdata[0])
	{
		GBPageBufferFree();
		GotoGBParentPage(NULL);
	}
	/*��ӡ*/
	else if(0x2b00 == pdata[0])
	{
		;
	}
	
	MyFree(pdata);
}

static void PageUpDate(void)
{

}

static MyState_TypeDef PageInit(void *  parm)
{
	if(My_Fail == PageBufferMalloc())
		return My_Fail;
	
	SelectPage(110);
	
	ClearPage();

	if(parm && GB_ShowPageBuffer)
	{
		GB_ShowPageBuffer->testdata = parm;
		
		RefreshText();
		
		DspLine();
		
		dspIco();
	}
	
	return My_Pass;
}

static MyState_TypeDef PageBufferMalloc(void)
{	
	GB_ShowPageBuffer = (ShowPageBuffer *)MyMalloc(sizeof(ShowPageBuffer));
			
	if(GB_ShowPageBuffer)
	{
		memset(GB_ShowPageBuffer, 0, sizeof(ShowPageBuffer));
		
		return My_Pass;
	}
	else
		return My_Fail;
}

static MyState_TypeDef PageBufferFree(void)
{
	MyFree(GB_ShowPageBuffer);
	GB_ShowPageBuffer = NULL;
	
	return My_Pass;
}

static void ClearPage(void)
{
	ClearLine(0x58);
	
	ClearText(0x2b10, 30);
	
	ClearText(0x2b20, 30);
	
	ClearText(0x2b30, 30);
	
	ClearText(0x2b40, 30);
	
	ClearText(0x2b50, 30);

	DspNum(0x2b70 , 0);
	DspNum(0x2b78 , 0);	

}
static void RefreshText(void)
{
	if(GB_ShowPageBuffer)
	{
		memset(GB_ShowPageBuffer->tempbuf, 0, 100);
		sprintf(GB_ShowPageBuffer->tempbuf, "%s", GB_ShowPageBuffer->testdata->temperweima.ItemName);
		DisText(0x2b10, GB_ShowPageBuffer->tempbuf, strlen(GB_ShowPageBuffer->tempbuf));
				
		memset(GB_ShowPageBuffer->tempbuf, 0, 100);
		sprintf(GB_ShowPageBuffer->tempbuf, "%s", GB_ShowPageBuffer->testdata->sampleid);
		DisText(0x2b20, GB_ShowPageBuffer->tempbuf, strlen(GB_ShowPageBuffer->tempbuf));
				
		sprintf(GB_ShowPageBuffer->tempbuf, "%2.1f", GB_ShowPageBuffer->testdata->TestTemp.O_Temperature);
		DisText(0x2b30, GB_ShowPageBuffer->tempbuf, strlen(GB_ShowPageBuffer->tempbuf));
				
		sprintf(GB_ShowPageBuffer->tempbuf, "%s", GB_ShowPageBuffer->testdata->temperweima.CardPiCi);
		DisText(0x2b40, GB_ShowPageBuffer->tempbuf, strlen(GB_ShowPageBuffer->tempbuf));
				
		sprintf(GB_ShowPageBuffer->tempbuf, "%.2f", GB_ShowPageBuffer->testdata->testline.AdjustResult);
		DisText(0x2b50, GB_ShowPageBuffer->tempbuf, strlen(GB_ShowPageBuffer->tempbuf));

	}
}

static void DspLine(void)
{
	unsigned short i=0;
	double TempMul_Y2;
	double TempY_Scale;
	unsigned short tempvalue;
	
	unsigned short *p = NULL;

	for(i=0; i<MaxPointLen;i++)
	{
		if(i%10 == 0)
		{
			p = &(GB_ShowPageBuffer->testdata->testline.TestPoint[i]);
			DisPlayLine(2 , p , 10);
		}
		
		if(GB_ShowPageBuffer->lineinfo.MaxData <= GB_ShowPageBuffer->testdata->testline.TestPoint[i])
			GB_ShowPageBuffer->lineinfo.MaxData = GB_ShowPageBuffer->testdata->testline.TestPoint[i];
	}
	
	////////////////////////��Ե�ǰ�������ֵ����y��Ŵ���//////////////////////////////////////
		TempMul_Y2 = TestLineHigh;
		TempMul_Y2 /= GB_ShowPageBuffer->lineinfo.MaxData;
		TempMul_Y2 *= 0.8;			//*0.8�ǽ����ֵ���ŵ����̶ȵ�0.8�߶ȴ�

		
		tempvalue = (unsigned short)(TempMul_Y2*10);
		GB_ShowPageBuffer->lineinfo.MUL_Y = ((tempvalue%10) > 5)?(tempvalue/10 + 1):(tempvalue/10);
		
		if(GB_ShowPageBuffer->lineinfo.MUL_Y < 1)			//��СֵΪ1
			GB_ShowPageBuffer->lineinfo.MUL_Y = 1;

		/////////////////////////��Ե�ǰ�Ŵ���������y��̶ȵ�������/////////////////////////////////////
		TempY_Scale = TestLineHigh ;
		TempY_Scale /= GB_ShowPageBuffer->lineinfo.MUL_Y;
		TempY_Scale /= 2.0;																//Ŀǰ��ʾ2��y��̶�
		GB_ShowPageBuffer->lineinfo.Y_Scale = (unsigned short)TempY_Scale;
		
		SetChartSize(0x2b80 , GB_ShowPageBuffer->lineinfo.MUL_Y);

		DspNum(0x2b78 , GB_ShowPageBuffer->lineinfo.Y_Scale);
		DspNum(0x2b70 , GB_ShowPageBuffer->lineinfo.Y_Scale*2);

}

static void dspIco(void)
{
	double tempvalue = 0.0;
	if(GB_ShowPageBuffer)
	{
		//�������ϱ�ǳ�T,C,����
		GB_ShowPageBuffer->myico[0].ICO_ID = 22;
		GB_ShowPageBuffer->myico[0].X = 505+GB_ShowPageBuffer->testdata->testline.T_Point[1]-5;
		tempvalue = GB_ShowPageBuffer->testdata->testline.T_Point[0];
		tempvalue /= GB_ShowPageBuffer->lineinfo.Y_Scale*2;
		tempvalue = 1-tempvalue;
		tempvalue *= 302;										//���ߴ��ڿ��
		tempvalue += 139;										//���ߴ�����ʼy
		GB_ShowPageBuffer->myico[0].Y = (unsigned short)tempvalue - 5;
		
		GB_ShowPageBuffer->myico[1].ICO_ID = 22;
		GB_ShowPageBuffer->myico[1].X = 505+GB_ShowPageBuffer->testdata->testline.C_Point[1]-5;
		tempvalue = GB_ShowPageBuffer->testdata->testline.C_Point[0];
		tempvalue /= GB_ShowPageBuffer->lineinfo.Y_Scale*2;
		tempvalue = 1-tempvalue;
		tempvalue *= 302;										//���ߴ��ڿ��
		tempvalue += 139;										//���ߴ�����ʼy
		GB_ShowPageBuffer->myico[1].Y = (unsigned short)tempvalue - 5;
		
		GB_ShowPageBuffer->myico[2].ICO_ID = 22;
		GB_ShowPageBuffer->myico[2].X = 505+GB_ShowPageBuffer->testdata->testline.B_Point[1]-5;
		tempvalue = GB_ShowPageBuffer->testdata->testline.B_Point[0];
		tempvalue /= GB_ShowPageBuffer->lineinfo.Y_Scale*2;
		tempvalue = 1-tempvalue;
		tempvalue *= 302;										//���ߴ��ڿ��
		tempvalue += 139;										//���ߴ�����ʼy
		GB_ShowPageBuffer->myico[2].Y = (unsigned short)tempvalue - 5;
		
		BasicUI(0x2b60 ,0x1907 , 3, &(GB_ShowPageBuffer->myico[0]) , sizeof(Basic_ICO)*3);
	}
}

