#include "command.h"
#include "includes.h"
#include "oled_interface.h"
#include "iap_protocols.h"

extern _App_Param mApp_Param;

static void can_send_sw_and_hw_to_android(char cmd, char msg_id);
static void can_send_door_sta_to_android(char cmd, char msg_id);
static void can_send_temp_sta_to_android(char cmd, char msg_id);
static void can_sed_heartbeat(char cmd, char msg_id);
static void box_update(void* ret_msg);
static void iap_simply_ack(uint32_t can_id, uint8_t ret_reuslt, uint16_t ret_id);
static void iap_check_ack(uint32_t can_id, uint8_t ret_reuslt, uint16_t ret_id, uint16_t toal_num);
/*==================================================================================
* 函 数 名： can_frame_parse
* 参    数： None
* 功能描述:  can协议解析
* 返 回 值： None
* 备    注： None
* 作    者： xiaozh
* 创建时间： 2019-09-24 162950
==================================================================================*/
void can_frame_parse(void* ret_msg)
{
	_pRet_Msg pmsg = ret_msg;
	_Disp_Param dispStr;
	uint16_t ret_id = 0;
	uint16_t toal_num = 0;
	uint16_t flash_addr = 0;
	uint8_t ret_s = 0,i;
	
	uint8_t buff[8] = {0};
	uint8_t picbuff[128] = {0xff};
	//判断数据合法性
//	debug_print("can_rev,");
//	debug_print("png_cmd:%4x, ", pmsg->ex_id._bit.png_cmd);
//	debug_print("s1_addr:%4x, ", pmsg->ex_id._bit.s1_addr);
//	debug_print("s2_addr:%4x, ", pmsg->ex_id._bit.s2_addr);
//	debug_print("is_end:%4x, ", pmsg->ex_id._bit.is_end);
//	debug_print("msg_id:%4x, ", pmsg->ex_id._bit.msg_id);
//	debug_print("lens=0x%02x, ", pmsg->byte_count);
//	debug_print("rev_data:");
//	debug_print_hex(pmsg->data, pmsg->byte_count);
//	debug_print("\n");
	
	if(pmsg->ex_id._bit.s2_addr != mApp_Param.can_addr && pmsg->ex_id._bit.s2_addr !=0x0f)
		return;
	 
	//进行协议解析
	if (pmsg->ex_id._bit.s1_addr != 0xD)//非cc过来的数据
	{
		switch(pmsg->ex_id._bit.png_cmd)
		{	
			//can1_msg_queue_pop(pmsg->ex_id._bit.msg_id);
			case Android_BOX_GET_SW_AND_HW_VER:
			{
				debug_print("send Android_BOX_GET_SW_AND_HW_VER \r\n");
				can_send_sw_and_hw_to_android(pmsg->ex_id._bit.png_cmd, pmsg->ex_id._bit.msg_id);
				break;
			}
			case Android_BOX_CONTROL_LED:
			{
				debug_print("Android_BOX_CONTROL_LED \r\n");
				buff[0] = led_control(pmsg->data[1], pmsg->data[0]);
				can_send_one_pkg_to_Android_by_link(pmsg->ex_id._bit.png_cmd, pmsg->ex_id._bit.msg_id, buff, 1);
				break;
			}
			case Android_BOX_CONTROL_DOOR:
			{
				debug_print("Android_BOX_CONTROL_DOOR \r\n");
				buff[0] = door_control(pmsg->data[0]);
				can_send_one_pkg_to_Android_by_link(pmsg->ex_id._bit.png_cmd, pmsg->ex_id._bit.msg_id, buff, 1);
				break;
			}
			case Android_BOX_GET_DOOR_STATUS:
			{
				debug_print("Android_BOX_GET_DOOR_STATUS \r\n");
				can_send_door_sta_to_android(pmsg->ex_id._bit.png_cmd, pmsg->ex_id._bit.msg_id);
				break;
			}
			case Android_BOX_CONTROL_HEAT:
			{
				debug_print("Android_BOX_CONTROL_HEAT \r\n");
				buff[0] = temp_control(pmsg->data[1], pmsg->data[0]);//功能未实现
				can_send_one_pkg_to_Android_by_link(pmsg->ex_id._bit.png_cmd, pmsg->ex_id._bit.msg_id, buff, 1);
				break;
			}
			case Android_BOX_GET_HEAT:
			{
				debug_print("Android_BOX_GET_HEAT \r\n");
				can_send_temp_sta_to_android(pmsg->ex_id._bit.png_cmd, pmsg->ex_id._bit.msg_id);
				break;
			}
			case Android_BOX_CONTROL_DISPLAY:
			{
				debug_print("Android_BOX_CONTROL_DISPLAY \r\n");
				break;
			}
			case Android_BOX_CONTROL_CARD_READER:
			{
				mApp_Param.rfid_retry_count = (pmsg->data[0] >> 4);
				buff[0] = BOX_SUCCESS;
				can_send_one_pkg_to_Android_by_link(pmsg->ex_id._bit.png_cmd, pmsg->ex_id._bit.msg_id, buff, 1);
				debug_print("Android_BOX_CONTROL_CARD_READER \r\n");
				break;
			}
			case Android_BOX_CONTROL_BEEP:
			{
				debug_print("Android_BOX_CONTROL_BEEP \r\n");
				buff[0] = BOX_SUCCESS;
				can_send_one_pkg_to_Android_by_link(pmsg->ex_id._bit.png_cmd, pmsg->ex_id._bit.msg_id, buff, 1);
				buff[0] = beep_control(pmsg->data[0]);
				break;
			}
			case Android_BOX_UPDATE:
			{
				debug_print("Android_BOX_UPDATE \r\n");
				box_update(pmsg);
				break;
			}
			case Android_BOX_FLASH_UPDATE:
			{
				debug_print("Android_BOX_FLASH_UPDATE \r\n");
				flash_addr =  *(__IO uint16_t*)(&(pmsg->data[0]));
				if(!pOled_Func->updataPic_opt(&(pmsg->data[2]),&flash_addr))
					buff[0] = BOX_SUCCESS;
				else
					buff[0]  = 0x02;
				can_send_one_pkg_to_Android_by_link(pmsg->ex_id._bit.png_cmd, pmsg->ex_id._bit.msg_id, buff, 1);
				break;
			}
			case Android_BOX_DISPLAY_DATA:
			{
				debug_print("Android_BOX_DISPLAY_DATA \r\n");
				dispStr.id = pmsg->data[0];
				dispStr.startRow = pmsg->data[1];
				dispStr.startCol = pmsg->data[2];
				dispStr.dispLen = pmsg->byte_count;
				memcpy(dispStr.data,pmsg->data+3,pmsg->byte_count);
				
				buff[0]  = pOled_Func->directlyDisp_opt(dispStr);
				can_send_one_pkg_to_Android_by_link(pmsg->ex_id._bit.png_cmd, pmsg->ex_id._bit.msg_id, buff, 1);

				break;
			}
			case Android_BOX_DISPLAY_FLASH:
			{
				debug_print("Android_BOX_DISPLAY_FLASH \r\n");
				debug_print("pmsg->byte_count== %d\r\n",pmsg->byte_count);
				debug_print_hex(pmsg->data, pmsg->byte_count);
				debug_print("\r\n");
				dispStr.id = pmsg->data[0];
				for(i=0;i<(pmsg->byte_count/8);i++)
				{			
					dispStr.cmd = pmsg->data[1+i*8];
					dispStr.startRow = pmsg->data[2+i*8];
					dispStr.startCol = pmsg->data[3+i*8];
					if(dispStr.cmd ==0x03)
					{
						dispStr.fontSize = pmsg->data[4+i*8];
						dispStr.dispLen = pmsg->data[5+i*8];
						memcpy(dispStr.data,pmsg->data+6,dispStr.dispLen);
					}
					else
					{
						dispStr.endRow = pmsg->data[4+i*8];
						dispStr.endCol = pmsg->data[5+i*8];
						pmsg->data[9+i*8] = 0;
						dispStr.dispAddr =  *(__IO uint32_t*)(&(pmsg->data[6+i*8]));
					}
					buff[0]  = pOled_Func->dispPic_opt(dispStr);
				}
					
				
				can_send_one_pkg_to_Android_by_link(pmsg->ex_id._bit.png_cmd, pmsg->ex_id._bit.msg_id, buff, 1);
				break;
			}
			case Android_BOX_CHECK:
			{
				debug_print("Android_BOX_CHECK \r\n");
				break;
			}
			//升级指令
			case Android_BOX_UPDATE_INFO:
			{
				DisableTask(TASK_RFID_READ);//关闭寻卡任务
				DisableTask(TASK_ADC_CONV);//关闭AD任务
				printf("Android_BOX_UPDATE_INFO \r\n");
				ret_s = pIap_Func->info_opt(pmsg->data, &ret_id);
				iap_simply_ack(pmsg->ex_id.EX_ID, ret_s, ret_id);
				break;
			}
			case Android_BOX_UPDATE_DATA:
			{
				debug_print("Android_BOX_UPDATE_DATA \r\n");
				ret_s = pIap_Func->data_opt(pmsg->data, &ret_id);
				iap_simply_ack(pmsg->ex_id.EX_ID, ret_s, ret_id);
				//show_upgrade_tag(pmsg->data[1]*256 + pmsg->data[0]);
				printf("package_id == %d\r\n",pmsg->data[1]*256 + pmsg->data[0]);
				break;
			}
			case Android_BOX_RADIO_DATA://广播数据包
			{
//				debug_print("Android_BOX_RADIO_DATA \r\n");
//				if(pmsg->data[0]==0xe1||pmsg->data[0]==0xe2)
//					return ;
				ret_s = pIap_Func->data_opt(pmsg->data, &ret_id);
				show_upgrade_tag(pmsg->data[1]*256 + pmsg->data[0]);
				printf("package_id == %d\r\n",pmsg->data[1]*256 + pmsg->data[0]);
				break;
			}
			case Android_BOX_RADIO_CHECK://广播升级包效验
			{
				printf("Android_BOX_RADIO_CHECK \r\n");
				ret_s = pIap_Func->check_opt(&ret_id,&toal_num);
				printf("lost_toal_num = %d\r\n",toal_num);
				iap_check_ack(pmsg->ex_id.EX_ID, ret_s, ret_id,toal_num);
				show_lost_num_tag(toal_num);
				printf("lost package id ==%d\r\n",ret_id);
				break;
			}
			case Android_BOX_UPDATE_CHECK:
			{
				printf("Android_BOX_UPDATE_CHECK \r\n");
				ret_s = pIap_Func->check_opt(&ret_id,&toal_num);
				printf("lost_toal_num = %d\r\n",toal_num);
				iap_simply_ack(pmsg->ex_id.EX_ID, ret_s, ret_id);
				break;
			}
			case Android_BOX_UPDATE_LOST:
			{
				printf("Android_BOX_UPDATE_LOST \r\n");
				ret_s = pIap_Func->patch_opt(pmsg->data, &ret_id);
				iap_simply_ack(pmsg->ex_id.EX_ID, ret_s, ret_id);
				break;
			}
	
			case Android_BOX_UPDATE_RESET:
			{
				printf("Android_BOX_UPDATE_RESET \r\n");
				iap_simply_ack(pmsg->ex_id.EX_ID, 0, 0);	//先响应再进行复位
				HAL_Delay(30);	//等待响应完成			
				ret_s = pIap_Func->reset_opt();	//执行复位操作
				break;
			}
			
			 			
			/***安卓收到box上报的信息****/
			case BOX_Android_UP_CARD_INFO:
			{
				printf("BOX_Android_UP_CARD_INFO \r\n");
				can1_msg_queue_pop(pmsg->ex_id._bit.msg_id);
				break;
			}
			case BOX_Android_UP_DOOR_STATUS:
			{
				debug_print("BOX_Android_UP_DOOR_STATUS \r\n");
				can1_msg_queue_pop(pmsg->ex_id._bit.msg_id);
				break;
			}
			case BOX_Android_UP_CHECK_STATUS:
			{
				debug_print("BOX_Android_UP_CHECK_STATUS \r\n");
				can1_msg_queue_pop(pmsg->ex_id._bit.msg_id);
				break;
			}
			default:
			{
				buff[0] = BOX_UNKNOW_CMD;
				debug_print("error cmd \r\n");
				can_send_one_pkg_to_Android_by_link(pmsg->ex_id._bit.png_cmd, pmsg->ex_id._bit.msg_id, buff, 1);
				break;
			}
		}
	} else { //cc过来的数据
		switch(pmsg->ex_id._bit.png_cmd) {
			case CC_BOX_HEART:
			{
				printf("CC_BOX_HEART \r\n");
				can_sed_heartbeat(pmsg->ex_id._bit.png_cmd, pmsg->ex_id._bit.msg_id);
				break;
			}
			case CC_BOX_SCAN_CONTROL:
			{
				debug_print("CC_BOX_SCAN_CONTROL \r\n");
				break;
			}
			case CC_BOX_ASSIGNED_ADDR:
			{
				config_mcan_addr(pmsg->data[0],pmsg->data[1]);
				MX_CAN_Init(mApp_Param.cc_can_addr, mApp_Param.can_addr);//重新分配地址时，需要重新初始化过滤器
		//	box_report_check_status();//主动上报自检状态
				debug_print("CC_BOX_ASSIGNED_ADDR \r\n");
				break;
			}
			default:
			{
				buff[0] = BOX_UNKNOW_CMD;
				debug_print("error cmd \r\n");
				break;
			}
		}	
	}
}

