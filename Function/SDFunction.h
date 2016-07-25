#ifndef __SD_F_H__
#define __SD_F_H__

#include	"Define.h"
#include	"MyTest_Data.h"

MyState_TypeDef CheckSDFunction(void);

/*��д�豸��Ϣ*/
MyState_TypeDef SaveDeviceInfo(DeviceInfo * deviceinfo);
MyState_TypeDef ReadDeviceInfo(DeviceInfo * deviceinfo);

/*��д������*/
MyState_TypeDef SaveUserData(User_Type * user);
void ReadUserData(User_Type * user);

/*��������*/
MyState_TypeDef SaveTestData(TestData *tempdata);
MyState_TypeDef ReadTestData(const char * filename, TestData *tempdata, unsigned short index);

/*ip����*/
MyState_TypeDef SaveNetData(NetData * netdata);
MyState_TypeDef ReadNetData(NetData * netdata);

/*WIFI����*/
MyState_TypeDef SaveWifiData(WIFI_Def * wifi);
MyState_TypeDef ReadWifiData(WIFI_Def * wifi);
MyState_TypeDef ClearWifiData(WIFI_Def * wifi);
#endif
