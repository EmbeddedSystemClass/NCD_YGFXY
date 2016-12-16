#ifndef __SD_F_H__
#define __SD_F_H__

#include	"Define.h"
#include	"MyTest_Data.h"

MyState_TypeDef CheckSDFunction(void);

/*��д������*/
MyState_TypeDef SaveUserData(User_Type * user);
MyState_TypeDef ReadUserData(User_Type * user);


/*ip����*/
MyState_TypeDef SaveNetData(NetData * netdata);
MyState_TypeDef ReadNetData(NetData * netdata);

/*WIFI����*/
MyState_TypeDef SaveWifiData(WIFI_Def * wifi);
MyState_TypeDef ReadWifiData(WIFI_Def * wifi);
MyState_TypeDef ClearWifiData(WIFI_Def * wifi);

/*У׼����*/
MyState_TypeDef SaveAdjustData(AdjustData *adjustdata);
MyState_TypeDef ReadAdjustData(AdjustData *adjustdata);

/*�ϻ�����*/
MyState_TypeDef SaveReTestData(ReTestData *retestdata, unsigned char type);

#endif

