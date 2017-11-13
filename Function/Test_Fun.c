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
static xQueueHandle xTestCurveQueue = NULL;												//发送测试曲线
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
*Description: 创建测试曲线的队列
*Input: None
*Output: None
*Author: xsx
*Date: 2016年11月29日09:14:43
***************************************************************************************************/
MyState_TypeDef InitTestFunData(void)
{
	/*曲线队列*/
	if(xTestCurveQueue == NULL)
		xTestCurveQueue = xQueueCreate( 150, ( unsigned portBASE_TYPE ) sizeof( unsigned short ) );
	
	if(xTestCurveQueue == NULL)
		return My_Fail;
	else
		return My_Pass;
}

/***************************************************************************************************
*FunctionName: SendTestPointData
*Description: 发送测试曲线点到队列中，曲线点包含y坐标信息
*Input: data -- 数据地址
*Output: None
*Return: 发送成功返回my_pass
*			发送超时，或者队列满，返回my_fail
*Author: xsx
*Date: 2016年11月29日09:18:33
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
*Description: 从测试曲线队列中读取数据
*Input: 读取数据缓存地址
*Output: None
*Return: 	My_Pass -- 成功读取到数据
			My_Fail -- 读取失败
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
	unsigned short i = 0;
	unsigned short index;
	TempCalData * S_TempCalData = NULL;															//测试过程中使用的变量
	ResultState S_ResultState = NoResult;
	
	if(parm == NULL)
		return MemError;
	
	/*初始化测试曲线队列*/
	if(InitTestFunData() == My_Fail)
		return MemError;
	while(My_Pass == TakeTestPointData(&i))
		;
	
	S_TempCalData = MyMalloc(sizeof(TempCalData));
	
	if(S_TempCalData)
	{
		memset(S_TempCalData, 0, sizeof(TempCalData));
		//测试数据指针指向传进来的真实数据空间
		S_TempCalData->itemData = parm;
		
		//初始配置
		SetGB_LedValue(S_TempCalData->itemData->ledLight);
		vTaskDelay(10/portTICK_RATE_MS);
		
		SetGB_CLineValue(0);
		vTaskDelay(10/portTICK_RATE_MS);
		
		SelectChannel(S_TempCalData->itemData->testdata.temperweima.ChannelNum);
		vTaskDelay(10/portTICK_RATE_MS);
		
		SMBUS_SCK_L();
		
		repeat:
				
			MotorMoveTo(1, 2, 0, true);
				
			MotorMoveTo(1, 2, StartTestLocation, true);
		
			S_TempCalData->resultstatues = NoResult;
			S_TempCalData->tempvalue1 = 0;
			
			MotorMoveTo(10, 20, EndTestLocation, false);
			
			for(i=1; i<= steps; i++)
			{
				vTaskDelay(1 / portTICK_RATE_MS);				
				S_TempCalData->tempvalue1 += ADS8325();
				
				//平均值滤波
				if(i%AvregeNum == 0)
				{
					index = i/AvregeNum;
					
					S_TempCalData->tempvalue1 /= AvregeNum;

					S_TempCalData->tempSeries[index-1] = (unsigned short)(S_TempCalData->tempvalue1);
					S_TempCalData->itemData->testdata.testline.TestPoint[index - 1] = S_TempCalData->tempSeries[index-1];
							
					SendTestPointData(&(S_TempCalData->itemData->testdata.testline.TestPoint[index - 1]));
						
					S_TempCalData->tempvalue1 = 0;
				}
			}
			
			//分析曲线
			AnalysisTestData(S_TempCalData);
			
			if(S_TempCalData->resultstatues == NoResult)
			{
				//发送一个特定数据，清除曲线
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
		//计算最大值,平均值
		S_TempCalData->maxdata = S_TempCalData->itemData->testdata.testline.TestPoint[0];
		S_TempCalData->tempvalue1 = 0;
		for(i=0; i<MaxPointLen; i++)
		{
			if(S_TempCalData->maxdata < S_TempCalData->itemData->testdata.testline.TestPoint[i])
				S_TempCalData->maxdata = S_TempCalData->itemData->testdata.testline.TestPoint[i];
			
			S_TempCalData->tempvalue1 += S_TempCalData->itemData->testdata.testline.TestPoint[i];
		}
		
		/*判断测试值是否饱和*/
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
		
		//step.1 计算整条曲线的cv值，用于判断是否加样
		S_TempCalData->tempCV = calculateDataCV(S_TempCalData->itemData->testdata.testline.TestPoint, 300, S_TempCalData->tempvalue1);
		if(S_TempCalData->tempCV < 0.025)
			goto END1;
		
		//step.2 find T
		findFeng(S_TempCalData->itemData->testdata.testline.TestPoint, S_TempCalData->itemData->testdata.temperweima.ItemLocation-30, 
			S_TempCalData->itemData->testdata.temperweima.ItemLocation, S_TempCalData->itemData->testdata.temperweima.ItemLocation+30,
			&S_TempCalData->itemData->testdata.testline.T_Point);
		
		//can not find T line
		if(S_TempCalData->itemData->testdata.testline.T_Point.y == 0)
		{
			S_TempCalData->itemData->testdata.testline.T_Point.x = S_TempCalData->itemData->testdata.temperweima.ItemLocation;
		
			S_TempCalData->CV_T = calculateDataCV(&S_TempCalData->itemData->testdata.testline.TestPoint[S_TempCalData->itemData->testdata.testline.T_Point.x-15], 31, 0);

			if(S_TempCalData->CV_T > 0.05)
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
					S_TempCalData->CV_T = calculateDataCV(&S_TempCalData->itemData->testdata.testline.TestPoint[S_TempCalData->itemData->testdata.testline.T_Point.x], S_TempCalData->tempvalue3, 0);
		        	
			        if(S_TempCalData->CV_T < 0.01)
			        {
			        	S_TempCalData->itemData->testdata.testline.T_Point.x = S_TempCalData->itemData->testdata.temperweima.ItemLocation;
			        }
		        }
			}
		}
		
		S_TempCalData->itemData->testdata.testline.T_Point.y = S_TempCalData->itemData->testdata.testline.TestPoint[S_TempCalData->itemData->testdata.testline.T_Point.x];

		
		//step.3 找C线
		findFeng(S_TempCalData->itemData->testdata.testline.TestPoint, S_TempCalData->itemData->testdata.temperweima.CLineLocation-30, 
			S_TempCalData->itemData->testdata.temperweima.CLineLocation, S_TempCalData->itemData->testdata.temperweima.CLineLocation+30,
			&S_TempCalData->itemData->testdata.testline.C_Point);
		if(S_TempCalData->itemData->testdata.testline.C_Point.y == 0)
			S_TempCalData->itemData->testdata.testline.C_Point.x = S_TempCalData->itemData->testdata.temperweima.CLineLocation;

		S_TempCalData->tempvalue3 = S_TempCalData->itemData->testdata.testline.C_Point.x-15;
		if(S_TempCalData->itemData->testdata.testline.C_Point.x < 284)
			S_TempCalData->CV_C = calculateDataCV(&S_TempCalData->itemData->testdata.testline.TestPoint[S_TempCalData->tempvalue3], 31, 0);
		else
			S_TempCalData->CV_C = calculateDataCV(&S_TempCalData->itemData->testdata.testline.TestPoint[S_TempCalData->tempvalue3], (300 - S_TempCalData->tempvalue3), 0);
		
		if((S_TempCalData->CV_C > 0.03) && (S_TempCalData->itemData->testdata.testline.C_Point.y == 0))
		{
			goto END2;
		}
		
		if(S_TempCalData->CV_C < 0.03)
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
		S_TempCalData->CV_T = calculateDataCV(&S_TempCalData->itemData->testdata.testline.TestPoint[S_TempCalData->itemData->testdata.testline.T_Point.x-15], 31, 0);
		
		if((S_TempCalData->CV_C + S_TempCalData->CV_T) < 0.13)
			goto END2;
		
		//step.6 canliu
		S_TempCalData->tempvalue1 = 0;
		S_TempCalData->tempCV = 0;
		for(i=20; i<S_TempCalData->itemData->testdata.testline.T_Point.x-30; i++)
		{
			S_TempCalData->tempvalue1 += S_TempCalData->itemData->testdata.testline.TestPoint[i];
			if(i >= 35)
			{
				S_TempCalData->CV_0 = calculateDataCV(&S_TempCalData->itemData->testdata.testline.TestPoint[20], i-20+1, S_TempCalData->tempvalue1);
				if(S_TempCalData->tempCV < S_TempCalData->CV_0)
					S_TempCalData->tempCV = S_TempCalData->CV_0;
			}
		}
		if(S_TempCalData->tempCV > 0.15)
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
		if(S_TempCalData->itemData->testdata.testline.B_Point.x < S_TempCalData->itemData->testdata.testline.T_Point.x)
			goto END2;
		
		//step.7 b line value : c > b, t >= b
		if((S_TempCalData->itemData->testdata.testline.B_Point.y >= S_TempCalData->itemData->testdata.testline.C_Point.y)
			|| (S_TempCalData->itemData->testdata.testline.B_Point.y > S_TempCalData->itemData->testdata.testline.T_Point.y))
		{
		     goto END2;
		}
		
		//step 9 判断微球是否过少，，补偿
		S_TempCalData->tempvalue1 = 0;
		if(S_TempCalData->CV_T < 0.02 && (S_TempCalData->CV_C + S_TempCalData->CV_T) < 0.3)
		{
			S_TempCalData->tempvalue1 = 1.1;

			for(i=0; i<20; i++)
			{
				S_TempCalData->tempvalue2 = S_TempCalData->tempvalue1;
				S_TempCalData->tempvalue2 *= S_TempCalData->itemData->testdata.testline.TestPoint[S_TempCalData->itemData->testdata.testline.C_Point.x];
				S_TempCalData->itemData->testdata.testline.C_Point.y = (unsigned short)S_TempCalData->tempvalue2;
				S_TempCalData->tempSeries[S_TempCalData->itemData->testdata.testline.C_Point.x] = S_TempCalData->itemData->testdata.testline.C_Point.y;
				
				S_TempCalData->tempvalue3 = S_TempCalData->itemData->testdata.testline.C_Point.x-15;
				if(S_TempCalData->itemData->testdata.testline.C_Point.x < 284)
					S_TempCalData->CV_C = calculateDataCV(&S_TempCalData->tempSeries[S_TempCalData->tempvalue3], 31, 0);
				else
					S_TempCalData->CV_C = calculateDataCV(&S_TempCalData->tempSeries[S_TempCalData->tempvalue3], (300 - S_TempCalData->tempvalue3), 0);
				
				if((S_TempCalData->CV_C + S_TempCalData->CV_T) >= 0.3)
					break;
				
				S_TempCalData->tempvalue1 += 0.1;
			}
		}
		
		S_TempCalData->itemData->testdata.testline.CMdifyNum = (unsigned char)(S_TempCalData->tempvalue1 * 10);
		/*计算结果*/
		S_TempCalData->tempvalue1 = (S_TempCalData->itemData->testdata.testline.T_Point.y - S_TempCalData->itemData->testdata.testline.B_Point.y);
		S_TempCalData->tempvalue2 = (S_TempCalData->itemData->testdata.testline.C_Point.y - S_TempCalData->itemData->testdata.testline.B_Point.y);
		S_TempCalData->itemData->testdata.testline.C_Point.y = S_TempCalData->itemData->testdata.testline.TestPoint[S_TempCalData->itemData->testdata.testline.C_Point.x];

		/*原始峰高比*/
		S_TempCalData->itemData->testdata.testline.BasicBili = S_TempCalData->tempvalue1 / S_TempCalData->tempvalue2;
				
		/*根据分段，计算原始结果*/
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
		else if(true == CheckStrIsSame(S_TempCalData->itemData->testdata.temperweima.PiHao, "IT1708-01", 9))
		{
			S_TempCalData->itemData->testdata.testline.BasicResult /= 1.75;
		}
		else if(true == CheckStrIsSame(S_TempCalData->itemData->testdata.temperweima.PiHao, "IM1705-01", 9))
		{
			S_TempCalData->itemData->testdata.testline.BasicResult /= 5.0f;
		}
		
		S_TempCalData->resultstatues = ResultIsOK;
		
		return;
		
		END1:
			S_TempCalData->resultstatues = NoSample;
			S_TempCalData->itemData->testdata.testline.BasicResult = 0;
			return;
		
		END2:
			S_TempCalData->resultstatues = PeakError;
			S_TempCalData->itemData->testdata.testline.BasicResult = 0;
			return;
	}		
}

