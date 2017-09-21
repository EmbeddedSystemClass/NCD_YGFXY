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
#include 	"MLX90614_Driver.h"

#include	"MyMem.h"

#include	"QueueUnits.h"
#include	"ServerFun.h"
#include	"MyTools.h"

#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"
#include	"stdlib.h"
#include	"math.h"
/***************************************************************************************************/
/**************************************??????*************************************************/
/***************************************************************************************************/
static xQueueHandle xTestCurveQueue = NULL;												//���Ͳ�������
/***************************************************************************************************/
/**************************************??????*************************************************/
/***************************************************************************************************/
static MyState_TypeDef SendTestPointData(void * data);
static void AnalysisTestData(TempCalData * S_TempCalData);
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************??********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************
*FunctionName: InitTestFunData
*Description: �����������ߵĶ���
*Input: None
*Output: None
*Author: xsx
*Date: 2016��11��29��09:14:43
***************************************************************************************************/
MyState_TypeDef InitTestFunData(void)
{
	/*���߶���*/
	if(xTestCurveQueue == NULL)
		xTestCurveQueue = xQueueCreate( 310, ( unsigned portBASE_TYPE ) sizeof( unsigned short ) );
	
	if(xTestCurveQueue == NULL)
		return My_Fail;
	else
		return My_Pass;
}

/***************************************************************************************************
*FunctionName: SendTestPointData
*Description: ���Ͳ������ߵ㵽�����У����ߵ����y������Ϣ
*Input: data -- ���ݵ�ַ
*Output: None
*Return: ���ͳɹ�����my_pass
*			���ͳ�ʱ�����߶�����������my_fail
*Author: xsx
*Date: 2016��11��29��09:18:33
***************************************************************************************************/
static MyState_TypeDef SendTestPointData(void * data)
{
	if(xTestCurveQueue == NULL)
		return My_Fail;
	
	if(pdPASS == xQueueSend( xTestCurveQueue, data, 1/portTICK_RATE_MS ))
		return My_Pass;
	else
		return My_Fail;	
}

/***************************************************************************************************
*FunctionName: TakeTestPointData
*Description: �Ӳ������߶����ж�ȡ����
*Input: ��ȡ���ݻ����ַ
*Output: None
*Return: 	My_Pass -- �ɹ���ȡ������
			My_Fail -- ��ȡʧ��
*Author: xsx
*Date: 
***************************************************************************************************/
MyState_TypeDef TakeTestPointData(void * data)
{
	if(xTestCurveQueue == NULL)
		return My_Fail;
	
	if(pdPASS == xQueueReceive( xTestCurveQueue, data, 0*portTICK_RATE_MS))
		return My_Pass;
	else
		return My_Fail;	
}

