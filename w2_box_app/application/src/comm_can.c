#include "comm_can.h" 
#include "crc.h"
#include "can_def_fifo.h"
#include <can_fifo.h>
#include "stdbool.h"

static _Mutil_Ring 	 mMutil1_Ring;		//多包接收 

static uint64_t mutil1_mark_table[64] = {0};

_Can_Instance mCan1_Instance;
//static _Can_Msg can1_sed_fifo[CAN_MAX_CACHE_LEN];	//can发送缓存
static send_queue_t	can1_queue;
static _Can_Msg can1_rev_fifo[CAN_MAX_CACHE_LEN];	//can接收缓存（单帧）

static uint8_t get_msg_id(void);
static uint8_t compare_id(uint32_t id1, uint32_t id2);
static uint8_t can_send_one_pkg(CAN_HandleTypeDef hcan, uint8_t host_cmd, uint8_t s1_addr, uint8_t s2_addr, uint8_t msg_id, uint8_t* buff, uint8_t len);
static void msg_queue_send(CAN_HandleTypeDef hcan, p_send_queue_t p_queue_buff);
static uint8_t msg_queue_ready(p_send_queue_t p_queue_buff);

/*==================================================================================
* 函 数 名： can_instance_init
* 参    数： None
* 功能描述:  can初始化
* 返 回 值： None
* 备    注： None
* 作    者： xiaozh
* 创建时间： 2019-10-28 170617
=================================================================================*/
void can_instance_init(CAN_HandleTypeDef hcan)
{  
	_Can_Instance * mCan_Instance;
	_pMutil_Ring  	 mMutil_Ring;		//多包接收指针

	mCan_Instance = &mCan1_Instance,
	mMutil_Ring = &mMutil1_Ring;
	mCan_Instance->p_ffunc	= pCan1_Fifo_Func,
	//发送接收缓存初始化（单帧） 

	msg_queue_init(&can1_queue);
 	//mCan_Instance->Sed_Fifo = mCan_Instance->p_ffunc->init_m(can1_sed_fifo, CAN_MAX_CACHE_LEN);
	mCan_Instance->Rcv_Fifo = mCan_Instance->p_ffunc->init_m(can1_rev_fifo, CAN_MAX_CACHE_LEN); 

	//长帧接收初始化
	mCan_Instance->pMutil_Fifo = mMutil_Ring;
	
	for(int i=0; i<MAX_SINGLE_ITEM_NUM; i++)
	{
		mMutil_Ring->cache_frame[i].ex_id.EX_ID = 0;
		mMutil_Ring->cache_frame[i].in_use = 0;
		memset(mMutil_Ring->cache_frame[i].r_data, 0, sizeof(mMutil_Ring->cache_frame[i].r_data));
	}
	mMutil_Ring->is_complete = 0;
	mMutil_Ring->recv_pkg_num = 0;
}
 
/*==================================================================================
* 函 数 名： find_avalib_node
* 参    数： None
* 功能描述:  查找可以节点
* 返 回 值： None
* 备    注： None
* 作    者： xiaozh
* 创建时间： 2019-10-28 161141
==================================================================================*/
static uint8_t find_null_node(CAN_HandleTypeDef hcan)
{
	_Mutil_Ring * 	 mMutil_Ring;		//多包接收指针

	mMutil_Ring = &mMutil1_Ring;
	//查询可用空节点索引
	for(int i=0; i<MAX_SINGLE_ITEM_NUM; i++)	//从后往前查找可用的ID号
	{
		if(mMutil_Ring->cache_frame[i].in_use == 0)
		{
			return i;
		}
	}

	return 0xFF;	//空，没有可用
}

