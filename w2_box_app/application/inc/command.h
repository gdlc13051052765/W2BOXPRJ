#ifndef  __COMMAND_H
#define  __COMMAND_H
#include <stdint.h>

#define BOX_MAX_INDEX 			0x27		//BOX的最大索引值
#define SL_INDEX						0x80				//中继索引

//CAN通信命令
typedef enum
{
	//安卓发起命令0x00--0x7F
	//安卓发起到CC的命令 0x00--0x1F
	Android_CC_GET_SW_AND_HW_VER					= 0x01,
	Android_CC_EMPTY_ADDR_AND_UP_CPUID,
	Android_CC_ASSIGNED_ADDR,
	Android_CC_CONTROL_BEEP,
	Android_CC_CONTROL_BOX_POWER,
	Android_CC_HEART,
	Android_CC_GET_BOX_STATUS,
	Android_CC_UPDATE,
	Android_CC_SCAN_CONTROL,
	Android_CC_GET_BOX_CURRENT,

	//安卓发起到BOX的命令 0x20--0x7F
	Android_BOX_GET_SW_AND_HW_VER					= 0x20,
	Android_BOX_CONTROL_LED,
	Android_BOX_CONTROL_DOOR,
	Android_BOX_GET_DOOR_STATUS,
	Android_BOX_CONTROL_HEAT,
	Android_BOX_GET_HEAT,
	Android_BOX_CONTROL_DISPLAY,
	Android_BOX_CONTROL_CARD_READER,
	Android_BOX_CONTROL_BEEP,
	Android_BOX_UPDATE,
	Android_BOX_FLASH_UPDATE,
	Android_BOX_DISPLAY_DATA,//0x2B
	Android_BOX_DISPLAY_FLASH,
	Android_BOX_CHECK,
	//升级操作
	Android_BOX_UPDATE_INFO = 0x2E,	
	Android_BOX_UPDATE_DATA = 0x2F,
	Android_BOX_UPDATE_LOST = 0x30,
	Android_BOX_UPDATE_CHECK = 0x31,
	Android_BOX_UPDATE_RESET = 0x32,
	Android_BOX_RADIO_DATA = 0x33,
	Android_BOX_RADIO_CHECK = 0x34,


	//CC发起命令0x80--0xBF
	CC_Android_UP_CPU_ID							= 0x80,
	CC_Android_UP_ADDR,
	CC_Android_UP_BOX_STATUS,

	//CC发起到BOX的命令 0XA0–0xBF
	CC_BOX_HEART									= 0xA0,
	CC_BOX_SCAN_CONTROL,
	CC_BOX_ASSIGNED_ADDR,
	CC_BOX_SAVE_CC_ADDR,

	//餐格发起命令0xC0--0xFF
	BOX_Android_UP_CARD_INFO						= 0xC0,
	BOX_Android_UP_DOOR_STATUS,
	BOX_Android_UP_CHECK_STATUS,
}_Can_Cmd;

typedef enum
{
	box_update_index = 0,	//餐格升级信息
	
	box_tag_info_index = 0x05,	//餐格信息
	
}_SL_Cmd_Index, *_pSL_Cmd_Index;

void serial_cmd_analay(void * instance);
void radio_cmd_analay(void * psi_pkt);
void can_frame_parse(void* ret_msg);
void can_upload_event_to_android(uint32_t host_cmd, uint8_t* buff, uint16_t len);


#endif
