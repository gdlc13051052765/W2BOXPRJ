#ifndef  __APPLICATION_H
#define  __APPLICATION_H
#include <stdint.h>
#include "board_info.h"

#define  MAX_FLASH_SIZE				256			//flash�洢����
#define  MAX_USE_FLASH_SIZE		64			//ʵ�ʲ���ʹ�õĿռ�


#pragma pack(1)
typedef union
{
	struct
	{
		uint8_t crc_data_len;	//crcУ�����ݵĳ���
		uint8_t crc_data_len_n;	//crcУ�����ݵĳ���
		
		//ʣ�����ݺ�У��ռ64�ֽ�
		uint8_t can_addr;			//can���ߵ�ַ
		uint8_t can_addr_n;		//can��ַȡ����У��ʹ��

		uint8_t cc_can_addr;		//cc���ߵ�ַ
		uint8_t cc_can_addr_n;		//can��ַȡ����У��ʹ��

		uint8_t  hard_ver;		//Ӳ���汾
		uint8_t  hard_ver_n;		//Ӳ���汾 ȡ��
		 
		uint8_t  is_heat;				//�Ƿ����
		uint8_t  is_heat_n;				//�Ƿ���� ȡ��

		uint8_t rfid_retry_count;		//rfid�ظ���������
		uint8_t rfid_retry_count_n;
		
		uint16_t heat_set_temp;	//���ȿ���,�趨�¶�ֵ
		uint16_t heat_set_temp_n;	//���ȿ���,�趨�¶�ֵ ȡ��
		
		uint16_t ant_adjust;	//RFID���ߵ������� 
		uint16_t ant_adjust_n;	//RFID���ߵ�������  ȡ��
		
		//����ռ��16byte
		
		//����ӵ�����
		
		uint32_t crc32;				//У����������
		
		uint8_t res_buff[MAX_USE_FLASH_SIZE-24];		//�����ռ�, 20ָ�Ѿ�ռ�õĿռ�
	}_Use_Param;
	
	struct
	{
		uint8_t null_buff[MAX_USE_FLASH_SIZE];	//�洢��
		uint8_t copy_param[MAX_USE_FLASH_SIZE];	//���ݲ�����
	}_copy_Param;
	
	uint8_t flash_buff[MAX_FLASH_SIZE];
}_App_Config_Param,*_pApp_Config_Param;
#pragma pack()


typedef struct
{
	//flash �洢����
	uint8_t can_addr;			//CAN���ߵ�ַ
	uint8_t cc_can_addr;		//cc���ߵ�ַ
	uint8_t hard_ver;			//Ӳ���汾
	uint8_t  is_heat;				//�Ƿ����
	uint16_t heat_set_temp;	//���ȿ���,�趨�¶�ֵ
	uint16_t ant_adjust;	//RFID���ߵ�������
	uint8_t rfid_retry_count;
	_App_Config_Param config_param;	//��������ı䣬��Ҫ���µ�config_param���ڸ��µ�flash
		
	//ȫ�ֱ���
	uint8_t sys_init_complete;		//ϵͳ��ʼ�����
}_App_Param, *_pApp_Param;

extern _App_Param mApp_Param;

void systerm_init(void);
void app_dispatch(void);


#define 	EN_DEBUG                (1)   //0:����          1:��������
#define 	EN_WWDG                 (0)   //0:�رտ��Ź�     1:ʹ�ܿ��Ź�

#if EN_DEBUG == 0
	#define debug_print		printf
	#define debug_print_hex  print_hex
#else
	#define debug_print	
	#define debug_print_hex  	
#endif
	

#endif