/*==================================================================================
* 函 数 名： delete_item_node
* 参    数： None
* 功能描述:  清空节点
* 返 回 值： None
* 备    注： None
* 作    者： xiaozh
* 创建时间： 2019-10-28 161141
==================================================================================*/
static uint8_t delete_item_node(CAN_HandleTypeDef hcan, uint8_t index)
{	
	_Mutil_Ring * 	 mMutil_Ring;		//多包接收指针
  
	mMutil_Ring = &mMutil1_Ring;
	//清空缓存
	mMutil_Ring->cache_frame[index].in_use = 0;	//删除不合法帧 
//	mMutil_Ring->is_complete = 0;
	mMutil_Ring->cache_frame[index].ex_id.EX_ID = 0;
	//mMutil_Ring[index].recv_pkg_num = 0;
	mMutil_Ring->cache_frame[index].r_len = 0;
	mMutil_Ring->cache_frame[index].pkg_id = 0;
	
	return 0;
}

static uint8_t compare_id(uint32_t id1, uint32_t id2)
{
	uint32_t id_mask = 0xFFFFE07F;
	uint8_t ret = 0;
	if (id1 == id2)
	{
		ret = SAME_ID;
	} else if ((id1 & id_mask) == (id2 & id_mask))
	{
		ret = SAME_PKG_ID;
	}
	return ret;
}
/*==================================================================================
* 函 数 名： item_is_exist
* 参    数： None
* 功能描述:  查看是否已经存在
* 返 回 值： None
* 备    注： None
* 作    者： xiaozh
* 创建时间： 2019-10-28 161141
==================================================================================*/
static uint8_t item_is_exist(CAN_HandleTypeDef hcan, uint32_t ex_id)
{
	_Mutil_Ring * 	 mMutil_Ring;		//多包接收指针
	uint8_t ret_index = 0xFF;	//返回的节点索引
	_pEx_id pmsg = (_pEx_id)&ex_id; 

	mMutil_Ring = &mMutil1_Ring;
	
	//查询当前地址是否已经有缓存帧
	for(int j=0; j<MAX_SINGLE_ITEM_NUM; j++)	//从后往前查找可用的ID号
	{
		uint8_t temp = compare_id(mMutil_Ring->cache_frame[j].ex_id.EX_ID , ex_id);
		//判断是否为同一个包的ID
		if( SAME_PKG_ID == temp)	
		{
			ret_index = j;
		} else if (SAME_ID == temp)
		{
			printf("mMutil_Ring[j].ex_id.EX_ID = %d,ex_id = %d ,j = %d \r\n",mMutil_Ring->cache_frame[j].ex_id.EX_ID , ex_id,j);
			ret_index = 0xFE;//完全相同的帧直接返回0xFE
		}
	}
	
	//没有接收完成的包，丢了结尾帧情况
	if(ret_index == 0xFF)
	{
		for(int i=0; i<MAX_SINGLE_ITEM_NUM; i++)	//从后往前查找可用的ID号
		{
			if(mMutil_Ring->cache_frame[i].ex_id._bit.s1_addr == pmsg->_bit.s1_addr)	//如果存在相同地址
			{
				if((mMutil_Ring[i].is_complete == 0)&&(mMutil_Ring->cache_frame[i].in_use != 0))	//没有接收完成
				{
					//直接覆盖没有接收完整节点
					mMutil_Ring->cache_frame[i].in_use = 0;
					mMutil_Ring->cache_frame[i].ex_id.EX_ID = ex_id;
					mMutil_Ring->recv_pkg_num = 0;
					mMutil_Ring->cache_frame[i].r_len = 0;
					ret_index = i;
				}
			}
		}
	}

	return ret_index;	//空，没有可用
}