void can_send_sw_and_hw_to_android(char cmd, char msg_id)
{	
	uint8_t send_buff[8] = {0};

	send_buff[1] = mApp_Param.hard_ver;	//硬件版本号
	send_buff[2] = SOFT_VERSION;	//软件版本号
	send_buff[0] = BOX_SUCCESS;
	can_send_one_pkg_to_Android_by_link(cmd, msg_id, send_buff, 3);
}

void can_send_door_sta_to_android(char cmd, char msg_id)
{	
	uint8_t send_buff[8] = {0};

	send_buff[1] = get_door_state();	//门状态
	send_buff[0] = BOX_SUCCESS;
	can_send_one_pkg_to_Android_by_link(cmd, msg_id, send_buff, 2);
}

void can_send_temp_sta_to_android(char cmd, char msg_id)
{	
	uint8_t send_buff[8] = {0};
	uint16_t temp = 0;

	temp = get_adc_temp();
	send_buff[1] = (uint8_t)(temp & 0x00FF);	//实际温度
	send_buff[2] = (uint8_t)(temp & 0x0F00) >> 8;	//实际温度
	send_buff[0] = BOX_SUCCESS;
	can_send_one_pkg_to_Android_by_link(cmd, msg_id, send_buff, 3);
}

void can_sed_heartbeat(char cmd, char msg_id)
{
	uint8_t sed_buff[8] = {0}; 
	uint16_t temp = 0;
	
	sed_buff[0] = BOX_SUCCESS;
	sed_buff[1] = (get_door_state() & 0x03);	//门状态
	temp = get_adc_temp();
	sed_buff[2] = (uint8_t)((temp>>8)&0x00FF);	//温度
	sed_buff[3] = (uint8_t)((temp)&0x00FF);			//温度
	
	temp = get_adc_ev();
	sed_buff[4] = (uint8_t)((temp>>8)&0x00FF);	//电流
	sed_buff[5] = (uint8_t)((temp)&0x00FF);			//电流 
	
	sed_buff[6] = get_box_checkStatus();	//餐格状态
	can_send_one_pkg_to_cc_by_link(cmd, msg_id, sed_buff, 7);
}

