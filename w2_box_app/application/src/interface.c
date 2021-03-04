#include "interface.h"
#include "includes.h"
#include "st_flash.h"

extern _App_Param mApp_Param;
timer_t mx_time;

/*==================================================================================
* 函 数 名： inter_init
* 参    数： None
* 功能描述:  接口协议初始化
* 返 回 值： None
* 备    注： 默认间隔10ms进行接收数据解析
* 作    者： xiaozh
* 创建时间： 2019-09-23 180348
==================================================================================*/
void inter_init(void)
{  
}

/*==================================================================================
* 函 数 名： write_default_config_param
* 参    数： None
* 功能描述:  写入默认配置参数
* 返 回 值： None
* 备    注： 如果没有读取到正确的配置参数，则写入默认的配置参数
* 作    者： xiaozh
* 创建时间： 2019-11-08 121017
==================================================================================*/
void write_default_config_param(void)
{
	_App_Config_Param cfg_param;	
	
	cfg_param._Use_Param.can_addr = CAN_ADDR_NULL;		//默认为空地址
	cfg_param._Use_Param.can_addr_n = (uint8_t)(~cfg_param._Use_Param.can_addr);		//默认为空地址

	cfg_param._Use_Param.cc_can_addr = CAN_ADDR_NULL;		//默认为空地址
	cfg_param._Use_Param.cc_can_addr_n = (uint8_t)(~cfg_param._Use_Param.cc_can_addr);

	cfg_param._Use_Param.hard_ver = HARD_VERSION;		//硬件版本
	cfg_param._Use_Param.hard_ver_n = (uint8_t)(~cfg_param._Use_Param.hard_ver);		//硬件版本

	cfg_param._Use_Param.is_heat = 0;		
	cfg_param._Use_Param.is_heat_n = (uint8_t)(~cfg_param._Use_Param.is_heat);		

	cfg_param._Use_Param.rfid_retry_count = 2;		
	cfg_param._Use_Param.rfid_retry_count_n = (uint8_t)(~cfg_param._Use_Param.rfid_retry_count);

	cfg_param._Use_Param.cc_can_addr = CAN_ADDR_NULL;		//默认为空地址
	cfg_param._Use_Param.cc_can_addr_n = (uint8_t)(~cfg_param._Use_Param.cc_can_addr);		

	cfg_param._Use_Param.ant_adjust = 2;		
	cfg_param._Use_Param.ant_adjust_n = (uint8_t)(~cfg_param._Use_Param.ant_adjust);		
 
	cfg_param._Use_Param.crc_data_len = 16;		
	cfg_param._Use_Param.crc_data_len_n = (uint8_t)(~cfg_param._Use_Param.crc_data_len);		

	cfg_param._Use_Param.crc32 = crc32(cfg_param.flash_buff, cfg_param._Use_Param.crc_data_len);		
	 
	//擦除
	stFlash_Func.erase(APP_CONFIG_ADDR, MAX_USE_FLASH_SIZE*2);	//擦除参数配置区域
	//写入配置参数
	stFlash_Func.write(APP_CONFIG_ADDR, cfg_param.flash_buff, MAX_USE_FLASH_SIZE*2);	//写入参数
}

