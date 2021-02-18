#ifndef  __CAN_DRV_H
#define  __CAN_DRV_H
#include <stdint.h>
#include "main.h"


#define  CAN_GLOBAL_ADDR			0xFF		//can����ȫ�ֵ�ַ
#define  CAN_MASTER_ADDR			0xAE		//���ڵ��ַ
#define  CAN_ADDR_NULL				0xFE					//�յ�ַ 
#define  CAN_STD_ID						0x12					//��׼֡ID
#define  CAN_FRAME_TYPE			CAN_ID_EXT		//��������ͨ�Ŷ�ʹ����չ֡��������250kbps


//�ӻ���������
#define SIGNAL_SLAVE_FILTERID			0x0000FF00		//���ù��ˣ������մӻ�����,ֻ����������ַ
#define SIGNAL_SLAVE_MASK					0x0300AE00		//�������룬�����մӻ����ݣ�ֻ���յ�֡����


#define MUTIL_SLAVE_FILTERID			0x0300FF00		//���ù��ˣ������մӻ�����,ֻ����������ַ 
#define MUTIL_SLAVE_MASK0					0x0100AE00		//�������룬�����մӻ����ݣ�ֻ���յ�֡����
#define MUTIL_SLAVE_MASK1					0x0200AE00		//�������룬�����մӻ����ݣ�ֻ���յ�֡����
 
#define MAX_CAN_NUM							8+1	//�����򳤶�
#define MAX_CACHE_NUM						32	//��󻺴��
#define MAX_RX_CAN_NUM					256		//���can���շ���buff


typedef union
{
	struct
	{
		//
		uint32_t res:5;			//����λ
		uint32_t msg_id:2;		//��ϢID������ֹ����ָ���λظ���Ϊ0-3��ͨ�ŷ������ɲ�����ͨ�Ž��շ�ʹ�ý��յ���msg_id��			
		uint32_t pkg_id:5;		//����ID������ʹ��0
		uint32_t is_end:1;		//�Ƿ�Ϊ������������Ϊ1
		uint32_t s2_addr:4;		  //��ַ2
		uint32_t s1_addr:4;			//��ַ1
		uint32_t png_cmd:8;			//������ 
		uint32_t no_se:3;		//����ʹ��

	}_bit;
	
	uint32_t EX_ID;			//29λ
}_Ex_id,*_pEx_id;

typedef struct
{
	_Ex_id  ex_id;		//��չ֡ID	
	
	uint8_t  byte_count;
	uint8_t  data[MAX_CAN_NUM];
}_Can_Msg,*_pCan_Msg;
 

 
uint8_t  can_sed_pkg_without_cache(CAN_HandleTypeDef can, void* can_msg);
#endif
