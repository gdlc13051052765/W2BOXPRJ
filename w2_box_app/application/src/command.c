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
* �� �� ���� can_frame_parse
* ��    ���� None
* ��������:  canЭ�����
* �� �� ֵ�� None
* ��    ע�� None
* ��    �ߣ� xiaozh
* ����ʱ�䣺 2019-09-24 162950
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
	//�ж����ݺϷ���
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
	 
	//����Э�����
	if (pmsg->ex_id._bit.s1_addr != 0xD)//��cc����������
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
				buff[0] = temp_control(pmsg->data[1], pmsg->data[0]);//����δʵ��
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
			//����ָ��
			case Android_BOX_UPDATE_INFO:
			{
				DisableTask(TASK_RFID_READ);//�ر�Ѱ������
				DisableTask(TASK_ADC_CONV);//�ر�AD����
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
			case Android_BOX_RADIO_DATA://�㲥���ݰ�
			{
//				debug_print("Android_BOX_RADIO_DATA \r\n");
//				if(pmsg->data[0]==0xe1||pmsg->data[0]==0xe2)
//					return ;
				ret_s = pIap_Func->data_opt(pmsg->data, &ret_id);
				show_upgrade_tag(pmsg->data[1]*256 + pmsg->data[0]);
				printf("package_id == %d\r\n",pmsg->data[1]*256 + pmsg->data[0]);
				break;
			}
			case Android_BOX_RADIO_CHECK://�㲥������Ч��
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
				iap_simply_ack(pmsg->ex_id.EX_ID, 0, 0);	//����Ӧ�ٽ��и�λ
				HAL_Delay(30);	//�ȴ���Ӧ���			
				ret_s = pIap_Func->reset_opt();	//ִ�и�λ����
				break;
			}
			
			 			
			/***��׿�յ�box�ϱ�����Ϣ****/
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
	} else { //cc����������
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
				MX_CAN_Init(mApp_Param.cc_can_addr, mApp_Param.can_addr);//���·����ַʱ����Ҫ���³�ʼ��������
		//	box_report_check_status();//�����ϱ��Լ�״̬
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

	send_buff[1] = mApp_Param.hard_ver;	//Ӳ���汾��
	send_buff[2] = SOFT_VERSION;	//����汾��
	send_buff[0] = BOX_SUCCESS;
	can_send_one_pkg_to_Android_by_link(cmd, msg_id, send_buff, 3);
}

void can_send_door_sta_to_android(char cmd, char msg_id)
{	
	uint8_t send_buff[8] = {0};

	send_buff[1] = get_door_state();	//��״̬
	send_buff[0] = BOX_SUCCESS;
	can_send_one_pkg_to_Android_by_link(cmd, msg_id, send_buff, 2);
}

void can_send_temp_sta_to_android(char cmd, char msg_id)
{	
	uint8_t send_buff[8] = {0};
	uint16_t temp = 0;

	temp = get_adc_temp();
	send_buff[1] = (uint8_t)(temp & 0x00FF);	//ʵ���¶�
	send_buff[2] = (uint8_t)(temp & 0x0F00) >> 8;	//ʵ���¶�
	send_buff[0] = BOX_SUCCESS;
	can_send_one_pkg_to_Android_by_link(cmd, msg_id, send_buff, 3);
}

void can_sed_heartbeat(char cmd, char msg_id)
{
	uint8_t sed_buff[8] = {0}; 
	uint16_t temp = 0;
	
	sed_buff[0] = BOX_SUCCESS;
	sed_buff[1] = (get_door_state() & 0x03);	//��״̬
	temp = get_adc_temp();
	sed_buff[2] = (uint8_t)((temp>>8)&0x00FF);	//�¶�
	sed_buff[3] = (uint8_t)((temp)&0x00FF);			//�¶�
	
	temp = get_adc_ev();
	sed_buff[4] = (uint8_t)((temp>>8)&0x00FF);	//����
	sed_buff[5] = (uint8_t)((temp)&0x00FF);			//���� 
	
	sed_buff[6] = get_box_checkStatus();	//�͸�״̬
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
* �� �� ���� iap_simply_ack
* ��    ���� None
* ��������:  ������Ӧ
* �� �� ֵ�� None
* ��    ע�� 
* ��    �ߣ� xiaozh
* ����ʱ�䣺 2021-01-07 025540
==================================================================================*/
static void iap_simply_ack(uint32_t can_id, uint8_t ret_reuslt, uint16_t ret_id)
{
	_Ret_Msg msg;
	msg.ex_id.EX_ID = can_id;
	
	if(ret_reuslt == 0)
	{
		msg.data[0] = 0;	//����
		msg.byte_count = 0x01;
	}
	else
	{
		msg.data[0] = 0x08;	//�쳣
		msg.data[1] = ret_reuslt;	//״ֵ̬
		msg.byte_count = 0x02;
		if(ret_id != 0)
		{
			msg.data[2] = (uint8_t)((ret_id>>0)&0x00FF);	//״ֵ̬����
			msg.data[3] = (uint8_t)((ret_id>>8)&0x00FF);
			msg.byte_count = 0x04;
		}
	} 
	
	can_send_one_pkg_to_Android_by_link(msg.ex_id._bit.png_cmd, msg.ex_id._bit.msg_id, msg.data, msg.byte_count);
}

/*==================================================================================
* �� �� ���� iap_check_ack
* ��    ���� None
* ��������:  ������Ӧ
* �� �� ֵ�� None
* ��    ע�� 
* ��    �ߣ� lc
* ����ʱ�䣺 2021-03-04 025540
==================================================================================*/
static void iap_check_ack(uint32_t can_id, uint8_t ret_reuslt, uint16_t ret_id, uint16_t toal_num)
{
	_Ret_Msg msg;
	msg.ex_id.EX_ID = can_id;
	
	if(toal_num == 0)
	{
		msg.data[0] = 0;	//����
		msg.byte_count = 0x01;
	}
	else
	{
		msg.data[0] = 0x08;	//�쳣
		msg.data[1] = 0x30;	//״ֵ̬
		msg.byte_count = 6;
	//	if(ret_id != 0)
		{
			msg.data[2] = (uint8_t)((toal_num>>0)&0x00FF);	//��ʧ���ܰ���
			msg.data[3] = (uint8_t)((toal_num>>8)&0x00FF);
			msg.data[4] = (uint8_t)((ret_id>>0)&0x00FF);	//��ʧ����С����
			msg.data[5] = (uint8_t)((ret_id>>8)&0x00FF);
			msg.byte_count = 0x06;
		}
	} 
	
	can_send_one_pkg_to_Android_by_link(msg.ex_id._bit.png_cmd, msg.ex_id._bit.msg_id, msg.data, msg.byte_count);
}