uint8_t can_recv_mutil_frame(CAN_HandleTypeDef hcan, void *can_msg)
{
	_Mutil_Ring * 	 mMutil_Ring;		//多包接收指针

	uint8_t new_index = 0;	//申请索引
	_pCan_Msg pmsg = can_msg;
	mMutil_Ring = &mMutil1_Ring;
	
	
//	debug_print("pmsg->ex_id.EX_ID == %d \r\n",pmsg->ex_id.EX_ID);
//	debug_print("pmsg->ex_id._bit.msg_id == %d \r\n",pmsg->ex_id._bit.msg_id );
//	debug_print("pmsg->ex_id._bit.pkg_id == %d \r\n",pmsg->ex_id._bit.pkg_id );
//	debug_print("pmsg->ex_id._bit.is_end == %d \r\n",pmsg->ex_id._bit.is_end );
	if(pmsg->ex_id._bit.pkg_id == 0)	//帧错误:多包pkg_id不能为0
	{
		return 0x81;
	}
	else
	{	
		//判断当前id是否存在
		new_index = item_is_exist(hcan, pmsg->ex_id.EX_ID);
		if(new_index != 0xFE)
		{
			//当时新帧的时候保存到缓存	
			new_index = find_null_node(hcan);	
			debug_print("new_index2 == %02X \r\n",new_index);
			mMutil_Ring->cache_frame[new_index].in_use = 0x01;	//使用中
			mMutil_Ring->cache_frame[new_index].r_len = pmsg->byte_count;
			mMutil_Ring->cache_frame[new_index].pkg_id = pmsg->ex_id._bit.pkg_id;
			//memcpy(mMutil_Ring[new_index].r_data + (pmsg->ex_id._bit.pkg_id - 1) * 8, pmsg -> data, pmsg -> byte_count);	
			memcpy(mMutil_Ring->cache_frame[new_index].r_data, pmsg->data, pmsg->byte_count);
			//接收帧数累加
			mMutil_Ring->recv_pkg_num = mMutil_Ring->recv_pkg_num + 1;
			//判断是否为末尾帧	
			if(pmsg->ex_id._bit.is_end == 0x01)	//末尾帧
			{
				//debug_print("recv_pkg_num = %d \r\n",mMutil_Ring->recv_pkg_num);
				if(mMutil_Ring->recv_pkg_num == pmsg->ex_id._bit.pkg_id)
				{ 
					mMutil_Ring->is_complete = 0x01;	//接收完成
					mMutil_Ring->ex_id.EX_ID = pmsg->ex_id.EX_ID;//将最后一帧的ID保存为长帧ID
					//CRC校验留在协议解析，防止过多占用中断	
				}
				else
				{  
					//清空节点
					for(uint8_t i=0;i<MAX_SINGLE_ITEM_NUM;i++)
						delete_item_node(hcan, new_index);
				}
			}		
		}
		else
		{
			debug_print("err index = %d \r\n",new_index);
		}
	}
	 
	return 0;
}
 
/*==================================================================================
* 函 数 名： can_recv_frame
* 参    数： _pCan_Msg
* 功能描述:  can总线数据接收
* 返 回 值： None
* 备    注： None
* 作    者： xiaozh
* 创建时间： 2019-10-28 154449
==================================================================================*/
uint8_t can_recv_signal_frame(CAN_HandleTypeDef hcan, void *can_msg)
{  
	_Can_Instance * mCan_Instance;

	mCan_Instance = &mCan1_Instance,

	mCan_Instance->p_ffunc->push(mCan_Instance->Rcv_Fifo, can_msg);
	
	return 0;
}

