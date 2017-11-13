#ifndef	_SERVER_F_H_H
#define _SERVER_F_H_H

#include	"Define.h"
#include	"tcp.h"
#include 	"api.h" 
#include	"HttpBuf.h"


void CommunicateWithServerByLineNet(HttpBuffer * httpBuffer);
void CommunicateWithServerByWifi(HttpBuffer * httpBuffer);

MyState_TypeDef UpLoadData(HttpBuffer * httpBuffer);

#endif