/***************************************************************************************************
*FunctionName: 
*Description: 
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 
***************************************************************************************************/
ResultState TestFunction(void * parm)
{
	unsigned short steps = EndTestLocation - StartTestLocation;
	unsigned short i = 0, j=0;
	unsigned short index;
	TempCalData * S_TempCalData = NULL;															//���Թ�����ʹ�õı���
	ResultState S_ResultState = NoResult;
	
	if(parm == NULL)
		return MemError;
	
	/*��ʼ���������߶���*/
	if(InitTestFunData() == My_Fail)
		return MemError;
	while(My_Pass == TakeTestPointData(&i))
		;
	
	S_TempCalData = MyMalloc(sizeof(TempCalData));
	
	if(S_TempCalData)
	{
		memset(S_TempCalData, 0, sizeof(TempCalData));
		//��������ָ��ָ�򴫽�������ʵ���ݿռ�
		S_TempCalData->itemData = parm;
		
		//��ʼ����
		SetGB_LedValue(S_TempCalData->itemData->ledLight);
		vTaskDelay(10/portTICK_RATE_MS);
		
		SetGB_CLineValue(0);
		vTaskDelay(10/portTICK_RATE_MS);
		
		SelectChannel(S_TempCalData->itemData->testdata.temperweima.ChannelNum);
		vTaskDelay(10/portTICK_RATE_MS);
		
		SMBUS_SCK_L();
		
		repeat:
				
			MotorMoveTo(0, 0);
				
			MotorMoveTo(StartTestLocation, 0);
		
			S_TempCalData->resultstatues = NoResult;
			S_TempCalData->tempvalue1 = 0;
			
			for(i=1; i<= steps; i++)
			{
				MotorMoveTo(GetGB_MotorLocation() + 1, 0);
				
				vTaskDelay(1 / portTICK_RATE_MS);				
				S_TempCalData->tempvalue1 += ADS8325();
				
				//ƽ��ֵ�˲�
				if(i%AvregeNum == 0)
				{
					index = i/AvregeNum;
					
					S_TempCalData->tempvalue1 /= AvregeNum;
					
					
					S_TempCalData->temptestline[index-1] = (unsigned short)(S_TempCalData->tempvalue1);


					//ƽ���˲�
					if(index >= FilterNum)
					{
						S_TempCalData->tempvalue2 = 0;
						for(j=index-FilterNum; j<index; j++)
						{
							S_TempCalData->tempvalue2 += S_TempCalData->temptestline[j];
						}
						
						S_TempCalData->tempvalue2 /= FilterNum;
						
						S_TempCalData->itemData->testdata.testline.TestPoint[index - FilterNum] = S_TempCalData->tempvalue2;
							
						SendTestPointData(&(S_TempCalData->itemData->testdata.testline.TestPoint[index - FilterNum]));
					}
						
					S_TempCalData->tempvalue1 = 0;
				}
			}
			
			//��������
			AnalysisTestData(S_TempCalData);
			
			if(S_TempCalData->resultstatues == NoResult)
			{
				//����һ���ض����ݣ��������
				S_TempCalData->itemData->testdata.testline.TestPoint[0] = 0xffff;
				SendTestPointData(&(S_TempCalData->itemData->testdata.testline.TestPoint[0]));
				goto repeat;
			}

			S_ResultState = S_TempCalData->resultstatues;
			
			MyFree(S_TempCalData);
				
			SMBUS_SCK_H();
			
			SetGB_LedValue(0);
			
			vTaskDelay(1500/portTICK_RATE_MS);
			
			return S_ResultState;
	}
	else
		return MemError;	
}




