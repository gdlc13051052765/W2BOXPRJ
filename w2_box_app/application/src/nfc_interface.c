#include "includes.h" 
#include "rfal_nfc.h"
#include "st25r3916.h"
#include "nfc_interface.h"
  
static uint32_t tag_cstm_id_table = 0x0000;		//每一位代表分配的情况
uint8_t globalCommProtectCnt; 

_Tag_Context mTag_Context;
	 
#define CLEAR_TAG_CSTM_ID(index)		do{(index == 0x0F)?(tag_cstm_id_table=0):(tag_cstm_id_table &= ~(0X0001<<index));}while(0)	//清空标签唯一ID，当标签个数为0时候，清空ID


//获取标签个数
#define GET_TAG_NUM()				mTag_Context.total_tag_num

extern rfalNfcvListenDevice nfcvDevList[];


void send_add_block_msg(uint8_t* buff)
{
	uint8_t send_buff[8];
	memcpy(send_buff + 1, buff, TAG_LENGTH - 1);
	send_buff[0] = 0x01;
	print_hex(send_buff, TAG_LENGTH);
	can_upload_event_to_android(BOX_Android_UP_CARD_INFO, send_buff, TAG_LENGTH);
} 

void send_rm_block_msg(uint8_t* buff)
{
	uint8_t send_buff[8];
	memcpy(send_buff + 1, buff, TAG_LENGTH - 1);
	send_buff[0] = 0x00;
	print_hex(send_buff, TAG_LENGTH);
	can_upload_event_to_android(BOX_Android_UP_CARD_INFO, send_buff, TAG_LENGTH);
} 

void send_rm_all_block_msg(void)
{
	uint8_t send_buff[8];  /* Flags + Block Data + CRC */

	send_buff[0] = 0x04;
	print_hex(send_buff, 1);
	can_upload_event_to_android(BOX_Android_UP_CARD_INFO, send_buff, 1);
} 
/*==================================================================================
* 函 数 名： rfid_context_init
* 参    数： None
* 功能描述:  数据结构初始化
* 返 回 值： None
* 备    注： 
* 作    者： xiaozh
* 创建时间： 2019-09-24 184722
==================================================================================*/
void rfid_context_init(void)
{
	mTag_Context.total_tag_num = 0;				//标签数量
	mTag_Context.read_tag_num = 0;				//标签数量
	mTag_Context.use_state = 0;			//当前存储是否占用
	
	for(int i=0; i<MAX_TAG_NUM; i++)
	{
		mTag_Context.new_tag_info[i].is_use  = 0;
		mTag_Context.new_tag_info[i].tag_state  = NULL_STA;
		memset(mTag_Context.new_tag_info[i].uid, 0xFF, TAG_LENGTH);
		memset(mTag_Context.new_tag_info[i].block, 0xFF, TAG_LENGTH);
		
		mTag_Context.old_tag_info[i].is_use  = 0;
		mTag_Context.old_tag_info[i].cstm_tag_id.cstm_id = 0;
		mTag_Context.old_tag_info[i].tag_state  = NULL_STA;
		memset(mTag_Context.old_tag_info[i].uid, 0xFF, TAG_LENGTH);
		memset(mTag_Context.old_tag_info[i].block, 0xFF, TAG_LENGTH);
	}
}

/*==================================================================================
* 函 数 名： context_clear
* 参    数： None
* 功能描述:  数据结构清空
* 返 回 值： None
* 备    注： 
* 作    者： xiaozh
* 创建时间： 2019-09-24 184722
==================================================================================*/
void context_clear(void)
{
	mTag_Context.total_tag_num = 0;
	mTag_Context.read_tag_num = 0;				//标签数量
	mTag_Context.use_state = 0;			//当前存储是否占用
	
	for(int i=0; i<MAX_TAG_NUM; i++)
	{
		mTag_Context.old_tag_info[i].is_use  = 0;
		mTag_Context.old_tag_info[i].tag_state  = NULL_STA;
		memset(mTag_Context.old_tag_info[i].uid, 0xFF, TAG_LENGTH);
		memset(mTag_Context.old_tag_info[i].block, 0xFF, TAG_LENGTH);
	}
}

