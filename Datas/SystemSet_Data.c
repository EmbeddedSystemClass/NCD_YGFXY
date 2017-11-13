/***************************************************************************************************
*FileName:	SystemSet_Data
*Description:	系统设置参数
*Author: xsx_kair
*Data:	2016年12月16日16:20:43
***************************************************************************************************/

/***************************************************************************************************/
/******************************************Header List********************************************/
/***************************************************************************************************/
#include	"SystemSet_Data.h"

#include	"MyTools.h"
#include	"CRC16.h"

#include	<string.h>
#include	"stdio.h"
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
static SystemSetData GB_SystemSetData;								//系统参数


static bool isShowRealValue = false;								//是否显示真实数据
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/****************************************File Start*************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************
*FunctionName: setDefaultSystemSetData
*Description: 初始化为默认配置
*Input: systemSetData -- 需要重置的系统设置地址
*Output: 
*Return: 
*Author: xsx
*Date: 2016年12月19日09:17:30
***************************************************************************************************/
void setDefaultSystemSetData(SystemSetData * systemSetData)
{
	if(systemSetData)
	{
		systemSetData->isAutoPrint = true;
		systemSetData->isMute = false;
		systemSetData->ledLightIntensity = 100;
		systemSetData->ledSleepTime = 60;
		
		memset(&(systemSetData->deviceInfo), 0, sizeof(DeviceInfo));
		sprintf(systemSetData->deviceInfo.deviceid, "ncd-device");
		systemSetData->deviceInfo.isnew = true;
		systemSetData->deviceInfo.crc = CalModbusCRC16Fun1(&(systemSetData->deviceInfo), sizeof(DeviceInfo) - 2);
		
		memset(&(systemSetData->netSet), 0, sizeof(NetSet));
		systemSetData->netSet.ipmode = DHCP_Mode;
		systemSetData->netSet.crc = CalModbusCRC16Fun1(&(systemSetData->netSet), sizeof(NetSet) - 2);
		
		systemSetData->testDataNum = 0;
		systemSetData->upLoadIndex = 0;
		
		systemSetData->testLedLightIntensity = 200;
		
		//清空校准参数
		memset(systemSetData->parm1, 0, 700);
		memset(systemSetData->parm2, 0, 156);
		
		systemSetData->crc = CalModbusCRC16Fun1(systemSetData, sizeof(SystemSetData) - 2);
	}
}

/***************************************************************************************************
*FunctionName: copyGBSystemSetData, getGBSystemSetData, setSystemSetData
*Description: 复制一份系统参数,获取系统参数地址
*Input: systemSetData -- 复制的目标地址
*Output: 
*Return: 
*Author: xsx
*Date: 2016年12月16日16:50:13
***************************************************************************************************/
void copyGBSystemSetData(SystemSetData * systemSetData)
{
	if(systemSetData)
	{
		//如果crc错误。恢复默认值
		if(GB_SystemSetData.crc != CalModbusCRC16Fun1(&GB_SystemSetData, sizeof(SystemSetData) - 2))
			setDefaultSystemSetData(&GB_SystemSetData);
		
		memcpy(systemSetData, &GB_SystemSetData, sizeof(SystemSetData));
	}
}

SystemSetData * getGBSystemSetData(void)
{
	return &GB_SystemSetData;
}

void setSystemSetData(SystemSetData * systemSetData)
{
	if(systemSetData)
	{
		systemSetData->crc = CalModbusCRC16Fun1(systemSetData, sizeof(SystemSetData) - 2);
		
		memcpy(&GB_SystemSetData, systemSetData, sizeof(SystemSetData));
	}
}

/***************************************************************************************************
*FunctionName: getdeviceInfo
*Description: 获取设备信息
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016年12月16日16:38:22
***************************************************************************************************/
void getDeviceInfo(DeviceInfo * deviceinfo)
{
	if(deviceinfo)
	{
		//如果crc错误。恢复默认值
		if(GB_SystemSetData.crc != CalModbusCRC16Fun1(&GB_SystemSetData, sizeof(SystemSetData) - 2))
			setDefaultSystemSetData(&GB_SystemSetData);
		
		memcpy(deviceinfo, &(GB_SystemSetData.deviceInfo), sizeof(DeviceInfo));

	}
}

void readDeviceId(char * buf)
{
	if(GB_SystemSetData.crc != CalModbusCRC16Fun1(&GB_SystemSetData, sizeof(SystemSetData) - 2))
		setDefaultSystemSetData(&GB_SystemSetData);
		
	memcpy(buf, GB_SystemSetData.deviceInfo.deviceid, MaxDeviceIDLen);
}

/***************************************************************************************************
*FunctionName: deviceInfoIsNew
*Description: 设备信息是否有更新
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016年12月16日16:59:16
***************************************************************************************************/
bool deviceInfoIsNew(void)
{
	if(GB_SystemSetData.crc == CalModbusCRC16Fun1(&GB_SystemSetData, sizeof(SystemSetData) - 2))
		return GB_SystemSetData.deviceInfo.isnew;
	else
		return false;
}

/***************************************************************************************************
*FunctionName: getNetSet
*Description: 获取网络设置
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016年12月16日16:39:18
***************************************************************************************************/
void getNetSet(NetSet * netSet)
{
	if(netSet)
	{
		//如果crc错误。恢复默认值
		if(GB_SystemSetData.crc != CalModbusCRC16Fun1(&GB_SystemSetData, sizeof(SystemSetData) - 2))
			setDefaultSystemSetData(&GB_SystemSetData);
		
		memcpy(netSet, &(GB_SystemSetData.netSet), sizeof(NetSet));
	}
}