/*==================================================================================
* 函 数 名： find_index_pkg
* 参    数： pkg_id
* 功能描述:  根据pkg id 从长包缓存中找到对应包号
* 返 回 值： None
* 备    注： 
* 作    者： xiaozh
* 创建时间： 2019-10-28 154449
==================================================================================*/ 
uint8_t find_index_pkg(uint8_t pkg_id)
{
	_Mutil_Ring * 	 mMutil_Ring;		//多包接收指针
	mMutil_Ring = &mMutil1_Ring;
	
	for(uint8_t i=0;i<MAX_SINGLE_ITEM_NUM;i++)
	{
		if(mMutil_Ring->cache_frame[i].pkg_id == pkg_id )
		{
			return i ;
		}
	}
}
/*==================================================================================
* 函 数 名： can_pop_one_frame
* 参    数： _pRet_Msg
* 功能描述:  can总线数据接收
* 返 回 值： None
* 备    注： 
* 作    者： xiaozh
* 创建时间： 2019-10-28 154449
==================================================================================*/ 
static uint8_t cur_index = 0;
uint8_t can_pop_one_frame(CAN_HandleTypeDef hcan, void *ret_msg)
{
	//uint8_t cur_index;
	_Can_Instance * mCan_Instance;
	_Mutil_Ring * 	 mMutil_Ring;		//多包接收指针

	_pRet_Msg pmsg = ret_msg;
	
	mCan_Instance = &mCan1_Instance,
	mMutil_Ring = &mMutil1_Ring;

	if(mMutil_Ring->is_complete == 0x01) 
	{
		mMutil_Ring->is_complete = 0;
		pmsg->byte_count = 0;
		//先查询长帧
		for(int i=0; i<mMutil_Ring->recv_pkg_num+1 ; i++)	//从后往前查找可用的ID号
		{		
			cur_index = find_index_pkg(i+1);
			if(mMutil_Ring->cache_frame[cur_index].in_use == 0x01)
			{
				//debug_print("mMutil_Ring->cache_frame[cur_index].r_len = %d ,cur_index = %d\r\n",mMutil_Ring->cache_frame[cur_index].r_len,cur_index);
				memcpy(pmsg->data +pmsg->byte_count, mMutil_Ring->cache_frame[cur_index].r_data,  mMutil_Ring->cache_frame[cur_index].r_len);
				pmsg->byte_count += mMutil_Ring->cache_frame[cur_index].r_len;
				//清空节点
				delete_item_node(hcan, cur_index);
			}		
		}
		//if(mMutil_Ring->is_complete == 0x01)
		{
			pmsg->ex_id.EX_ID = mMutil_Ring->ex_id.EX_ID;	
			mMutil_Ring->recv_pkg_num = 0;
			debug_print("mMutil_data:");
			debug_print_hex(pmsg->data, pmsg->byte_count);
			debug_print("\r\n");
			debug_print("pmsg->byte_count = %d \r\n",pmsg->byte_count);
			return pmsg->byte_count;
		}			
	}

	
	//查询短帧 
	if(	mCan_Instance->p_ffunc->pop(mCan_Instance->Rcv_Fifo, ret_msg) != 0) 
	{
		//查找成功
//		datalen = pmsg->byte_count;
		
		debug_print("mMutil_data:");
		debug_print_hex(pmsg->data, pmsg->byte_count);
		debug_print("\r\n");
		return pmsg->byte_count;
	}
	
	return 0xff;
}

/*==================================================================================
* 函 数 名： can_sed_loop_check
* 参    数： None
* 功能描述:  CAN循环检测是否有发送的数据
* 返 回 值： None
* 备    注： None
* 作    者： xiaozh
* 创建时间： 2019-10-28 171529
==================================================================================*/
void can_sed_loop_check(CAN_HandleTypeDef hcan)
{
	p_send_queue_t p_queue_buff;
	
	p_queue_buff = &can1_queue;

	//检查是否符合发送条件
	if(msg_queue_ready(p_queue_buff))
	{
		//发送数据
		msg_queue_send(hcan, p_queue_buff);
	}
}

uint8_t can_send_one_pkg_to_Android_by_link(uint8_t host_cmd, uint8_t msg_id, uint8_t* buff, uint8_t len)
{
	return can_send_one_pkg(hcan1, host_cmd, mApp_Param.cc_can_addr, mApp_Param.can_addr, msg_id, buff, len);//0x7发送本机地址
}

uint8_t can_send_one_pkg_to_cc_by_radio(uint8_t host_cmd, uint8_t msg_id, uint8_t* buff, uint8_t len)
{
	return can_send_one_pkg(hcan1, host_cmd, 0xD, 0, msg_id, buff, len);
}

uint8_t can_send_one_pkg_to_cc_by_link(uint8_t host_cmd, uint8_t msg_id, uint8_t* buff, uint8_t len)
{
	return can_send_one_pkg(hcan1, host_cmd, 0xD, mApp_Param.can_addr, msg_id, buff, len);//0x7发送本机地址
}

