#ifndef _CARDS_S_D_H__
#define _CARDS_S_D_H__


#include	"Define.h"

typedef enum
{ 
	NoCard = 0,								//�޿�
	CardIN = 1								//�п�
}CardState_Def;


CardState_Def GetCardState(void);
void SetCardState(CardState_Def value);

#endif

