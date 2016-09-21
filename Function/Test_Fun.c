/***************************************************************************************************
*FileName:TestFun
*Description:??
*Author:xsx
*Data:2016?5?14?17:09:53
***************************************************************************************************/

/***************************************************************************************************/
/******************************************???***************************************************/
/***************************************************************************************************/
#include	"Test_Fun.h"

#include	"Ads8325_Driver.h"
#include	"TLV5617_Driver.h"
#include	"MAX4051_Driver.h"
#include	"DRV8825_Driver.h"
#include	"Motor_Fun.h"
#include	"SDFunction.h"
#include	"Motor_Data.h"
#include	"CardStatues_Data.h"
#include	"MyTest_Data.h"
#include	"CRC16.h"

#include	"MyMem.h"

#include	"QueueUnits.h"
#include	"ServerFun.h"

#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"
#include	"stdlib.h"
/***************************************************************************************************/
/**************************************??????*************************************************/
/***************************************************************************************************/
static xQueueHandle xTestCurveQueue = NULL;												//���Ͳ�������

static TempCalData * S_TempCalData = NULL;												//�������ݿռ�
static TestTaskData *S_TestTaskData = NULL;
/***************************************************************************************************/
/**************************************??????*************************************************/
/***************************************************************************************************/
static MyState_TypeDef SendTestPointData(void * data);
static void AnalysisTestData(void);
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************??********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

MyState_TypeDef InitTestFunData(void)
{
	/*���߶���*/
	if(xTestCurveQueue == NULL)
		xTestCurveQueue = xQueueCreate( 1000, ( unsigned portBASE_TYPE ) sizeof( unsigned short ) );
	
	if(xTestCurveQueue == NULL)
		return My_Fail;
	else
		return My_Pass;
}


ResultState TestFunction(void * parm)
{
	unsigned short steps = EndTestLocation - StartTestLocation;
	unsigned short i = 0;
	
	if(parm == NULL)
		return MemError;
	
	/*��ʼ���������߶���*/
	if(xTestCurveQueue == NULL)
		return MemError;
	while(My_Pass == TakeTestPointData(&i))
		;
	
	S_TempCalData = MyMalloc(sizeof(TempCalData));
	
	if(S_TempCalData)
	{
		memset(S_TempCalData, 0, sizeof(TempCalData));
		
		//��ʼ����
		SetGB_LedValue(1000);
		vTaskDelay(10/portTICK_RATE_MS);
		
		SetGB_CLineValue(0);
		vTaskDelay(10/portTICK_RATE_MS);
		
		SelectChannel(7);
		vTaskDelay(10/portTICK_RATE_MS);
		
		S_TestTaskData = parm;
		
		repeat:
				
			MotorMoveTo(0, 0);
				
			MotorMoveTo(StartTestLocation, 0);
		
			memset(S_TempCalData, 0, sizeof(TempCalData));

			for(i=1; i<= steps; i++)
			{
				MotorMoveTo(GetGB_MotorLocation() + 1, 0);
						
				S_TempCalData->tempvalue2 += ADS8325();
				
				if(i%AvregeNum == 0)
				{
					S_TempCalData->tempvalue2 /= AvregeNum;
					
					S_TempCalData->tempvalue = (unsigned short)(S_TempCalData->tempvalue2);
					
					S_TestTaskData->testdata->testline.TestPoint[i/AvregeNum-1] = S_TempCalData->tempvalue;
							
					SendTestPointData(&(S_TestTaskData->testdata->testline.TestPoint[i/AvregeNum-1]));
						
					S_TempCalData->tempvalue2 = 0;
				}
				vTaskDelay(1 / portTICK_RATE_MS);
				
/*				if(GetCardState() == NoCard)				//�����γ�
				{
					S_TempCalData->resultstatues = TestInterrupt;
					goto END;
				}*/
			}
			
			vTaskDelay(10/portTICK_RATE_MS);
			
			//��������
			AnalysisTestData();
			
			if(S_TempCalData->resultstatues == NoResult)
			{
				//����һ���ض����ݣ��������
				S_TestTaskData->testdata->testline.TestPoint[0] = 0xffff;
				SendTestPointData(&(S_TestTaskData->testdata->testline.TestPoint[0]));
				goto repeat;
			}

//			END:
				S_TestTaskData->testresult = S_TempCalData->resultstatues;
				MotorMoveTo(GetGB_MotorMaxLocation(), 0);
			
				MyFree(S_TempCalData);

				return S_TestTaskData->testresult;
	}
	else
		return MemError;	
}