/*==================================================================================
* 函 数 名： get_cstm_tag_id
* 参    数： None
* 功能描述:  给新接入的标签分配一个ID号
* 返 回 值： None
* 备    注： 
* 作    者： xiaozh
* 创建时间： 2019-11-05 114204
==================================================================================*/
uint8_t get_cstm_tag_id(void)
{
	for(int i=0; i<0x0F; i++)
	{
		if((tag_cstm_id_table & (0x0001<<i)) == 0)
		{
			tag_cstm_id_table |= (0x0001<<i);
			return i;
		}
	}
	
	return 0x0F;	//空间已满
}

/*==================================================================================
* 函 数 名： get_cstm_tag_id
* 参    数： None
* 功能描述:  给新接入的标签分配一个ID号
* 返 回 值： None
* 备    注： 
* 作    者： xiaozh
* 创建时间： 2019-11-05 114204
==================================================================================*/
uint8_t delete_cstm_tag_id(uint8_t index)
{
 	CLEAR_TAG_CSTM_ID(index); 
	return 0;	//空间已满
}

/*==================================================================================
* 函 数 名： find_tag_id_node
* 参    数： None
* 功能描述:  根据ID号查找标签索引
* 返 回 值： 返回索引
* 备    注： 
* 作    者： xiaozh
* 创建时间： 2019-11-05 120716
==================================================================================*/
uint8_t find_tag_id_node(uint8_t cstm_id)
{
	for(int i=0; i<MAX_TAG_NUM; i++)
	{
		if(mTag_Context.old_tag_info[i].is_use)
		{
			if(mTag_Context.old_tag_info[i].cstm_tag_id.cstm_id == cstm_id)
			{
				return i;
			}
		}
	}
	
	return 0xFF;	//空间已满
}

/*==================================================================================
* 函 数 名： find_avalib_node
* 参    数： None
* 功能描述:  查找空闲可以的节点
* 返 回 值： None
* 备    注： 
* 作    者： xiaozh
* 创建时间： 2019-09-25 145139
==================================================================================*/
uint8_t find_avalib_node(void)
{
	for(int i=0; i<MAX_TAG_NUM; i++)
	{
		if(!mTag_Context.old_tag_info[i].is_use)
		{
			return i;
		}
	}
	
	return 0xFF;	//没有查找到
}



/*==================================================================================
* 函 数 名： delete_node_index
* 参    数： None
* 功能描述:  删除节点
* 返 回 值： None
* 备    注： 
* 作    者： xiaozh
* 创建时间： 2019-10-10 180604
==================================================================================*/
uint8_t delete_node_index(void* info, uint8_t index)
{ 
	_pTag_Context pmsg = info;
	
	pmsg->old_tag_info[index].is_use = 0;
	pmsg->old_tag_info[index].tag_state = NULL_STA;
	
	pmsg->old_tag_info[index].is_update = 0; 
	pmsg->old_tag_info[index].tag_up_state = (_Tag_Sta)0;
	delete_cstm_tag_id(pmsg->old_tag_info[index].cstm_tag_id._bit.id_code);
	pmsg->old_tag_info[index].cstm_tag_id.cstm_id = 0;
	
	if(pmsg->total_tag_num > 0)
	{
		pmsg->total_tag_num--;
	}
	else
	{
		pmsg->total_tag_num = 0;
	}
	
	return 0;
}

/*==================================================================================
* 函 数 名： find_alike_card
* 参    数： None
* 功能描述:  查找相同id号的标签
* 返 回 值： 如果有相同的则返回索引，否则返回0xff
* 备    注： 
* 作    者： xiaozh
* 创建时间： 2019-10-10 174600
==================================================================================*/
uint8_t find_alike_card(void *info,  uint8_t *buff, uint16_t len)
{
	_pTag_Context pmsg = info;

	for(int i=0; i<MAX_TAG_NUM; i++)
	{
		if(pmsg->old_tag_info[i].is_use) //当前位置使用，且不为新接入的标签
		{
			if(memcmp(pmsg->old_tag_info[i].uid, buff, len) == 0)	//相等,比较到相同的标签
			{
				return i;
			}
		}
	}
	
	return 0xFF;
}

