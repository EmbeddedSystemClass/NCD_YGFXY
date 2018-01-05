/****************************************file start****************************************************/
#ifndef ITEMCONST_D_H
#define ITEMCONST_D_H

#include	"Define.h"

MyState_TypeDef getItemConstData(ItemConstData * itemConstData, char * itemName);
MyState_TypeDef getItemNameByIndex(unsigned char index, char * itemName);
MyState_TypeDef getItemConstDataByIndex(ItemConstData * itemConstData, unsigned char index);

#endif

/****************************************end of file************************************************/
