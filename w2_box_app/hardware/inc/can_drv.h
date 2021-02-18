#ifndef  __CAN_DRV_H
#define  __CAN_DRV_H
#include <stdint.h>
#include "main.h"


#define  CAN_GLOBAL_ADDR			0xFF		//can总线全局地址
#define  CAN_MASTER_ADDR			0xAE		//主节点地址
#define  CAN_ADDR_NULL				0xFE					//空地址 
#define  CAN_STD_ID						0x12					//标准帧ID
#define  CAN_FRAME_TYPE			CAN_ID_EXT		//本网络中通信都使用扩展帧，波特率250kbps


//从机过滤设置
#define SIGNAL_SLAVE_FILTERID			0x0000FF00		//设置过滤，不接收从机数据,只接收主机地址
#define SIGNAL_SLAVE_MASK					0x0300AE00		//设置掩码，不接收从机数据，只接收单帧数据


#define MUTIL_SLAVE_FILTERID			0x0300FF00		//设置过滤，不接收从机数据,只接收主机地址 
#define MUTIL_SLAVE_MASK0					0x0100AE00		//设置掩码，不接收从机数据，只接收单帧数据
#define MUTIL_SLAVE_MASK1					0x0200AE00		//设置掩码，不接收从机数据，只接收单帧数据
 
#define MAX_CAN_NUM							8+1	//数据域长度
#define MAX_CACHE_NUM						32	//最大缓存池
#define MAX_RX_CAN_NUM					256		//最大can接收返回buff


typedef union
{
	struct
	{
		//
		uint32_t res:5;			//保留位
		uint32_t msg_id:2;		//消息ID用来防止单条指令被多次回复，为0-3（通信发起方生成并管理，通信接收方使用接收到的msg_id）			
		uint32_t pkg_id:5;		//长报ID，单包使用0
		uint32_t is_end:1;		//是否为结束包，单包为1
		uint32_t s2_addr:4;		  //地址2
		uint32_t s1_addr:4;			//地址1
		uint32_t png_cmd:8;			//命令字 
		uint32_t no_se:3;		//不能使用

	}_bit;
	
	uint32_t EX_ID;			//29位
}_Ex_id,*_pEx_id;

typedef struct
{
	_Ex_id  ex_id;		//扩展帧ID	
	
	uint8_t  byte_count;
	uint8_t  data[MAX_CAN_NUM];
}_Can_Msg,*_pCan_Msg;
 

 
uint8_t  can_sed_pkg_without_cache(CAN_HandleTypeDef can, void* can_msg);
#endif
