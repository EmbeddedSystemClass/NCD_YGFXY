#ifndef	_SERVER_F_H_H
#define _SERVER_F_H_H

#include	"Define.h"
#include	"tcp.h"
#include 	"api.h" 
#include	"HttpBuf.h"


MyState_TypeDef CommunicateWithServerByLineNet(HttpBuffer * httpBuffer);
MyState_TypeDef CommunicateWithServerByWifi(HttpBuffer * httpBuffer);
MyState_TypeDef CommunicateWithServerByUSB(HttpBuffer * httpBuffer);
#endif