/*==================================================================================
* 函 数 名： find_lose_card
* 参    数： None
* 功能描述:  查询丢失的卡号
* 返 回 值： None
* 备    注： 
* 作    者： xiaozh
* 创建时间： 2019-10-10 174600
==================================================================================*/
uint8_t find_lose_card(void *info,  uint32_t opt_index)
{
	_pTag_Context pmsg = info;

	for(int i=0; i<MAX_TAG_NUM; i++)
	{
		if(pmsg->old_tag_info[i].is_use) //当前位置使用
		{
			if((opt_index & (0x00000001<<i)) == 0)		//当前节点没有被操作过
			{
				if(pmsg->old_tag_info[i].tag_state == EXIT_STA)		//如果上次已经为退出状态
				{
					if(pmsg->old_tag_info[i].try_time < mApp_Param.rfid_retry_count)
					{
						pmsg->old_tag_info[i].try_time++;
					}
					else
					{
						send_rm_block_msg(pmsg->old_tag_info[i].block);//上报删除
						//UPDATE_RFID_STATE(pmsg->old_tag_info[i], LOST_EXIT_STA);	//退出状态
						pmsg->old_tag_info[i].tag_state = NULL_STA;		//则认为标签离开，并删除节点
						delete_node_index(info, i);	//删除节点
					}
				}
				else
				{
					pmsg->old_tag_info[i].tag_state = EXIT_STA;		//则认为标签进入将要退出状态
				}
			}
		}
	}
	
	return 0;
}

/*==================================================================================
* 函 数 名： add_new_item
* 参    数： None
* 功能描述:  加入新节点
* 返 回 值： None
* 备    注： 
* 作    者： xiaozh
* 创建时间： 2019-10-10 174600
==================================================================================*/
uint8_t add_new_item(void *info,  uint8_t *card_id, uint16_t len)
{ 
	uint8_t f_index = 0;
	_pTag_Context pmsg = info;
 
	f_index = find_avalib_node();

	//拷贝标签ID
	memcpy(pmsg->old_tag_info[f_index].uid, card_id, TAG_UID_LENS);	//拷贝标签号
	debug_print("total_tag_num is %d new tag\n", pmsg ->total_tag_num);
	debug_print("uid:");
	print_hex(pmsg->old_tag_info[f_index].uid, 8);
	debug_print("\n");
	pmsg->old_tag_info[f_index].tag_state = ENTRY_BLOCK_FAIL_STA;
	pmsg->old_tag_info[f_index].is_use = 0x01;
	
	//变量增加
	pmsg->total_tag_num++;
	
	//给新增加的标签分配一个ID号 
	pmsg->old_tag_info[f_index].cstm_tag_id._bit.id_code = (uint8_t)(get_cstm_tag_id()&0x0F);	//获取唯一ID
	pmsg->old_tag_info[f_index].cstm_tag_id._bit.cstm_crc = crc4_itu(card_id, TAG_UID_LENS);	//获取标签校验码
	pmsg->old_tag_info[f_index].tag_up_state = NEW_ENTRY_STA;	//标记更新状态
	pmsg->old_tag_info[f_index].is_update = 0x00;	//标记更新状态

	return f_index;
}
 
/*==================================================================================
* 函 数 名： tag_compare_remark
* 参    数： None
* 功能描述:  返回标记的节点
* 返 回 值： None
* 备    注： 
* 作    者： xiaozh
* 创建时间： 2019-09-24 185604
==================================================================================*/
uint8_t tag_compare_remark(void *info,  rfalNfcvInventoryRes* cards)
{
	uint8_t r_index = 0; 
	_pTag_Context pmsg = info;
	uint8_t block_buff[16];  /* Flags + Block Data + CRC */
	uint16_t read_block_len = 0;

	//查找是否有相同ID号的
	if((r_index = find_alike_card(info, cards->UID, TAG_UID_LENS)) != 0xFF)	//有相同的
	{
		//判断状态是否为读取block失败
		if(pmsg->old_tag_info[r_index].tag_state == ENTRY_BLOCK_FAIL_STA)	//上一次读取失败
		{
			if(read_card_block(cards->UID, block_buff, &read_block_len) == ERR_NONE)		//读取标签信息成功，则上传标签信息
			{
				memcpy(pmsg->old_tag_info[r_index].block, block_buff, read_block_len);	//拷贝标签号
				pmsg->old_tag_info[r_index].tag_state = ENTRY_BLOCK_OK_STA;
				send_add_block_msg(pmsg->old_tag_info[r_index].block);
				//can_upload_event_to_android(0x20, 7, 6, block_buff, TAG_LENGTH - 1);
			}
		}
		else
		{
			pmsg->old_tag_info[r_index].tag_state = EXIST_STA;	//标签存在
		}
		
		return r_index;
	}
	else //没有找到相同的标签号
	{
		//新接入的标签号
		//拷贝标签ID
		r_index = add_new_item(info, cards->UID, TAG_UID_LENS); 
		
		//读取块内容
		if(read_card_block(cards->UID, block_buff, &read_block_len) == ERR_NONE)		//读取标签信息成功，则上传标签信息
		{
			memcpy(pmsg->old_tag_info[r_index].block, block_buff, read_block_len);	//拷贝标签号
			pmsg->old_tag_info[r_index].tag_state = ENTRY_BLOCK_OK_STA;
			send_add_block_msg(pmsg->old_tag_info[r_index].block);
		}

		//UPDATE_RFID_STATE(pmsg->old_tag_info[r_index], NEW_ENTRY_STA);	//退出状态
		//先上传
//		UPDATE_RFID_INFO(&pmsg->old_tag_info[r_index], pmsg->total_tag_num, NEW_ENTRY_STA, pmsg->old_tag_info[r_index].cstm_tag_id.cstm_id);	//上传标签信息
		return r_index;	//记录操作过的节点
	}
}