/***************************************************************************************************
*FunctionName: isAutoPrint
*Description: 是否自动打印
*Input: 
*Output: 
*Return: 如果crc错误，则恢复默认值：不自动打印
*Author: xsx
*Date: 2016年12月16日16:43:04
***************************************************************************************************/
bool isAutoPrint(void)
{
	if(GB_SystemSetData.crc == CalModbusCRC16Fun1(&GB_SystemSetData, sizeof(SystemSetData) - 2))
		return GB_SystemSetData.isAutoPrint;
	else
		return false;
}

/***************************************************************************************************
*FunctionName: isMute
*Description: 是否静音，如果crc错误，则恢复默认值：不静音
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016年12月16日16:43:47
***************************************************************************************************/
bool isMute(void)
{
	if(GB_SystemSetData.crc == CalModbusCRC16Fun1(&GB_SystemSetData, sizeof(SystemSetData) - 2))
		return GB_SystemSetData.isMute;
	else
		return false;
}

/***************************************************************************************************
*FunctionName: getLedSleepTime
*Description: 读取屏幕休眠时间，如果crc错误，则恢复默认1分钟，0代表不休眠
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016年12月16日16:45:46
***************************************************************************************************/
unsigned short getLedSleepTime(void)
{
	if(GB_SystemSetData.crc == CalModbusCRC16Fun1(&GB_SystemSetData, sizeof(SystemSetData) - 2))
		return GB_SystemSetData.ledSleepTime;
	else
		return 60;
}

/***************************************************************************************************
*FunctionName: getLedLightIntensity
*Description: 读取led亮度，如果crc错误，则恢复默认：100%
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016年12月16日16:47:11
***************************************************************************************************/
unsigned char getLedLightIntensity(void)
{
	if(GB_SystemSetData.crc == CalModbusCRC16Fun1(&GB_SystemSetData, sizeof(SystemSetData) - 2))
		return GB_SystemSetData.ledLightIntensity;
	else
		return 100;
}

/***************************************************************************************************
*FunctionName: plusTestTotalNum
*Description: 增加测试数据数据
*Input: num -- 增加的数目
*Output: 
*Return: 
*Author: xsx
*Date: 2017年2月6日15:41:01
***************************************************************************************************/
void plusTestDataTotalNum(unsigned char num)
{
	if(GB_SystemSetData.crc != CalModbusCRC16Fun1(&GB_SystemSetData, sizeof(SystemSetData) - 2))
		setDefaultSystemSetData(&GB_SystemSetData);
	
	GB_SystemSetData.testDataNum += num;
	GB_SystemSetData.crc = CalModbusCRC16Fun1(&GB_SystemSetData, sizeof(SystemSetData) - 2);
}

/***************************************************************************************************
*FunctionName: getTestDataTotalNum
*Description: 读取当前测试数据总数
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2017年2月6日15:46:09
***************************************************************************************************/
unsigned int getTestDataTotalNum(void)
{
	if(GB_SystemSetData.crc != CalModbusCRC16Fun1(&GB_SystemSetData, sizeof(SystemSetData) - 2))
		setDefaultSystemSetData(&GB_SystemSetData);
	
	return GB_SystemSetData.testDataNum;
}

/***************************************************************************************************
*FunctionName: setUpLoadIndex
*Description: 更新上传索引
*Input: index -- 上传数据的索引
*Output: 
*Return: 
*Author: xsx
*Date: 2017年2月6日14:52:13
***************************************************************************************************/
void setUpLoadIndex(unsigned int index)
{
	if(GB_SystemSetData.crc != CalModbusCRC16Fun1(&GB_SystemSetData, sizeof(SystemSetData) - 2))
		setDefaultSystemSetData(&GB_SystemSetData);
	
	GB_SystemSetData.upLoadIndex = index;
	GB_SystemSetData.crc = CalModbusCRC16Fun1(&GB_SystemSetData, sizeof(SystemSetData) - 2);
}
unsigned int getUpLoadIndex(void)
{
	if(GB_SystemSetData.crc != CalModbusCRC16Fun1(&GB_SystemSetData, sizeof(SystemSetData) - 2))
		setDefaultSystemSetData(&GB_SystemSetData);
	
	return GB_SystemSetData.upLoadIndex;
}

/***************************************************************************************************
*FunctionName: setTestLedLightIntensity
*Description: 设置测试时的LED 亮度值
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2017年2月7日17:08:33
***************************************************************************************************/
void setTestLedLightIntensity(SystemSetData * systemSetData, unsigned short value)
{
	if(systemSetData == NULL)
		return;
	
	if(systemSetData->crc != CalModbusCRC16Fun1(systemSetData, sizeof(SystemSetData) - 2))
		setDefaultSystemSetData(systemSetData);
	
	systemSetData->testLedLightIntensity = value;
	systemSetData->crc = CalModbusCRC16Fun1(systemSetData, sizeof(SystemSetData) - 2);
}

/***************************************************************************************************
*FunctionName: getTestLedLightIntensity
*Description: 读取系统中的测试led亮度值
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2017年2月7日17:09:00
***************************************************************************************************/
unsigned short getTestLedLightIntensity(SystemSetData * systemSetData)
{
	if(systemSetData == NULL)
		return 200;
	
	if(systemSetData->crc != CalModbusCRC16Fun1(systemSetData, sizeof(SystemSetData) - 2))
		setDefaultSystemSetData(systemSetData);
	
	return systemSetData->testLedLightIntensity;
}

/***************************************************************************************************
*FunctionName: setIsShowRealValue, IsShowRealValue
*Description: 是否显示真实值
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2017年2月27日14:04:02
***************************************************************************************************/
void setIsShowRealValue(bool isShow)
{
	isShowRealValue = isShow;
}

bool IsShowRealValue(void)
{
	return isShowRealValue;
}
/****************************************end of file************************************************/
