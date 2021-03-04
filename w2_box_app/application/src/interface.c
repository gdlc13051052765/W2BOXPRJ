#include "interface.h"
#include "includes.h"
#include "st_flash.h"

extern _App_Param mApp_Param;
timer_t mx_time;

/*==================================================================================
* �� �� ���� inter_init
* ��    ���� None
* ��������:  �ӿ�Э���ʼ��
* �� �� ֵ�� None
* ��    ע�� Ĭ�ϼ��10ms���н������ݽ���
* ��    �ߣ� xiaozh
* ����ʱ�䣺 2019-09-23 180348
==================================================================================*/
void inter_init(void)
{  
}

/*==================================================================================
* �� �� ���� write_default_config_param
* ��    ���� None
* ��������:  д��Ĭ�����ò���
* �� �� ֵ�� None
* ��    ע�� ���û�ж�ȡ����ȷ�����ò�������д��Ĭ�ϵ����ò���
* ��    �ߣ� xiaozh
* ����ʱ�䣺 2019-11-08 121017
==================================================================================*/
void write_default_config_param(void)
{
	_App_Config_Param cfg_param;	
	
	cfg_param._Use_Param.can_addr = CAN_ADDR_NULL;		//Ĭ��Ϊ�յ�ַ
	cfg_param._Use_Param.can_addr_n = (uint8_t)(~cfg_param._Use_Param.can_addr);		//Ĭ��Ϊ�յ�ַ

	cfg_param._Use_Param.cc_can_addr = CAN_ADDR_NULL;		//Ĭ��Ϊ�յ�ַ
	cfg_param._Use_Param.cc_can_addr_n = (uint8_t)(~cfg_param._Use_Param.cc_can_addr);

	cfg_param._Use_Param.hard_ver = HARD_VERSION;		//Ӳ���汾
	cfg_param._Use_Param.hard_ver_n = (uint8_t)(~cfg_param._Use_Param.hard_ver);		//Ӳ���汾

	cfg_param._Use_Param.is_heat = 0;		
	cfg_param._Use_Param.is_heat_n = (uint8_t)(~cfg_param._Use_Param.is_heat);		

	cfg_param._Use_Param.rfid_retry_count = 2;		
	cfg_param._Use_Param.rfid_retry_count_n = (uint8_t)(~cfg_param._Use_Param.rfid_retry_count);

	cfg_param._Use_Param.cc_can_addr = CAN_ADDR_NULL;		//Ĭ��Ϊ�յ�ַ
	cfg_param._Use_Param.cc_can_addr_n = (uint8_t)(~cfg_param._Use_Param.cc_can_addr);		

	cfg_param._Use_Param.ant_adjust = 2;		
	cfg_param._Use_Param.ant_adjust_n = (uint8_t)(~cfg_param._Use_Param.ant_adjust);		
 
	cfg_param._Use_Param.crc_data_len = 16;		
	cfg_param._Use_Param.crc_data_len_n = (uint8_t)(~cfg_param._Use_Param.crc_data_len);		

	cfg_param._Use_Param.crc32 = crc32(cfg_param.flash_buff, cfg_param._Use_Param.crc_data_len);		
	 
	//����
	stFlash_Func.erase(APP_CONFIG_ADDR, MAX_USE_FLASH_SIZE*2);	//����������������
	//д�����ò���
	stFlash_Func.write(APP_CONFIG_ADDR, cfg_param.flash_buff, MAX_USE_FLASH_SIZE*2);	//д�����
}

/*==================================================================================
* �� �� ���� read_config_param
* ��    ���� _pApp_Config_Param
* ��������:  ��ȡ���ò���
* �� �� ֵ�� None
* ��    ע�� 
* ��    �ߣ� xiaozh
* ����ʱ�䣺 2019-09-06 115746
==================================================================================*/
uint8_t read_config_param(void* c_param)
{  
	_pApp_Param pmsg = c_param;
	
	stFlash_Func.read(APP_CONFIG_ADDR, pmsg->config_param.flash_buff, MAX_USE_FLASH_SIZE*2);	//��ȡ����
	
	//�жϲ����Ƿ�Ϸ�
	if(crc32(pmsg->config_param.flash_buff, pmsg->config_param._Use_Param.crc_data_len) == pmsg->config_param._Use_Param.crc32)	//�ж�У���Ƿ�Ϸ�
	{
		//�ж�ÿһ������Ƿ�Ϸ�
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
		//У�����ʹ��Ĭ�����ò���
		pmsg->can_addr = CAN_ADDR_NULL;
		pmsg->hard_ver = 0xFF;		//Ĭ��Ӳ���汾Ϊ0xFF��Ϊ�������Ƿ�ʹ�õ�Ĭ�����ò���
		pmsg->is_heat = 0;
		pmsg->rfid_retry_count = 0x2;
		pmsg->heat_set_temp = 0;
		pmsg->ant_adjust = 0x02; 
		
		//д��Ĭ�����ò���
		write_default_config_param();
	}
	return 0;
}