/*==================================================================================
* 函 数 名： compare_all_tag
* 参    数： None
* 功能描述:  对所有标签对比，并序列化
* 返 回 值： None
* 备    注： 
* 作    者： xiaozh
* 创建时间： 2019-09-24 185604
==================================================================================*/
void compare_all_tag(void *info,  rfalNfcvListenDevice* cards, uint8_t card_num)
{
	uint8_t r_index = 0;
	uint32_t opt_node_index = 0;
	_pTag_Context pmsg = info;
	uint8_t block_buff[16];  /* Flags + Block Data + CRC */
	uint16_t read_block_len = 0;

	if(pmsg ->total_tag_num == 0)	//如果没有一个标签，直接存储
	{
		CLEAR_TAG_CSTM_ID(0x0F);	//清空步进值
		 
		//添加新增标签
		for(int i = 0; i < card_num; i ++)
		{
			//拷贝标签ID
			r_index = add_new_item(info, nfcvDevList[i].InvRes.UID, TAG_UID_LENS); 
			
			//读取块内容
			if(read_card_block(nfcvDevList[r_index].InvRes.UID, block_buff, &read_block_len) == ERR_NONE)		//读取标签信息成功，则上传标签信息
			{
				memcpy(pmsg->old_tag_info[r_index].block, block_buff, read_block_len);	//拷贝标签号
				pmsg->old_tag_info[r_index].tag_state = ENTRY_BLOCK_OK_STA;
				send_add_block_msg(pmsg->old_tag_info[r_index].block);
			}
		}
		//置位总的标签数量
		pmsg->total_tag_num = card_num;
		return ;
	}

	//进行单个标签操作
	for(int i=0; i<card_num; i++)
	{
		r_index = tag_compare_remark(info, &cards[i].InvRes); 
		opt_node_index |= (0x00000001<<r_index);	//记录操作过的节点
	}
	 
	//查询丢失节点
	find_lose_card(info, opt_node_index);
}

//==================================================================================================
// NAME: void iso15693FindTag(void)
//
// BRIEF:扫描卡，并读block
// INPUTS: none
// OUTPUTS: none         
//====================================================================================================
void rfid_find_tag(void* info)
{ 
	static uint8_t read_zero_count = 0;		//读取到0个标签的次数
	   
	uint8_t actcnt; 

	actcnt = iso15693_find_tag();

	compare_all_tag(info, nfcvDevList, actcnt); 
  
	read_zero_count = 0;
	
	if(actcnt != 0)
	{ 
		//进行数据分组标记拷贝
		compare_all_tag(info, nfcvDevList, actcnt); 
  
		read_zero_count = 0;
	}
	else
	{
		//在最后一张标签退厂时候 
		if(GET_TAG_NUM() > 0)
		{
			read_zero_count++;

			if(read_zero_count > MAX_READ_ZERO_NUM)	//如果多次读取为空，则清空缓存
			{
				read_zero_count = 0;
				context_clear();
				printf("lost all tag\n");
				send_rm_all_block_msg();
				//send_rm_block_msg();
				//can_send_pkg_to_android(3, 3, pmsg->old_tag_info[i].uid, 8);
				//UPDATE_RFID_INFO(0, 0, LOST_ALL_STA, 0);	//告诉中继丢失所有标签
			}
		}
	}
	
//	show_read_tag(actcnt,  mTag_Context.total_tag_num);
//	show_read_tag(actcnt,  0);
}