/*==================================================================================
* 函 数 名： read_config_param
* 参    数： _pApp_Config_Param
* 功能描述:  读取配置参数
* 返 回 值： None
* 备    注： 
* 作    者： xiaozh
* 创建时间： 2019-09-06 115746
==================================================================================*/
uint8_t read_config_param(void* c_param)
{  
	_pApp_Param pmsg = c_param;
	
	stFlash_Func.read(APP_CONFIG_ADDR, pmsg->config_param.flash_buff, MAX_USE_FLASH_SIZE*2);	//读取参数
	
	//判断参数是否合法
	if(crc32(pmsg->config_param.flash_buff, pmsg->config_param._Use_Param.crc_data_len) == pmsg->config_param._Use_Param.crc32)	//判断校验是否合法
	{
		//判断每一项参数是否合法
		if(pmsg->config_param._Use_Param.can_addr == ((~pmsg->config_param._Use_Param.can_addr_n)&0x000000FF))
		{
			pmsg->can_addr = pmsg->config_param._Use_Param.can_addr;
		}
		if(pmsg->config_param._Use_Param.cc_can_addr == ((~pmsg->config_param._Use_Param.cc_can_addr_n)&0x000000FF))
		{
			pmsg->cc_can_addr = pmsg->config_param._Use_Param.cc_can_addr;
		}
		
		if(pmsg->config_param._Use_Param.hard_ver == ((~pmsg->config_param._Use_Param.hard_ver_n)&0x000000FF))
		{
			pmsg->hard_ver = pmsg->config_param._Use_Param.hard_ver;
		}
		
		if(pmsg->config_param._Use_Param.is_heat == ((~pmsg->config_param._Use_Param.is_heat_n)&0x000000FF))
		{
			pmsg->is_heat = pmsg->config_param._Use_Param.is_heat;
		}

		if(pmsg->config_param._Use_Param.rfid_retry_count == ((~pmsg->config_param._Use_Param.rfid_retry_count_n)&0x000000FF))
		{
			pmsg->rfid_retry_count = pmsg->config_param._Use_Param.rfid_retry_count;
		}
		
		if(pmsg->config_param._Use_Param.heat_set_temp == ((~pmsg->config_param._Use_Param.heat_set_temp_n)&0x0000FFFF))
		{
			pmsg->heat_set_temp = pmsg->config_param._Use_Param.heat_set_temp;
		}
		
		if(pmsg->config_param._Use_Param.ant_adjust == ((~pmsg->config_param._Use_Param.ant_adjust_n)&0x0000FFFF))
		{
			pmsg->ant_adjust = pmsg->config_param._Use_Param.ant_adjust;
		}
	}
	else
	{ 
		//校验出错使用默认配置参数
		pmsg->can_addr = CAN_ADDR_NULL;
		pmsg->hard_ver = 0xFF;		//默认硬件版本为0xFF，为了区分是否使用的默认配置参数
		pmsg->is_heat = 0;
		pmsg->rfid_retry_count = 0x2;
		pmsg->heat_set_temp = 0;
		pmsg->ant_adjust = 0x02; 
		
		//写入默认配置参数
		write_default_config_param();
	}
	return 0;
}

/*==================================================================================
* 函 数 名： config_mcan_addr
* 参    数： uint8_t
* 功能描述:  配置CAN总线地址
* 返 回 值： None
* 备    注： 
* 作    者： xiaozh
* 创建时间： 2019-11-08 115948
==================================================================================*/
ret_msg_t config_mcan_addr(uint8_t cc_addr, uint8_t c_addr)
{
	ret_msg_t ret = BOX_UNKNOW_PARAM;
	_pApp_Param pmsg = &mApp_Param;
	
	if((mApp_Param.cc_can_addr == cc_addr) && (mApp_Param.can_addr == c_addr))//如果分配的地址完全相等，不再重复写入
	{
		debug_print("the cc addr is %d lc addr is %d\n", cc_addr, c_addr);
		ret = BOX_SUCCESS;
	} else {
		//修改缓存信息
		mApp_Param.cc_can_addr = cc_addr;
		mApp_Param.config_param._Use_Param.cc_can_addr = cc_addr;
		mApp_Param.config_param._Use_Param.cc_can_addr_n = ~ mApp_Param.config_param._Use_Param.cc_can_addr;

		mApp_Param.can_addr = c_addr;
		mApp_Param.config_param._Use_Param.can_addr = c_addr;
		mApp_Param.config_param._Use_Param.can_addr_n = (uint8_t)(~c_addr);
		
		//重新计算校验值 
		mApp_Param.config_param._Use_Param.crc32 = crc32(pmsg->config_param.flash_buff, pmsg->config_param._Use_Param.crc_data_len);
		
		//写入配置参数
			//擦除
		stFlash_Func.erase(APP_CONFIG_ADDR, MAX_USE_FLASH_SIZE*2);	//擦除参数配置区域
		//写入配置参数
		stFlash_Func.write(APP_CONFIG_ADDR, mApp_Param.config_param.flash_buff, MAX_USE_FLASH_SIZE*2);	//写入参数
		ret = BOX_SUCCESS;
	}
	
	return ret;
}
 
