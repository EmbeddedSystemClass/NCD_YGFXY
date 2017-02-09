#ifndef __SD_F_H__
#define __SD_F_H__

#include	"Define.h"
#include	"MyTest_Data.h"

/*��д������*/
MyState_TypeDef SaveUserData(User_Type * user);
MyState_TypeDef ReadUserData(User_Type * user);

/*WIFI����*/
MyState_TypeDef SaveWifiData(WIFI_Def * wifi);
MyState_TypeDef ReadWifiData(WIFI_Def * wifi);
MyState_TypeDef ClearWifiData(WIFI_Def * wifi);

/*�ϻ�����*/
MyState_TypeDef SaveReTestData(ReTestData *retestdata, unsigned char type);

#endif