//==================================================================================================
// NAME: void scanRfidTag(void)
//
// BRIEF:扫描卡，并读block

// INPUTS: none
// OUTPUTS: none         
//====================================================================================================
void scan_rfid_tag(void)
{
	//platformDelay(10);//可减少延时？？	

  rfid_find_tag(&mTag_Context);
}

/*==================================================================================
* 函 数 名： rfid_loop_read_task
* 参    数： None
* 功能描述:  循环读卡操作
* 返 回 值： None
* 备    注： 间隔100ms读取一次，可适当提高速率
* 作    者：  xiaozh
* 创建时间： 2019-09-24 163737
==================================================================================*/
extern uint32_t tick_num;

void rfid_loop_read_task(void* argv)
{
	uint32_t timebak = 0;
	
	DisableTask(TASK_RFID_READ);//关闭
	timebak = tick_num;
//	printf("raid task start = %d \r\n",tick_num);
	scan_rfid_tag();
//	printf("raid task time = %d \r\n",tick_num -timebak);
	EnableTask(TASK_RFID_READ);//打开
}

/*==================================================================================
* 函 数 名： sed_all_tag_info
* 参    数： None
* 功能描述:  向主机发送所有标签信息
* 返 回 值： None
* 备    注： 
* 作    者：  xiaozh
* 创建时间： 2019-10-30 151640
==================================================================================*/
uint8_t sed_all_tag_info(void)
{
	uint8_t step_count = 0;
	uint8_t sed_buff[MAX_TAG_NUM*20] = {0};
	
	sed_buff[step_count++] = mTag_Context.total_tag_num;
	 
	for(int i=0; i<MAX_TAG_NUM; i++)
	{
		if(mTag_Context.old_tag_info[i].is_use)
		{
			sed_buff[step_count++] = mTag_Context.old_tag_info[i].tag_state;
			sed_buff[step_count++] = mTag_Context.old_tag_info[i].cstm_tag_id.cstm_id;
			
			//拷贝标签号
			memcpy(sed_buff+step_count, mTag_Context.old_tag_info[i].uid, TAG_UID_LENS);
			step_count += TAG_UID_LENS;
			
			//拷贝block
			memcpy(sed_buff+step_count, mTag_Context.old_tag_info[i].block, TAG_LENGTH);
			step_count += TAG_LENGTH;
		}
	}

//	can_sed_link_pkg(update_tag_info, CAN_MASTER_ADDR, sed_buff, step_count);		//发送标签号和block块数据

	return 0;
}
/*==================================================================================
* 函 数 名： tag_info_param_ack
* 参    数： None
* 功能描述:  标签信息响应，根据ID进行标签信息同步判断
* 返 回 值： None
* 备    注： 
* 作    者：  xiaozh
* 创建时间： 2019-11-05 115752
==================================================================================*/
uint8_t tag_info_param_ack(uint8_t *tag_id, uint8_t len)
{
	uint8_t ret_index = 0;
	uint8_t ack_cstm_id = 0xFF;
	uint8_t ack_tag_state = 0xFF;
	
	if(len == 0x02)
	{
		ack_tag_state = tag_id[0];
		ack_cstm_id = tag_id[1];
		
		if((ret_index = find_tag_id_node(ack_cstm_id)) != 0xFF)
		{
			//查找到相同id，则代表成功
			if((mTag_Context.old_tag_info[ret_index].is_update == 0x01) &&(mTag_Context.old_tag_info[ret_index].tag_up_state == ack_tag_state))		//等待更新
			{
				mTag_Context.old_tag_info[ret_index].is_update = 0x02;		//确认完成
				
				//如果当前标签状态为退出，则删除相关缓存
				if((mTag_Context.old_tag_info[ret_index].tag_state == EXIT_STA)&&(mTag_Context.old_tag_info[ret_index].tag_up_state == LOST_EXIT_STA))
				{
					 delete_node_index(&mTag_Context, ret_index);		//删除当前标签信息
				}
			}
		}
	}
	else
	{
		
	}
	 
	return 0;
}


/*==================================================================================
* 函 数 名： get_total_tag_num
* 参    数： None
* 功能描述:  获取标签的总个数
* 返 回 值： None
* 备    注： 
* 作    者：  xiaozh
* 创建时间： 2019-11-06 114151
==================================================================================*/
uint8_t get_total_tag_num(void)
{
	return mTag_Context.total_tag_num;
}


