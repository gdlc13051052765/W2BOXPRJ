#include "can_def_fifo.h"


#define func_name   can_xjy

//…Í«ÎΩ·ππ 
DEF_SAFE_FIFO(can_xjy, _Can_Item, uint16_t, uint8_t)     
DEF_SAFE_FIFO_END


static _Can_Fifo_Func mCan_Fifo_Func = 
{
	.init_s						=  	FIFO_INIT_S_X(can_xjy),
	.init_m						=  	FIFO_INIT_M_X(can_xjy),
	.is_empty 				= 	FIFO_IS_EMPTY_X(can_xjy),
	.push	 						= 	FIFO_PUSH_X(can_xjy),
	.pop 							= 	FIFO_POP_X(can_xjy),
};

const _pCan_Fifo_Func pCan1_Fifo_Func = &mCan_Fifo_Func;