static void AnalysisTestData(TempCalData * S_TempCalData)
{
	unsigned short i=0;
	
	{
		//�������ֵ,ƽ��ֵ
		S_TempCalData->maxdata = S_TempCalData->itemData->testdata.testline.TestPoint[0];
		S_TempCalData->tempvalue1 = 0;
		for(i=0; i<MaxPointLen; i++)
		{
			if(S_TempCalData->maxdata < S_TempCalData->itemData->testdata.testline.TestPoint[i])
				S_TempCalData->maxdata = S_TempCalData->itemData->testdata.testline.TestPoint[i];
			
			S_TempCalData->tempvalue1 += S_TempCalData->itemData->testdata.testline.TestPoint[i];
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
		else if(S_TempCalData->maxdata < 50)
		{
			if(GetChannel() < 7)
			{
				SelectChannel(GetChannel() + 3);
				
				vTaskDelay(10/portTICK_RATE_MS);
				return;
			}
		}
		
		//step.1 �����������ߵ�cvֵ�������ж��Ƿ����
		S_TempCalData->CV_1 = calculateDataCV(S_TempCalData->itemData->testdata.testline.TestPoint, 300, S_TempCalData->tempvalue1);
		if(S_TempCalData->CV_1 < 0.025)
			goto END1;
		
		//step.2 find T
		findFeng(S_TempCalData->itemData->testdata.testline.TestPoint, S_TempCalData->itemData->testdata.temperweima.ItemLocation-30, 
			S_TempCalData->itemData->testdata.temperweima.ItemLocation, S_TempCalData->itemData->testdata.temperweima.ItemLocation+30,
			&S_TempCalData->itemData->testdata.testline.T_Point);
		
		//can not find T line
		if(S_TempCalData->itemData->testdata.testline.T_Point.y == 0)
		{
			S_TempCalData->itemData->testdata.testline.T_Point.x = S_TempCalData->itemData->testdata.temperweima.ItemLocation;
		
			S_TempCalData->CV_1 = calculateDataCV(&S_TempCalData->itemData->testdata.testline.TestPoint[S_TempCalData->itemData->testdata.testline.T_Point.x-15], 31, 0);

			if(S_TempCalData->CV_1 > 0.05)
			{
				goto END2;
			}
		}
		else
		{
			if((S_TempCalData->itemData->testdata.testline.T_Point.x >= 200) || (S_TempCalData->itemData->testdata.testline.T_Point.x <= 100))
				goto END2;
			
			if(S_TempCalData->itemData->testdata.testline.T_Point.x < S_TempCalData->itemData->testdata.temperweima.ItemLocation) 
			{
				S_TempCalData->tempvalue3 = S_TempCalData->itemData->testdata.temperweima.ItemLocation - S_TempCalData->itemData->testdata.testline.T_Point.x;
				
				if(S_TempCalData->tempvalue3 >= 10)
		        {
					S_TempCalData->CV_1 = calculateDataCV(&S_TempCalData->itemData->testdata.testline.TestPoint[S_TempCalData->itemData->testdata.testline.T_Point.x], S_TempCalData->tempvalue3, 0);
		        	
			        if(S_TempCalData->CV_1 < 0.01)
			        {
			        	S_TempCalData->itemData->testdata.testline.T_Point.x = S_TempCalData->itemData->testdata.temperweima.ItemLocation;
			        }
		        }
			}
		}
		
		S_TempCalData->itemData->testdata.testline.T_Point.y = S_TempCalData->itemData->testdata.testline.TestPoint[S_TempCalData->itemData->testdata.testline.T_Point.x];

		
		//step.3 ��C��
		findFeng(S_TempCalData->itemData->testdata.testline.TestPoint, S_TempCalData->itemData->testdata.temperweima.CLineLocation-30, 
			S_TempCalData->itemData->testdata.temperweima.CLineLocation, S_TempCalData->itemData->testdata.temperweima.CLineLocation+30,
			&S_TempCalData->itemData->testdata.testline.C_Point);
		if(S_TempCalData->itemData->testdata.testline.C_Point.y == 0)
			S_TempCalData->itemData->testdata.testline.C_Point.x = S_TempCalData->itemData->testdata.temperweima.CLineLocation;

		S_TempCalData->tempvalue3 = S_TempCalData->itemData->testdata.testline.C_Point.x-15;
		if(S_TempCalData->tempvalue3 < (300 - 31))
			S_TempCalData->CV_1 = calculateDataCV(&S_TempCalData->itemData->testdata.testline.TestPoint[S_TempCalData->itemData->testdata.testline.C_Point.x-15], 31, 0);
		else
			S_TempCalData->CV_1 = calculateDataCV(&S_TempCalData->itemData->testdata.testline.TestPoint[S_TempCalData->itemData->testdata.testline.C_Point.x-15], (300 - 31), 0);
		
		if((S_TempCalData->CV_1 > 0.03) && (S_TempCalData->itemData->testdata.testline.C_Point.y == 0))
		{
			goto END2;
		}
		
		if(S_TempCalData->CV_1 < 0.03)
		{
			goto END2;
		}
		
		S_TempCalData->itemData->testdata.testline.C_Point.y = S_TempCalData->itemData->testdata.testline.TestPoint[S_TempCalData->itemData->testdata.testline.C_Point.x];

		//step.4 c.x - t.x should between in 50-100
		if(S_TempCalData->itemData->testdata.testline.C_Point.x <= S_TempCalData->itemData->testdata.testline.T_Point.x)
			goto END2;
		
		S_TempCalData->tempvalue3 = S_TempCalData->itemData->testdata.testline.C_Point.x - S_TempCalData->itemData->testdata.testline.T_Point.x;
		if((S_TempCalData->tempvalue3 < 50) || (S_TempCalData->tempvalue3 > 100))
			goto END2;
		
		//step.5 c+t cv > 0.2
		S_TempCalData->tempvalue3 = S_TempCalData->itemData->testdata.testline.C_Point.x-15;
		if(S_TempCalData->tempvalue3 < (300 - 31))
			S_TempCalData->CV_1 = calculateDataCV(&S_TempCalData->itemData->testdata.testline.TestPoint[S_TempCalData->tempvalue3], 31, 0);
		else
			S_TempCalData->CV_1 = calculateDataCV(&S_TempCalData->itemData->testdata.testline.TestPoint[S_TempCalData->tempvalue3], (300 - 31), 0);
		
		S_TempCalData->CV_2 = calculateDataCV(&S_TempCalData->itemData->testdata.testline.TestPoint[S_TempCalData->itemData->testdata.testline.T_Point.x-15], 31, 0);
		
		if((S_TempCalData->CV_1 + S_TempCalData->CV_2) < 0.13)
			goto END2;
		
		//step.6 canliu
		S_TempCalData->tempvalue1 = 0;
		S_TempCalData->CV_1 = 0;
		for(i=20; i<S_TempCalData->itemData->testdata.testline.T_Point.x-30; i++)
		{
			S_TempCalData->tempvalue1 += S_TempCalData->itemData->testdata.testline.TestPoint[i];
			if(i >= 35)
			{
				S_TempCalData->CV_2 = calculateDataCV(&S_TempCalData->itemData->testdata.testline.TestPoint[20], i-20+1, S_TempCalData->tempvalue1);
				if(S_TempCalData->CV_1 < S_TempCalData->CV_2)
					S_TempCalData->CV_1 = S_TempCalData->CV_2;
			}
		}
		if(S_TempCalData->CV_1 > 0.15)
			goto END2;
		
		//step.6 find b
		S_TempCalData->itemData->testdata.testline.B_Point.y = 10000;
		S_TempCalData->itemData->testdata.testline.B_Point.x = 0;

		for(i=S_TempCalData->itemData->testdata.testline.T_Point.x; i<S_TempCalData->itemData->testdata.testline.C_Point.x; i++)
		{
			if(S_TempCalData->itemData->testdata.testline.B_Point.y > S_TempCalData->itemData->testdata.testline.TestPoint[i])
			{
				S_TempCalData->itemData->testdata.testline.B_Point.x = i;
				S_TempCalData->itemData->testdata.testline.B_Point.y = S_TempCalData->itemData->testdata.testline.TestPoint[i];
			}
		}
		
		//step.7 b line value : c > b, t >= b
		if((S_TempCalData->itemData->testdata.testline.B_Point.y >= S_TempCalData->itemData->testdata.testline.C_Point.y)
			|| (S_TempCalData->itemData->testdata.testline.B_Point.y > S_TempCalData->itemData->testdata.testline.T_Point.y))
		{
		     goto END2;
		}
				
		/*������*/
		S_TempCalData->tempvalue2 = (S_TempCalData->itemData->testdata.testline.T_Point.y - S_TempCalData->itemData->testdata.testline.B_Point.y);
		S_TempCalData->tempvalue2 /= (S_TempCalData->itemData->testdata.testline.C_Point.y - S_TempCalData->itemData->testdata.testline.B_Point.y);
				
		/*ԭʼ��߱�*/
		S_TempCalData->itemData->testdata.testline.BasicBili = S_TempCalData->tempvalue2;
				
		/*���ݷֶΣ�����ԭʼ���*/
		if((S_TempCalData->itemData->testdata.testline.BasicBili < S_TempCalData->itemData->testdata.temperweima.ItemFenDuan[0]) || (S_TempCalData->itemData->testdata.temperweima.ItemFenDuan[0] == 0))
		{
			S_TempCalData->itemData->testdata.testline.BasicResult = S_TempCalData->itemData->testdata.testline.BasicBili * S_TempCalData->itemData->testdata.testline.BasicBili;
			S_TempCalData->itemData->testdata.testline.BasicResult *= S_TempCalData->itemData->testdata.temperweima.ItemBiaoQu[0][0];
					
			S_TempCalData->itemData->testdata.testline.BasicResult += (S_TempCalData->itemData->testdata.testline.BasicBili * S_TempCalData->itemData->testdata.temperweima.ItemBiaoQu[0][1]);
					
			S_TempCalData->itemData->testdata.testline.BasicResult += S_TempCalData->itemData->testdata.temperweima.ItemBiaoQu[0][2];
		}
		else if((S_TempCalData->itemData->testdata.testline.BasicBili < S_TempCalData->itemData->testdata.temperweima.ItemFenDuan[1]) || (S_TempCalData->itemData->testdata.temperweima.ItemFenDuan[1] == 0))
		{
			S_TempCalData->itemData->testdata.testline.BasicResult = S_TempCalData->itemData->testdata.testline.BasicBili * S_TempCalData->itemData->testdata.testline.BasicBili;
			S_TempCalData->itemData->testdata.testline.BasicResult *= S_TempCalData->itemData->testdata.temperweima.ItemBiaoQu[1][0];
					
			S_TempCalData->itemData->testdata.testline.BasicResult += (S_TempCalData->itemData->testdata.testline.BasicBili * S_TempCalData->itemData->testdata.temperweima.ItemBiaoQu[1][1]);
					
			S_TempCalData->itemData->testdata.testline.BasicResult += S_TempCalData->itemData->testdata.temperweima.ItemBiaoQu[1][2];
		}
		else
		{
			S_TempCalData->itemData->testdata.testline.BasicResult = S_TempCalData->itemData->testdata.testline.BasicBili * S_TempCalData->itemData->testdata.testline.BasicBili;
			S_TempCalData->itemData->testdata.testline.BasicResult *= S_TempCalData->itemData->testdata.temperweima.ItemBiaoQu[2][0];
					
			S_TempCalData->itemData->testdata.testline.BasicResult += (S_TempCalData->itemData->testdata.testline.BasicBili * S_TempCalData->itemData->testdata.temperweima.ItemBiaoQu[2][1]);
					
			S_TempCalData->itemData->testdata.testline.BasicResult += S_TempCalData->itemData->testdata.temperweima.ItemBiaoQu[2][2];
		}

		if(S_TempCalData->itemData->testdata.testline.BasicResult < 0)
			S_TempCalData->itemData->testdata.testline.BasicResult = 0;
		
		if(true == CheckStrIsSame(S_TempCalData->itemData->testdata.temperweima.PiHao, "IT1705-01", 9))
		{
			S_TempCalData->itemData->testdata.testline.BasicResult *= 0.6666;
		}
		else if(true == CheckStrIsSame(S_TempCalData->itemData->testdata.temperweima.PiHao, "IK1705-01", 9))
		{
			S_TempCalData->itemData->testdata.testline.BasicResult /= 2.3;
		}

		S_TempCalData->resultstatues = ResultIsOK;
		S_TempCalData->itemData->testdata.testline.AdjustResult =  S_TempCalData->itemData->testdata.testline.BasicResult * S_TempCalData->itemData->testdata.tempadjust.parm;
		return;
		
		END1:
			S_TempCalData->resultstatues = NoSample;
			S_TempCalData->itemData->testdata.testline.BasicResult = 0;
			S_TempCalData->itemData->testdata.testline.AdjustResult =  0;
			return;
		
		END2:
			S_TempCalData->resultstatues = PeakError;
			S_TempCalData->itemData->testdata.testline.BasicResult = 0;
			S_TempCalData->itemData->testdata.testline.AdjustResult =  0;
			return;
	}		
}

