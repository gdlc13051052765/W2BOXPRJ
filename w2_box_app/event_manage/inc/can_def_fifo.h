#ifndef  __CAN_DEF_FIFO_H
#define  __CAN_DEF_FIFO_H
#include "can_drv.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "def_fifo.h"
 
typedef _Can_Msg		_Can_Item;
typedef _pCan_Msg		_pCan_Item;
  
#define _Can_Fifo 	struct	FIFO(can_xjy)

typedef struct
{
	uint8_t (*init_s)(void*, _pCan_Item, uint16_t);
	void* (*init_m)(_pCan_Item, uint16_t);
	uint8_t (*is_empty)(void*);
	uint8_t (*push)(void*, _pCan_Item);
	uint8_t (*pop)(void*, _pCan_Item);
}_Can_Fifo_Func,*_pCan_Fifo_Func;

extern const _pCan_Fifo_Func pCan1_Fifo_Func;

#endif
