#ifndef	_SERVER_F_H_H
#define _SERVER_F_H_H

#include	"Define.h"
#include	"tcp.h"
#include 	"api.h" 
#include	"HttpBuf.h"


MyState_TypeDef CommunicateWithServerByLineNet(HttpBuffer * httpBuffer);

#if (DEVICE_CON_TYPE == DEVICE_WIFI)
MyState_TypeDef CommunicateWithServerByWifi(HttpBuffer * httpBuffer);
#elif (DEVICE_CON_TYPE == DEVICE_GPRS)
MyState_TypeDef CommunicateWithServerByGPRS(HttpBuffer * httpBuffer);
#endif //DEVICE_CON_TYPE

#if (USB_USE == 1)
MyState_TypeDef CommunicateWithServerByUSB(HttpBuffer * httpBuffer);
#endif

MyState_TypeDef CommunicateWithLisByLineNet(HttpBuffer * httpBuffer);

#endif //_SERVER_F_H_H