static void box_update(void* ret_msg)
{
	_pRet_Msg pmsg = ret_msg;
	uint8_t send_buff[8] = {0};

	debug_print("update pkg:");
	debug_print_hex(pmsg->data, pmsg->byte_count);
	debug_print("\n");

	send_buff[0] = BOX_SUCCESS;
	can_send_one_pkg_to_Android_by_link(pmsg->ex_id._bit.png_cmd, pmsg->ex_id._bit.msg_id, send_buff, 1);

}

/*==================================================================================
* 函 数 名： iap_simply_ack
* 参    数： None
* 功能描述:  升级响应
* 返 回 值： None
* 备    注： 
* 作    者： xiaozh
* 创建时间： 2021-01-07 025540
==================================================================================*/
static void iap_simply_ack(uint32_t can_id, uint8_t ret_reuslt, uint16_t ret_id)
{
	_Ret_Msg msg;
	msg.ex_id.EX_ID = can_id;
	
	if(ret_reuslt == 0)
	{
		msg.data[0] = 0;	//正常
		msg.byte_count = 0x01;
	}
	else
	{
		msg.data[0] = 0x08;	//异常
		msg.data[1] = ret_reuslt;	//状态值
		msg.byte_count = 0x02;
		if(ret_id != 0)
		{
			msg.data[2] = (uint8_t)((ret_id>>0)&0x00FF);	//状态值参数
			msg.data[3] = (uint8_t)((ret_id>>8)&0x00FF);
			msg.byte_count = 0x04;
		}
	} 
	
	can_send_one_pkg_to_Android_by_link(msg.ex_id._bit.png_cmd, msg.ex_id._bit.msg_id, msg.data, msg.byte_count);
}

