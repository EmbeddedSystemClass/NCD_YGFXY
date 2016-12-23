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

#include	"CRC16.h"

#include	<string.h>
#include	"stdio.h"
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
static SystemSetData GB_SystemSetData;								//ϵͳ����
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
*FunctionName: setDefaultSetData
*Description: ��ʼ��ΪĬ������
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��19��09:17:30
***************************************************************************************************/
void setDefaultSetData(SystemSetData * systemSetData)
{
	if(systemSetData)
	{
		systemSetData->isAutoPrint = false;
		systemSetData->isMute = false;
		systemSetData->ledLightIntensity = 100;
		systemSetData->ledSleepTime = 60;
		
		sprintf(systemSetData->deviceInfo.deviceid, "ncd-device");
		systemSetData->deviceInfo.crc = CalModbusCRC16Fun1(&(systemSetData->deviceInfo), sizeof(DeviceInfo) - 2);
		
		systemSetData->netSet.ipmode = DHCP_Mode;
		systemSetData->netSet.crc = CalModbusCRC16Fun1(&(systemSetData->netSet), sizeof(NetSet) - 2);
		
		systemSetData->crc = CalModbusCRC16Fun1(systemSetData, sizeof(SystemSetData) - 2);
	}
}

/***************************************************************************************************
*FunctionName: getSystemSetData, setSystemSetData
*Description: ��дϵͳ������Ϣ��д�������Դֻ����SD���б��������
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��16��16:50:13
***************************************************************************************************/
void getSystemSetData(SystemSetData * systemSetData)
{
	if(systemSetData)
	{
		//���crc���󡣻ָ�Ĭ��ֵ
		if(GB_SystemSetData.crc != CalModbusCRC16Fun1(&GB_SystemSetData, sizeof(SystemSetData) - 2))
			setDefaultSetData(&GB_SystemSetData);
		
		memcpy(systemSetData, &GB_SystemSetData, sizeof(SystemSetData));
	}
}

void setSystemSetData(SystemSetData * systemSetData)
{
	if(systemSetData)
	{
		//�Զ�ȡ�����ݽ���У��
		if(systemSetData->ledLightIntensity > 100)
			systemSetData->ledLightIntensity = 100;
		else if(systemSetData->ledLightIntensity < 10)
			systemSetData->ledLightIntensity = 10;
		
		//0xffff��ʾ������
		if(systemSetData->ledSleepTime != 0xffff)
		{
			if(systemSetData->ledSleepTime > 600)
				systemSetData->ledSleepTime = 100;
			else if(systemSetData->ledSleepTime < 10)
				systemSetData->ledSleepTime = 10;
		}
		
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
			setDefaultSetData(&GB_SystemSetData);
		
		memcpy(deviceinfo, &(GB_SystemSetData.deviceInfo), sizeof(DeviceInfo));

	}
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
			setDefaultSetData(&GB_SystemSetData);
		
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
/****************************************end of file************************************************/