/*????*/
static MyState_TypeDef SendTestPointData(void * data)
{
	if(xTestCurveQueue == NULL)
		return My_Fail;
	
	if(pdPASS == xQueueSend( xTestCurveQueue, data, 10*portTICK_RATE_MS ))
		return My_Pass;
	else
		return My_Fail;	
}

MyState_TypeDef TakeTestPointData(void * data)
{
	if(xTestCurveQueue == NULL)
		return My_Fail;
	
	if(pdPASS == xQueueReceive( xTestCurveQueue, data, 0*portTICK_RATE_MS))
		return My_Pass;
	else
		return My_Fail;	
}


static void AnalysisTestData(void)
{
	unsigned short i=0;
	
	{
		S_TempCalData->maxdata = S_TestTaskData->testdata->testline.TestPoint[0];
		
		/*��������ֵ*/
		for(i=1; i<MaxPointLen; i++)
		{
			S_TempCalData->testline2[i-1] = S_TestTaskData->testdata->testline.TestPoint[i] - S_TestTaskData->testdata->testline.TestPoint[i-1];
			if(S_TempCalData->maxdata < S_TestTaskData->testdata->testline.TestPoint[i])
				S_TempCalData->maxdata = S_TestTaskData->testdata->testline.TestPoint[i];
		}
		
		/*�жϲ���ֵ�Ƿ񱥺�*/
		if(S_TempCalData->maxdata >= 4000)
		{
			if(GetChannel() > 0)
			{
				SelectChannel(GetChannel() - 1);

				vTaskDelay(10/portTICK_RATE_MS);
				return;
			}
		}

		/*Ѱ�����з�*/
		memset(&(S_TempCalData->peakdata[0]), 0, 10*sizeof(PeakData)+1);
		
		for(i=0; i<MaxPointLen-1; i++)
		{
			if((S_TempCalData->peakdata[S_TempCalData->peaknum].step == 0) && (S_TempCalData->testline2[i] > 0))
			{
				S_TempCalData->peakdata[S_TempCalData->peaknum].StartLocation = i;
				S_TempCalData->peakdata[S_TempCalData->peaknum].StartValue = S_TestTaskData->testdata->testline.TestPoint[i];
				S_TempCalData->peakdata[S_TempCalData->peaknum].step = 1;
			}
			else if((S_TempCalData->peakdata[S_TempCalData->peaknum].step == 1) && (S_TempCalData->testline2[i] <= 0))
			{
				S_TempCalData->peakdata[S_TempCalData->peaknum].PeakLocation = i;
				S_TempCalData->peakdata[S_TempCalData->peaknum].PeakValue = S_TestTaskData->testdata->testline.TestPoint[i];
				S_TempCalData->peakdata[S_TempCalData->peaknum].UpWidth = i - S_TempCalData->peakdata[S_TempCalData->peaknum].StartLocation;
				S_TempCalData->peakdata[S_TempCalData->peaknum].step = 2;
			}
			else if((S_TempCalData->peakdata[S_TempCalData->peaknum].step == 2) && (S_TempCalData->testline2[i] >= 0))
			{
				S_TempCalData->peakdata[S_TempCalData->peaknum].EndLocation = i;
				S_TempCalData->peakdata[S_TempCalData->peaknum].EndValue = S_TestTaskData->testdata->testline.TestPoint[i];
				S_TempCalData->peakdata[S_TempCalData->peaknum].DownWidth = i - S_TempCalData->peakdata[S_TempCalData->peaknum].PeakLocation;
				S_TempCalData->peakdata[S_TempCalData->peaknum].PeakWidth = i - S_TempCalData->peakdata[S_TempCalData->peaknum].StartLocation;
				S_TempCalData->peakdata[S_TempCalData->peaknum].PeakScale = S_TempCalData->peakdata[S_TempCalData->peaknum].UpWidth;
				S_TempCalData->peakdata[S_TempCalData->peaknum].PeakScale /= S_TempCalData->peakdata[S_TempCalData->peaknum].DownWidth;
				/*һ�������꣬�����������ж�*/
				/*�����Ŀ��С��30���ߴ���100����˷�Ϊ��*/
				if((S_TempCalData->peakdata[S_TempCalData->peaknum].PeakWidth > 30)&&(S_TempCalData->peakdata[S_TempCalData->peaknum].PeakWidth<150))
				{
					S_TempCalData->peaknum++;
					
				}
				/*����ж�������ͨ�������Ϊ��*/
				else
				{
					S_TempCalData->peakdata[S_TempCalData->peaknum].step = 0;
				}
			}
		}
		
		/*���û�з�*/
		if(S_TempCalData->peaknum == 0)
		{
			S_TempCalData->resultstatues = PeakNumZero;
			return;
		}
		/*���ֻ��һ����*/
/*		else if(S_TempCalData->peaknum == 1)
		{
			S_TempCalData->resultstatues = PeakNumError;
			return;
		}*/
		/*�������ȷ�����ݶ�ά���ҷ�*/
		else
		{

#if (NormalCode == CodeType)			
			for(i=0; i<S_TempCalData->peaknum; i++)
			{
				S_TempCalData->tempvalue2 = S_TempCalData->peakdata[i].PeakLocation - S_TestTaskData->testdata->temperweima.ItemLocation;
				if(abs(S_TempCalData->tempvalue2) < 25)
				{
					S_TestTaskData->testdata->testline.T_Point[0] = S_TempCalData->peakdata[i].PeakValue;
					S_TestTaskData->testdata->testline.T_Point[1] = S_TempCalData->peakdata[i].PeakLocation;
				}
				else
				{
					S_TempCalData->tempvalue2 = S_TempCalData->peakdata[i].PeakLocation - S_TestTaskData->testdata->temperweima.CLineLocation;
					if(abs(S_TempCalData->tempvalue2) < 25)
					{
						S_TestTaskData->testdata->testline.C_Point[0] = S_TempCalData->peakdata[i].PeakValue;
						S_TestTaskData->testdata->testline.C_Point[1] = S_TempCalData->peakdata[i].PeakLocation;
					}
				}	
			}

			/*�ҵ�t��c�ߣ��һ���*/
			S_TestTaskData->testdata->testline.B_Point[0] = 0xffff;
			if((S_TestTaskData->testdata->testline.T_Point[1] != 0) && (S_TestTaskData->testdata->testline.C_Point[1] != 0) && (S_TestTaskData->testdata->testline.C_Point[1] != S_TestTaskData->testdata->testline.T_Point[1]))
			{
				for(i=S_TestTaskData->testdata->testline.T_Point[1]; i<S_TestTaskData->testdata->testline.C_Point[1]; i++)
				{
					if(S_TestTaskData->testdata->testline.B_Point[0] > S_TestTaskData->testdata->testline.TestPoint[i])
					{
						S_TestTaskData->testdata->testline.B_Point[0] = S_TestTaskData->testdata->testline.TestPoint[i];
						S_TestTaskData->testdata->testline.B_Point[1] = i;
					}
				}
				
				/*������*/
				S_TempCalData->tempvalue2 = (S_TestTaskData->testdata->testline.T_Point[0] - S_TestTaskData->testdata->testline.B_Point[0]);
				S_TempCalData->tempvalue2 /= (S_TestTaskData->testdata->testline.C_Point[0] - S_TestTaskData->testdata->testline.B_Point[0]);
				
				/*ԭʼ��߱�*/
				S_TestTaskData->testdata->testline.BasicBili = S_TempCalData->tempvalue2;
				
				/*���ݷֶΣ�����ԭʼ���*/
				if(S_TestTaskData->testdata->testline.BasicBili < S_TestTaskData->testdata->temperweima.ItemFenDuan)
				{
					S_TestTaskData->testdata->testline.BasicResult = S_TestTaskData->testdata->testline.BasicBili * S_TestTaskData->testdata->testline.BasicBili;
					S_TestTaskData->testdata->testline.BasicResult *= S_TestTaskData->testdata->temperweima.ItemBiaoQu[0][0];
					
					S_TestTaskData->testdata->testline.BasicResult += (S_TestTaskData->testdata->testline.BasicBili * S_TestTaskData->testdata->temperweima.ItemBiaoQu[0][1]);
					
					S_TestTaskData->testdata->testline.BasicResult += S_TestTaskData->testdata->temperweima.ItemBiaoQu[0][2];
				}
				else
				{
					S_TestTaskData->testdata->testline.BasicResult = S_TestTaskData->testdata->testline.BasicBili * S_TestTaskData->testdata->testline.BasicBili;
					S_TestTaskData->testdata->testline.BasicResult *= S_TestTaskData->testdata->temperweima.ItemBiaoQu[1][0];
					
					S_TestTaskData->testdata->testline.BasicResult += (S_TestTaskData->testdata->testline.BasicBili * S_TestTaskData->testdata->temperweima.ItemBiaoQu[1][1]);
					
					S_TestTaskData->testdata->testline.BasicResult += S_TestTaskData->testdata->temperweima.ItemBiaoQu[1][2];
				}
				
				if(S_TestTaskData->testdata->tempadjust.crc == CalModbusCRC16Fun1(&(S_TestTaskData->testdata->tempadjust), sizeof(AdjustData)-2) )
					S_TestTaskData->testdata->testline.AdjustResult =  S_TestTaskData->testdata->testline.BasicResult * S_TestTaskData->testdata->tempadjust.parm;
				else
					S_TestTaskData->testdata->testline.AdjustResult =  S_TestTaskData->testdata->testline.BasicResult;
				
				S_TempCalData->resultstatues = ResultIsOK;
			}
			else
			{
				S_TempCalData->resultstatues = PeakNumError;
				return;
			}
#else
			if(S_TempCalData->peaknum == 1)
			{
				S_TestTaskData->testdata->testline.T_Point[0] = 0;
				S_TestTaskData->testdata->testline.T_Point[1] = S_TestTaskData->testdata->temperweima.ItemLocation;
			}
			else
			{
				S_TestTaskData->testdata->testline.T_Point[0] = S_TempCalData->peakdata[S_TempCalData->peaknum-2].PeakValue;
				S_TestTaskData->testdata->testline.T_Point[1] = S_TempCalData->peakdata[S_TempCalData->peaknum-2].PeakLocation;
			}
			
			S_TestTaskData->testdata->testline.C_Point[0] = S_TempCalData->peakdata[S_TempCalData->peaknum-1].PeakValue;
			S_TestTaskData->testdata->testline.C_Point[1] = S_TempCalData->peakdata[S_TempCalData->peaknum-1].PeakLocation;
			
			S_TestTaskData->testdata->testline.B_Point[0] = 0xffff;
			for(i=S_TestTaskData->testdata->testline.T_Point[1]; i<S_TestTaskData->testdata->testline.C_Point[1]; i++)
			{
				if(S_TestTaskData->testdata->testline.B_Point[0] > S_TestTaskData->testdata->testline.TestPoint[i])
				{
					S_TestTaskData->testdata->testline.B_Point[0] = S_TestTaskData->testdata->testline.TestPoint[i];
					S_TestTaskData->testdata->testline.B_Point[1] = i;
				}
			}
			
			if(S_TempCalData->peaknum == 1)
			{
				S_TestTaskData->testdata->testline.T_Point[0] = S_TestTaskData->testdata->testline.B_Point[0];
				S_TestTaskData->testdata->testline.T_Point[1] = S_TestTaskData->testdata->testline.B_Point[1];
			}
			/*������*/
			if(((S_TestTaskData->testdata->testline.T_Point[0] < S_TestTaskData->testdata->testline.B_Point[0])) || (S_TestTaskData->testdata->testline.C_Point[0] <= S_TestTaskData->testdata->testline.B_Point[0]))
			{
				S_TempCalData->resultstatues = PeakNumError;
				return;
			}
			
			S_TempCalData->tempvalue2 = (S_TestTaskData->testdata->testline.T_Point[0] - S_TestTaskData->testdata->testline.B_Point[0]);
			S_TempCalData->tempvalue2 /= (S_TestTaskData->testdata->testline.C_Point[0] - S_TestTaskData->testdata->testline.B_Point[0]);
				
			/*ԭʼ��߱�*/
			S_TestTaskData->testdata->testline.BasicBili = S_TempCalData->tempvalue2;
			
			/*���ݷֶΣ�����ԭʼ���*/
			
			if((S_TestTaskData->testdata->testline.BasicBili < S_TestTaskData->testdata->temperweima.ItemFenDuan) || (S_TestTaskData->testdata->temperweima.ItemBiaoQuNum == 1))
			{
				S_TestTaskData->testdata->testline.BasicResult = S_TestTaskData->testdata->testline.BasicBili * S_TestTaskData->testdata->testline.BasicBili;
				S_TestTaskData->testdata->testline.BasicResult *= S_TestTaskData->testdata->temperweima.ItemBiaoQu[0][0];
						
				S_TestTaskData->testdata->testline.BasicResult += (S_TestTaskData->testdata->testline.BasicBili * S_TestTaskData->testdata->temperweima.ItemBiaoQu[0][1]);
						
				S_TestTaskData->testdata->testline.BasicResult += S_TestTaskData->testdata->temperweima.ItemBiaoQu[0][2];
			}
			else
			{
				S_TestTaskData->testdata->testline.BasicResult = S_TestTaskData->testdata->testline.BasicBili * S_TestTaskData->testdata->testline.BasicBili;
				S_TestTaskData->testdata->testline.BasicResult *= S_TestTaskData->testdata->temperweima.ItemBiaoQu[1][0];
						
				S_TestTaskData->testdata->testline.BasicResult += (S_TestTaskData->testdata->testline.BasicBili * S_TestTaskData->testdata->temperweima.ItemBiaoQu[1][1]);
						
				S_TestTaskData->testdata->testline.BasicResult += S_TestTaskData->testdata->temperweima.ItemBiaoQu[1][2];
			}
			
			if(S_TestTaskData->testdata->testline.BasicResult < 0)
				S_TestTaskData->testdata->testline.BasicResult = 0;
			
			if(S_TestTaskData->testdata->tempadjust.crc == CalModbusCRC16Fun1(&(S_TestTaskData->testdata->tempadjust), sizeof(AdjustData)-2) )
				S_TestTaskData->testdata->testline.AdjustResult =  S_TestTaskData->testdata->testline.BasicResult * S_TestTaskData->testdata->tempadjust.parm;
			else
				S_TestTaskData->testdata->testline.AdjustResult =  S_TestTaskData->testdata->testline.BasicResult;
				
			S_TempCalData->resultstatues = ResultIsOK;
#endif
		}
	}		
}

