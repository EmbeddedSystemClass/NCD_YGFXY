#ifndef	_SERVER_F_H_H
#define _SERVER_F_H_H

#include	"Define.h"
#include	"tcp.h"
#include 	"api.h" 
#include	"HttpBuf.h"


MyState_TypeDef CommunicateWithServerByLineNet(HttpBuffer * httpBuffer);
MyState_TypeDef CommunicateWithServerByWifi(HttpBuffer * httpBuffer);
#if (USB_USE == 1)
MyState_TypeDef CommunicateWithServerByUSB(HttpBuffer * httpBuffer);
#endif
MyState_TypeDef CommunicateWithLisByLineNet(HttpBuffer * httpBuffer);
#endif