/*==================================================================================
* 函 数 名： iap_check_ack
* 参    数： None
* 功能描述:  升级响应
* 返 回 值： None
* 备    注： 
* 作    者： lc
* 创建时间： 2021-03-04 025540
==================================================================================*/
static void iap_check_ack(uint32_t can_id, uint8_t ret_reuslt, uint16_t ret_id, uint16_t toal_num)
{
	_Ret_Msg msg;
	msg.ex_id.EX_ID = can_id;
	
	if(toal_num == 0)
	{
		msg.data[0] = 0;	//正常
		msg.byte_count = 0x01;
	}
	else
	{
		msg.data[0] = 0x08;	//异常
		msg.data[1] = 0x30;	//状态值
		msg.byte_count = 6;
	//	if(ret_id != 0)
		{
			msg.data[2] = (uint8_t)((toal_num>>0)&0x00FF);	//丢失的总包好
			msg.data[3] = (uint8_t)((toal_num>>8)&0x00FF);
			msg.data[4] = (uint8_t)((ret_id>>0)&0x00FF);	//丢失的最小包号
			msg.data[5] = (uint8_t)((ret_id>>8)&0x00FF);
			msg.byte_count = 0x06;
		}
	} 
	
	can_send_one_pkg_to_Android_by_link(msg.ex_id._bit.png_cmd, msg.ex_id._bit.msg_id, msg.data, msg.byte_count);
}