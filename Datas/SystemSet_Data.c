/***************************************************************************************************
*FileName:	SystemSet_Data
*Description:	ϵͳ���ò���
*Author: xsx_kair
*Data:	2016��12��16��16:20:43
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
static SystemSetData GB_SystemSetData;								//ϵͳ����


static bool isShowRealValue = false;								//�Ƿ���ʾ��ʵ����
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
*Description: ��ʼ��ΪĬ������
*Input: systemSetData -- ��Ҫ���õ�ϵͳ���õ�ַ
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��19��09:17:30
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
		
		//���У׼����
		memset(systemSetData->parm1, 0, 700);
		memset(systemSetData->parm2, 0, 156);
		
		systemSetData->crc = CalModbusCRC16Fun1(systemSetData, sizeof(SystemSetData) - 2);
	}
}

/***************************************************************************************************
*FunctionName: copyGBSystemSetData, getGBSystemSetData, setSystemSetData
*Description: ����һ��ϵͳ����,��ȡϵͳ������ַ
*Input: systemSetData -- ���Ƶ�Ŀ���ַ
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��16��16:50:13
***************************************************************************************************/
void copyGBSystemSetData(SystemSetData * systemSetData)
{
	if(systemSetData)
	{
		//���crc���󡣻ָ�Ĭ��ֵ
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
*Description: ��ȡ�豸��Ϣ
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��16��16:38:22
***************************************************************************************************/
void getDeviceInfo(DeviceInfo * deviceinfo)
{
	if(deviceinfo)
	{
		//���crc���󡣻ָ�Ĭ��ֵ
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
*Description: �豸��Ϣ�Ƿ��и���
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��16��16:59:16
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
*Description: ��ȡ��������
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��16��16:39:18
***************************************************************************************************/
void getNetSet(NetSet * netSet)
{
	if(netSet)
	{
		//���crc���󡣻ָ�Ĭ��ֵ
		if(GB_SystemSetData.crc != CalModbusCRC16Fun1(&GB_SystemSetData, sizeof(SystemSetData) - 2))
			setDefaultSystemSetData(&GB_SystemSetData);
		
		memcpy(netSet, &(GB_SystemSetData.netSet), sizeof(NetSet));
	}
}

/***************************************************************************************************
*FunctionName: isAutoPrint
*Description: �Ƿ��Զ���ӡ
*Input: 
*Output: 
*Return: ���crc������ָ�Ĭ��ֵ�����Զ���ӡ
*Author: xsx
*Date: 2016��12��16��16:43:04
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
*Description: �Ƿ��������crc������ָ�Ĭ��ֵ��������
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��16��16:43:47
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
*Description: ��ȡ��Ļ����ʱ�䣬���crc������ָ�Ĭ��1���ӣ�0��������
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��16��16:45:46
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
*Description: ��ȡled���ȣ����crc������ָ�Ĭ�ϣ�100%
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��16��16:47:11
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
*Description: ���Ӳ�����������
*Input: num -- ���ӵ���Ŀ
*Output: 
*Return: 
*Author: xsx
*Date: 2017��2��6��15:41:01
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
*Description: ��ȡ��ǰ������������
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2017��2��6��15:46:09
***************************************************************************************************/
unsigned int getTestDataTotalNum(void)
{
	if(GB_SystemSetData.crc != CalModbusCRC16Fun1(&GB_SystemSetData, sizeof(SystemSetData) - 2))
		setDefaultSystemSetData(&GB_SystemSetData);
	
	return GB_SystemSetData.testDataNum;
}

/***************************************************************************************************
*FunctionName: setUpLoadIndex
*Description: �����ϴ�����
*Input: index -- �ϴ����ݵ�����
*Output: 
*Return: 
*Author: xsx
*Date: 2017��2��6��14:52:13
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
*Description: ���ò���ʱ��LED ����ֵ
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2017��2��7��17:08:33
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
*Description: ��ȡϵͳ�еĲ���led����ֵ
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2017��2��7��17:09:00
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
*Description: �Ƿ���ʾ��ʵֵ
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2017��2��27��14:04:02
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