/*==================================================================================
* 函 数 名： can_rev_decode
* 参    数： None
* 功能描述:  can协议解析
* 返 回 值： None
* 备    注： none
* 作    者： xiaozh
* 创建时间： 2019-10-28 172557
==================================================================================*/
void can_rev_decode(void)
{
	_Ret_Msg c_msg; 
	 
	if(can_pop_one_frame(hcan1, &c_msg) < 0xff)		//获取数据
	{
		//协议解析
		can_frame_parse(&c_msg);		
	}
}

/*==================================================================================
* 函 数 名： can_sed_loop_task
* 参    数： None
* 功能描述:  CAN定时发送任务
* 返 回 值： None
* 备    注： 默认间隔20ms发送一次
* 作    者： xiaozh
* 创建时间： 2019-09-23 180348
==================================================================================*/
void can_sed_loop_task(void* argv)
{
	can_sed_loop_check(hcan1);	
}

/*==================================================================================
* 函 数 名： HAL_CAN_RxFifo0MsgPendingCallback
* 参    数： None
* 功能描述:  接收完成
* 返 回 值： None
* 备    注： 
* 作    者：  
* 创建时间： 2019-09-23 180101
==================================================================================*/
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* hcan) 
{
	_Can_Msg Can_Msg;
	CAN_RxHeaderTypeDef mCan_Header;
	
	//读取数据
	HAL_CAN_GetRxMessage(hcan,CAN_RX_FIFO0, &mCan_Header, (uint8_t*)Can_Msg.data);
	Can_Msg.ex_id.EX_ID = mCan_Header.ExtId;
	Can_Msg.byte_count = mCan_Header.DLC;
	
	//debug_print("can_IRQ,");
	switch(mCan_Header.FilterMatchIndex)
	{
		case 0://广播所有餐格
		case 1://广播一个cc下的所有餐格
		case 2://安卓->box 数据包
		case 4://CC-->餐格广播包过滤
		case 5://CC-->餐格数据包过滤
			//debug_print("Android to cc\r\n");
			can_recv_signal_frame(hcan1, &Can_Msg);
		break;

		case 3://安卓->box多包接收
			//debug_print("Android to box data\r\n");
			can_recv_mutil_frame(hcan1, &Can_Msg);
		break;

		default:
			//debug_print("error data\r\n");
		break;

	}
	
	//使能中断
	__HAL_CAN_ENABLE_IT(hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
}

/*==================================================================================
* 函 数 名： HAL_CAN_RxFifo0MsgPendingCallback
* 参    数： None
* 功能描述:  接收完成
* 返 回 值： None
* 备    注： 长包
* 作    者：  
* 创建时间： 2019-09-23 180101
==================================================================================*/
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef* hcan) 
{
	_Can_Msg Can_Msg;
	CAN_RxHeaderTypeDef mCan_Header;
	
	//读取数据
	HAL_CAN_GetRxMessage(hcan,CAN_RX_FIFO1, &mCan_Header, (uint8_t*)Can_Msg.data);
	Can_Msg.ex_id.EX_ID = mCan_Header.ExtId;
	Can_Msg.byte_count = mCan_Header.DLC;

	can_recv_mutil_frame(hcan1, &Can_Msg);
	
	//使能中断
	__HAL_CAN_ENABLE_IT(hcan, CAN_IT_RX_FIFO1_MSG_PENDING);
}
 

void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
{
	
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(htim ->Instance == TIM4)
  {
  	if(mx_time.motor_time < 130)   //给舵机400ms时间执行命令
  	{
  		mx_time.motor_time ++;
  	}
	else {
		MOTOR_DISABLE_OUT();
	}
  	led_change();
  }
}


