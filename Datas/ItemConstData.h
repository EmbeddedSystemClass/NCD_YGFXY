/****************************************file start****************************************************/
#ifndef	ITEMCONSTDATA_D_H
#define	ITEMCONSTDATA_D_H

#include	"Define.h"

#pragma pack(1)
typedef struct ItemConstData_tag {
	char itemName[20];
	float minV;
	float maxV;
}ItemConst;
#pragma pack()

void getItemMaxMinValue(CardCodeInfo * cardCodeInfo);

#endif

/****************************************end of file************************************************/
