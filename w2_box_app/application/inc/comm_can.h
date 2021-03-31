#ifndef  __COMM_CAN_H
#define  __COMM_CAN_H
#include <stdint.h>
#include <stdio.h>
#include "can_drv.h"
#include "can_def_fifo.h"
#include "can_fifo.h"
#include "application.h"

#define CAN_MAX_CACHE_LEN			32	//CAN发送缓存
#define MAX_MUTIL_LEN   256 
#define MAX_SINGLE_ITEM_NUM	32
#define MAX_MUTIL_ITEM_NUM	10
#define MAX_SED_SIGNAL_LEN	8	//最大能发送的单帧

#define RETRY_TIME				3000 //3s没收到回复重发

#define SAME_ID					1
#define SAME_PKG_ID			2

typedef struct
{
	uint8_t in_use;			//是否正在使用  0：空闲 1：使用中
	_Ex_id  ex_id;		//扩展帧ID	
	uint8_t r_len;
	uint8_t pkg_id;
	uint8_t r_data[8];
}_S_Ring,*_pS_Ring;

typedef struct
{
	uint8_t is_complete;	//是否接收完成 
	uint8_t recv_pkg_num;		//记录接收数据帧的数量
	_Ex_id  ex_id;		//扩展帧ID	
	_S_Ring cache_frame[CAN_MAX_CACHE_LEN];

}_Mutil_Ring,*_pMutil_Ring;
  
typedef _pCan_Msg _pSignal_Ring;

typedef struct
{ 
	_Ex_id  ex_id;		//扩展帧ID	
	
	uint8_t  byte_count;		//实际缓存中的长度
	uint8_t  data[MAX_MUTIL_LEN];
}_Ret_Msg,*_pRet_Msg;		//包含连包和单包

typedef struct
{
	_Ex_id  ex_id;		//接收的id
	uint8_t  tbale_index;	//表索引值，对应缓存
}_Mutil_Table,*_pMutil_Table;

typedef struct
{
	//mark
	uint8_t is_use;			//是否有效  0：无效  1：有效
	uint8_t try_time;		//重发次数
	uint8_t is_ack;			//是否需要ACK  0：不需要  1：需要
	
	//sed_msg
	_Ex_id  ex_id;		//扩展帧ID	
	uint8_t  byte_count;		//实际缓存中的长度
	uint8_t  data[MAX_MUTIL_LEN];
}_Can_Pkg,*_pCan_Pkg;

typedef struct
{
	//发送fifo
	void*     		Sed_Fifo;		//发送fifo
	//接收fifo
	void* 				Rcv_Fifo;	//短包fifo
	
	//fifo函数接口
  _pCan_Fifo_Func  p_ffunc;			//函数接口
	
	//多帧处理缓存
	_pMutil_Ring 	pMutil_Fifo;		//长包fifo
	
	//等待响应包
	_Can_Pkg     packet;					//缓存包
}_Can_Instance, *_pCan_Instance;

extern CAN_HandleTypeDef hcan1;

void can_instance_init(CAN_HandleTypeDef hcan);
uint8_t can_recv_mutil_frame(CAN_HandleTypeDef hcan, void *can_msg);
uint8_t can_recv_signal_frame(CAN_HandleTypeDef hcan, void *can_msg);
uint8_t can_pop_one_frame(CAN_HandleTypeDef hcan, void *ret_msg);
void can_sed_loop_check(CAN_HandleTypeDef hcan);
void can1_msg_queue_pop(uint32_t msg_id);

uint8_t can_send_one_pkg_to_Android_by_link(uint8_t host_cmd, uint8_t msg_id, uint8_t* buff, uint8_t len);
uint8_t can_send_one_pkg_to_cc_by_radio(uint8_t host_cmd, uint8_t msg_id, uint8_t* buff, uint8_t len);
uint8_t can_send_one_pkg_to_cc_by_link(uint8_t host_cmd, uint8_t msg_id, uint8_t* buff, uint8_t len);


#endif