static uint8_t can_send_one_pkg(CAN_HandleTypeDef hcan, uint8_t host_cmd, uint8_t s1_addr, uint8_t s2_addr, uint8_t msg_id, uint8_t* buff, uint8_t len)//通过广播方式组包进行发送，不关心对方是否收到
{
	_Can_Msg c_msg;

	c_msg.ex_id.EX_ID = 0;
	c_msg.ex_id._bit.s1_addr = s1_addr;
	c_msg.ex_id._bit.s2_addr = s2_addr;
	c_msg.ex_id._bit.png_cmd = host_cmd;

	c_msg.ex_id._bit.is_end = 1;
	c_msg.ex_id._bit.pkg_id = 0;
	c_msg.ex_id._bit.msg_id = msg_id;
	c_msg.byte_count = len;

	memset(c_msg.data, 0, 8);
	for(int j = 0; j < len; j ++)
	{
		c_msg.data[j] = buff[j]; 
	}
	
	return can_sed_pkg_without_cache(hcan, &c_msg);
}

static unsigned int last_ts = 0;
static void msg_queue_send(CAN_HandleTypeDef hcan, p_send_queue_t p_queue_buff)
{
	_Can_Msg c_msg;

    last_ts = HAL_GetTick();//记录发送时间

	memcpy(&c_msg, &p_queue_buff->queue[p_queue_buff->rd], sizeof(_Can_Msg));
    can_sed_pkg_without_cache(hcan1, &c_msg);
    //msg_queue_pop(&can1_queue, 1);
    
    //log_printf(PRINT_DEBUG"Send Q %d len: %d\r\n",p_queue_buff->count, p_queue_buff->queue[p_queue_buff->rd].len);
}

void can1_msg_queue_pop(uint32_t msg_id)
{
//	unsigned int num;
//    unsigned int ts;
//	
//		last_ts = HAL_GetTick();//两次发送间隔&& (ts - last_ts > 3)

//    num = msg_queue_num(&can1_queue);
//		if(num>0)
//		{
//			
//		}
    msg_queue_pop(&can1_queue, msg_id);
    
    //log_printf(PRINT_DEBUG"Send Q %d len: %d\r\n",p_queue_buff->count, p_queue_buff->queue[p_queue_buff->rd].len);
}

uint8_t send_num_bak = 0;
uint8_t msg_queue_ready(p_send_queue_t p_queue_buff)
{
    unsigned int num;
    unsigned int ts;

    ts = HAL_GetTick();//两次发送间隔&& (ts - last_ts > 3)

    num = msg_queue_num(&can1_queue);
		if(send_num_bak !=num)
		{
			send_num_bak = num;
			return 1;
		}
		
    if (num > 0 && (ts - last_ts > RETRY_TIME))//间隔RETRY_TIME 没有收到安卓回复重发指令
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void can_upload_event_to_android(uint32_t host_cmd, uint8_t* buff, uint16_t len)//回复安卓发送的命令
{
	_Can_Msg c_msg;

	c_msg.ex_id.EX_ID = 0;
	c_msg.ex_id._bit.s1_addr = mApp_Param.cc_can_addr;
	c_msg.ex_id._bit.s2_addr = mApp_Param.can_addr;
	c_msg.ex_id._bit.png_cmd = host_cmd;

	c_msg.ex_id._bit.is_end = 1;
	c_msg.ex_id._bit.pkg_id = 0;
	c_msg.ex_id._bit.msg_id = get_msg_id();
	c_msg.byte_count = len;

	memset(c_msg.data, 0, 8);
	memcpy(c_msg.data, buff, len);
	msg_queue_push(&can1_queue, &c_msg);

	return ;
}

static uint8_t get_msg_id(void)
{
	static uint8_t msg_id = 0;
	msg_id = (msg_id + 1) % 4;

	return msg_id;
}


 