/*==================================================================================
* �� �� ���� config_mcan_addr
* ��    ���� uint8_t
* ��������:  ����CAN���ߵ�ַ
* �� �� ֵ�� None
* ��    ע�� 
* ��    �ߣ� xiaozh
* ����ʱ�䣺 2019-11-08 115948
==================================================================================*/
ret_msg_t config_mcan_addr(uint8_t cc_addr, uint8_t c_addr)
{
	ret_msg_t ret = BOX_UNKNOW_PARAM;
	_pApp_Param pmsg = &mApp_Param;
	
	if((mApp_Param.cc_can_addr == cc_addr) && (mApp_Param.can_addr == c_addr))//�������ĵ�ַ��ȫ��ȣ������ظ�д��
	{
		debug_print("the cc addr is %d lc addr is %d\n", cc_addr, c_addr);
		ret = BOX_SUCCESS;
	} else {
		//�޸Ļ�����Ϣ
		mApp_Param.cc_can_addr = cc_addr;
		mApp_Param.config_param._Use_Param.cc_can_addr = cc_addr;
		mApp_Param.config_param._Use_Param.cc_can_addr_n = ~ mApp_Param.config_param._Use_Param.cc_can_addr;

		mApp_Param.can_addr = c_addr;
		mApp_Param.config_param._Use_Param.can_addr = c_addr;
		mApp_Param.config_param._Use_Param.can_addr_n = (uint8_t)(~c_addr);
		
		//���¼���У��ֵ 
		mApp_Param.config_param._Use_Param.crc32 = crc32(pmsg->config_param.flash_buff, pmsg->config_param._Use_Param.crc_data_len);
		
		//д�����ò���
			//����
		stFlash_Func.erase(APP_CONFIG_ADDR, MAX_USE_FLASH_SIZE*2);	//����������������
		//д�����ò���
		stFlash_Func.write(APP_CONFIG_ADDR, mApp_Param.config_param.flash_buff, MAX_USE_FLASH_SIZE*2);	//д�����
		ret = BOX_SUCCESS;
	}
	
	return ret;
}
 
/*==================================================================================
* �� �� ���� can_rev_decode
* ��    ���� None
* ��������:  canЭ�����
* �� �� ֵ�� None
* ��    ע�� none
* ��    �ߣ� xiaozh
* ����ʱ�䣺 2019-10-28 172557
==================================================================================*/
void can_rev_decode(void)
{
	_Ret_Msg c_msg; 
	 
	if(can_pop_one_frame(hcan1, &c_msg) < 0xff)		//��ȡ����
	{
		//Э�����
		can_frame_parse(&c_msg);		
	}
}

/*==================================================================================
* �� �� ���� can_sed_loop_task
* ��    ���� None
* ��������:  CAN��ʱ��������
* �� �� ֵ�� None
* ��    ע�� Ĭ�ϼ��20ms����һ��
* ��    �ߣ� xiaozh
* ����ʱ�䣺 2019-09-23 180348
==================================================================================*/
void can_sed_loop_task(void* argv)
{
	can_sed_loop_check(hcan1);	
}

/*==================================================================================
* �� �� ���� HAL_CAN_RxFifo0MsgPendingCallback
* ��    ���� None
* ��������:  �������
* �� �� ֵ�� None
* ��    ע�� 
* ��    �ߣ�  
* ����ʱ�䣺 2019-09-23 180101
==================================================================================*/
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* hcan) 
{
	_Can_Msg Can_Msg;
	CAN_RxHeaderTypeDef mCan_Header;
	
	//��ȡ����
	HAL_CAN_GetRxMessage(hcan,CAN_RX_FIFO0, &mCan_Header, (uint8_t*)Can_Msg.data);
	Can_Msg.ex_id.EX_ID = mCan_Header.ExtId;
	Can_Msg.byte_count = mCan_Header.DLC;
	
	//debug_print("can_IRQ,");
	switch(mCan_Header.FilterMatchIndex)
	{
		case 0://�㲥���в͸�
		case 1://�㲥һ��cc�µ����в͸�
		case 2://��׿->box ���ݰ�
		case 4://CC-->�͸�㲥������
		case 5://CC-->�͸����ݰ�����
			//debug_print("Android to cc\r\n");
			can_recv_signal_frame(hcan1, &Can_Msg);
		break;

		case 3://��׿->box�������
			//debug_print("Android to box data\r\n");
			can_recv_mutil_frame(hcan1, &Can_Msg);
		break;

		default:
			//debug_print("error data\r\n");
		break;

	}
	
	//ʹ���ж�
	__HAL_CAN_ENABLE_IT(hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
}

/*==================================================================================
* �� �� ���� HAL_CAN_RxFifo0MsgPendingCallback
* ��    ���� None
* ��������:  �������
* �� �� ֵ�� None
* ��    ע�� ����
* ��    �ߣ�  
* ����ʱ�䣺 2019-09-23 180101
==================================================================================*/
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef* hcan) 
{
	_Can_Msg Can_Msg;
	CAN_RxHeaderTypeDef mCan_Header;
	
	//��ȡ����
	HAL_CAN_GetRxMessage(hcan,CAN_RX_FIFO1, &mCan_Header, (uint8_t*)Can_Msg.data);
	Can_Msg.ex_id.EX_ID = mCan_Header.ExtId;
	Can_Msg.byte_count = mCan_Header.DLC;

	can_recv_mutil_frame(hcan1, &Can_Msg);
	
	//ʹ���ж�
	__HAL_CAN_ENABLE_IT(hcan, CAN_IT_RX_FIFO1_MSG_PENDING);
}
 

void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
{
	
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(htim ->Instance == TIM4)
  {
  	if(mx_time.motor_time < 130)   //�����400msʱ��ִ������
  	{
  		mx_time.motor_time ++;
  	}
	else {
		MOTOR_DISABLE_OUT();
	}
  	led_change();
  }
}


