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
#include	"BackDoorData.h"

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

			S_TempCalData->maxdata = 0;
			
			for(i=1; i<= steps; i++)
			{
				MotorMoveTo(GetGB_MotorLocation() + 1, 0);
						
				S_TempCalData->tempvalue2 += ADS8325();
				
				if(i%AvregeNum == 0)
				{
					S_TempCalData->tempvalue2 /= AvregeNum;
					
					S_TempCalData->tempvalue = (unsigned short)(S_TempCalData->tempvalue2);
					
					if(S_TempCalData->maxdata < S_TempCalData->tempvalue)
						S_TempCalData->maxdata = S_TempCalData->tempvalue;
					
					if(S_TempCalData->maxdata >= 4000)
					{
						if(GetChannel() > 0)
						{
							SelectChannel(GetChannel() - 1);
							goto END1;
						}
					}
					S_TestTaskData->testdata->testline.TestPoint[i/AvregeNum-1] = S_TempCalData->tempvalue;
							
					//SendTestPointData(&(S_TestTaskData->testdata->testline.TestPoint[i/AvregeNum-1]));
						
					S_TempCalData->tempvalue2 = 0;
				}
				vTaskDelay(1 / portTICK_RATE_MS);
				
/*				if(GetCardState() == NoCard)				//�����γ�
				{
					S_TempCalData->resultstatues = TestInterrupt;
					goto END;
				}*/
			}
			
			vTaskDelay(100/portTICK_RATE_MS);
			
			//��������
			AnalysisTestData();
			
			goto END;
			
			END1:
				vTaskDelay(100/portTICK_RATE_MS);
				//����һ���ض����ݣ��������
				S_TestTaskData->testdata->testline.TestPoint[0] = 0xffff;
				//SendTestPointData(&(S_TestTaskData->testdata->testline.TestPoint[0]));
				goto repeat;
			
			END:
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
	unsigned short i=0, j=0;
	
	{
		memset(&(S_TempCalData->peakdata[0]), 0, 2*sizeof(PeakData));
		
		/*��T��*/
		if(S_TestTaskData->testdata->temperweima.ItemLocation < 40)
		{
			S_TempCalData->resultstatues = LineLocationError;
			return;
		}
		S_TestTaskData->testdata->testline.T_Point[0] = 0;
		for(i=(S_TestTaskData->testdata->temperweima.ItemLocation - 40); i<(S_TestTaskData->testdata->temperweima.ItemLocation + 40); i++)
		{
			if(S_TestTaskData->testdata->testline.T_Point[0] < S_TestTaskData->testdata->testline.TestPoint[i])
			{
				S_TestTaskData->testdata->testline.T_Point[0] = S_TestTaskData->testdata->testline.TestPoint[i];
				S_TestTaskData->testdata->testline.T_Point[1] = i;
				
				S_TempCalData->peakdata[0].PeakLocation = i;
				S_TempCalData->peakdata[0].PeakValue = S_TestTaskData->testdata->testline.T_Point[0];
			}
		}
		
		/*��C��*/
		if(S_TestTaskData->testdata->temperweima.CLineLocation < 40)
		{
			S_TempCalData->resultstatues = LineLocationError;
			return;
		}
		S_TestTaskData->testdata->testline.C_Point[0] = 0;
		for(i=(S_TestTaskData->testdata->temperweima.CLineLocation - 40); i<(S_TestTaskData->testdata->temperweima.CLineLocation + 40); i++)
		{
			if(S_TestTaskData->testdata->testline.C_Point[0] < S_TestTaskData->testdata->testline.TestPoint[i])
			{
				S_TestTaskData->testdata->testline.C_Point[0] = S_TestTaskData->testdata->testline.TestPoint[i];
				S_TestTaskData->testdata->testline.C_Point[1] = i;
				
				S_TempCalData->peakdata[1].PeakLocation = i;
				S_TempCalData->peakdata[1].PeakValue = S_TestTaskData->testdata->testline.C_Point[0];
			}
		}
		
		/*�һ���*/
		if(S_TestTaskData->testdata->testline.T_Point[1] >= S_TestTaskData->testdata->testline.C_Point[1])
		{
			S_TempCalData->resultstatues = LineLocationError;
			return;
		}
		S_TestTaskData->testdata->testline.B_Point[0] = 0xffff;
		for(i=S_TestTaskData->testdata->testline.T_Point[1]; i<S_TestTaskData->testdata->testline.C_Point[1]; i++)
		{
			if(S_TestTaskData->testdata->testline.B_Point[0] > S_TestTaskData->testdata->testline.TestPoint[i])
			{
				S_TestTaskData->testdata->testline.B_Point[0] = S_TestTaskData->testdata->testline.TestPoint[i];
				S_TestTaskData->testdata->testline.B_Point[1] = i;
			}
		}
		
		/*��T���ǰ���*/
		for(j=0; j<S_TestTaskData->testdata->testline.T_Point[1]; j++)
		{
			if(S_TestTaskData->testdata->testline.TestPoint[S_TestTaskData->testdata->testline.T_Point[1]-j] == S_TestTaskData->testdata->testline.T_Point[0])
				continue;
			
			S_TempCalData->tempvalue2 = S_TestTaskData->testdata->testline.TestPoint[S_TestTaskData->testdata->testline.T_Point[1]-j] - 
				S_TestTaskData->testdata->testline.TestPoint[S_TestTaskData->testdata->testline.T_Point[1]-j-1];
			if(S_TempCalData->tempvalue2 < 0)
			{
				S_TempCalData->peakdata[0].StartLocation = S_TestTaskData->testdata->testline.T_Point[1]-j;
				S_TempCalData->peakdata[0].StartValue = S_TestTaskData->testdata->testline.TestPoint[S_TempCalData->peakdata[0].StartLocation];
				break;
			}
		}
		
		/*��T��ĺ���*/
		for(j=S_TestTaskData->testdata->testline.T_Point[1]; j<MaxPointLen-1; j++)
		{
			if(S_TestTaskData->testdata->testline.TestPoint[j] == S_TestTaskData->testdata->testline.T_Point[0])
				continue;
			
			S_TempCalData->tempvalue2 = S_TestTaskData->testdata->testline.TestPoint[j] - S_TestTaskData->testdata->testline.TestPoint[j+1];
			if(S_TempCalData->tempvalue2 < 0)
			{
				S_TempCalData->peakdata[0].EndLocation = j;
				S_TempCalData->peakdata[0].EndValue = S_TestTaskData->testdata->testline.TestPoint[S_TempCalData->peakdata[0].EndLocation];
				break;
			}
		}
		
		S_TempCalData->peakdata[0].UpWidth = S_TempCalData->peakdata[0].PeakLocation - S_TempCalData->peakdata[0].StartLocation;
		S_TempCalData->peakdata[0].DownWidth = S_TempCalData->peakdata[0].EndLocation - S_TempCalData->peakdata[0].PeakLocation;
		S_TempCalData->peakdata[0].PeakWidth = S_TempCalData->peakdata[0].EndLocation - S_TempCalData->peakdata[0].StartLocation;
		S_TempCalData->peakdata[0].PeakScale = S_TempCalData->peakdata[0].UpWidth;
		S_TempCalData->peakdata[0].PeakScale /= S_TempCalData->peakdata[0].DownWidth;
		/*һ�������꣬�����������ж�*/
		/*�����Ŀ��С��30���ߴ���100����˷�Ϊ��*/
		if((S_TempCalData->peakdata[0].PeakWidth < 30)&&(S_TempCalData->peakdata[0].PeakWidth > 150))
		{
			S_TempCalData->peakdata[0].status = 1;			
		}
		
		/*��C���ǰ���*/
		for(j=0; j<S_TestTaskData->testdata->testline.C_Point[1]; j++)
		{
			if(S_TestTaskData->testdata->testline.TestPoint[S_TestTaskData->testdata->testline.C_Point[1]-j] == S_TestTaskData->testdata->testline.C_Point[0])
				continue;
			
			S_TempCalData->tempvalue2 = S_TestTaskData->testdata->testline.TestPoint[S_TestTaskData->testdata->testline.C_Point[1]-j] - 
				S_TestTaskData->testdata->testline.TestPoint[S_TestTaskData->testdata->testline.C_Point[1]-j-1];
			if(S_TempCalData->tempvalue2 < 0)
			{
		
				S_TempCalData->peakdata[1].StartLocation = S_TestTaskData->testdata->testline.C_Point[1]-j;
				S_TempCalData->peakdata[1].StartValue = S_TestTaskData->testdata->testline.TestPoint[S_TempCalData->peakdata[1].StartLocation];
				break;
			}
		}
		
		/*��C��ĺ���*/
		for(j=S_TestTaskData->testdata->testline.C_Point[1]; j<MaxPointLen-1; j++)
		{
			if(S_TestTaskData->testdata->testline.TestPoint[j] == S_TestTaskData->testdata->testline.C_Point[0])
				continue;
			
			S_TempCalData->tempvalue2 = S_TestTaskData->testdata->testline.TestPoint[j] - S_TestTaskData->testdata->testline.TestPoint[j+1];
			if(S_TempCalData->tempvalue2 < 0)
			{
				S_TempCalData->peakdata[1].EndLocation = j;
				S_TempCalData->peakdata[1].EndValue = S_TestTaskData->testdata->testline.TestPoint[S_TempCalData->peakdata[1].EndLocation];
				break;
			}
		}
		
		S_TempCalData->peakdata[1].UpWidth = S_TempCalData->peakdata[1].PeakLocation - S_TempCalData->peakdata[1].StartLocation;
		S_TempCalData->peakdata[1].DownWidth = S_TempCalData->peakdata[1].EndLocation - S_TempCalData->peakdata[1].PeakLocation;
		S_TempCalData->peakdata[1].PeakWidth = S_TempCalData->peakdata[1].EndLocation - S_TempCalData->peakdata[1].StartLocation;
		S_TempCalData->peakdata[1].PeakScale = S_TempCalData->peakdata[1].UpWidth;
		S_TempCalData->peakdata[1].PeakScale /= S_TempCalData->peakdata[1].DownWidth;
		/*һ�������꣬�����������ж�*/
		/*�����Ŀ��С��30���ߴ���100����˷�Ϊ��*/
		if((S_TempCalData->peakdata[1].PeakWidth < 30)&&(S_TempCalData->peakdata[1].PeakWidth > 150))
		{
			S_TempCalData->peakdata[1].status = 1;			
		}
		
		/*���û�з�*/
		if((S_TempCalData->peakdata[0].status == 1) && (S_TempCalData->peakdata[1].status == 1))
		{
			S_TempCalData->resultstatues = PeakNumZero;
			return;
		}
		/*���C�������ο��쳣*/
		else if(S_TempCalData->peakdata[1].status == 1)
		{
			S_TempCalData->resultstatues = PeakNumError;
			return;
		}
		/*�������ȷ�����ݶ�ά���ҷ�*/
		else
		{
			//���T�岻���ڣ���ʹ����ڻ���
			if(S_TempCalData->peakdata[0].status == 1)
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
			
			/*������*/
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
				
			if(S_TestTaskData->testdata->tempadjust.crc == CalModbusCRC16Fun1(&(S_TestTaskData->testdata->tempadjust), sizeof(AdjustData)-2) )
				S_TestTaskData->testdata->testline.AdjustResult =  S_TestTaskData->testdata->testline.BasicResult * S_TestTaskData->testdata->tempadjust.parm;
			else
				S_TestTaskData->testdata->testline.AdjustResult =  S_TestTaskData->testdata->testline.BasicResult;
			
			S_TestTaskData->testdata->testline.AdjustResult = GetCurrentData();
			
			S_TempCalData->resultstatues = ResultIsOK;

		}
	}		
}

