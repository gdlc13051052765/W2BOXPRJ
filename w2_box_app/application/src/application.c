#include "application.h"
#include "includes.h"


_App_Param mApp_Param= 
{
	.sys_init_complete = 0,	//ϵͳ��ʼ����־��û�г�ʼ����ɣ�������ִ���ж�
	.can_addr 		= 2,			//�յ�ַ
};

sys_info_t g_sys_info;

/*==================================================================================
* �� �� ���� systerm_init
* ��    ���� None
* ��������:  ϵͳ��ʼ��
* �� �� ֵ�� None
* ��    ע�� 
* ��    �ߣ� xiaozh
* ����ʱ�䣺 2019-09-24 111301
==================================================================================*/
void systerm_init(void)
{
//�ж��������ʼ��
#if  EN_DEBUG
	SCB->VTOR = FLASH_BASE | 0X3000; /* Vector Table Relocation in Internal FLASH. */
#else

#endif
	
	HAL_Init();

	__disable_irq();   // �ر����ж�

	//��ȡ���ò���
	read_config_param(&mApp_Param);
	//ϵͳ�弶��ʼ��
	bsp_init();
	st_crc_init(); 

	//canЭ���ʼ��
	can_instance_init(hcan1);
	
	//�����ʼ��
	inter_init();
	
	//ʹ���ⲿ�ж�
	__enable_irq();    // �������ж�
	
	//��ʼ��OLED, �ֿ�
	oled_gt_init();
//	oled_gt_assic_init();

//	//����
//	door_close();

	//��������ʼ��
	rfid_st25r3916_init();
	rfid_context_init();
	
	//��������
//	EnableTask(TASK_RFID_READ);

	//ϵͳ��ʼ�����
	mApp_Param.sys_init_complete = 0x01;
	mApp_Param.rfid_retry_count = 0x02;
	printf("slave box init complete\n");
	
	//����ϵͳ�ж�
	HAL_Delay(100+((mApp_Param.can_addr<10)?(mApp_Param.can_addr*10):(100)));
	
	//ʹ���ж�
	systerm_init_complete();

	printf("can addr = %d CC ADDR IS = %d\n", mApp_Param.can_addr, mApp_Param.cc_can_addr);
	//main_oled_test();
}

/*==================================================================================
* �� �� ���� HAL_SysTick_Callback
* ��    ���� None
* ��������:  ϵͳ�δ�ص�
* �� �� ֵ�� None
* ��    ע�� 
* ��    �ߣ� xiaozh
* ����ʱ�䣺 2019-09-06 115746
==================================================================================*/
void HAL_SysTick_Callback(void)
{
	//�������
	TaskRemarks();
}

/*==================================================================================
* �� �� ���� app_dispatch
* ��    ���� None
* ��������:  ϵͳ�������
* �� �� ֵ�� None
* ��    ע�� 
* ��    �ߣ� xiaozh
* ����ʱ�䣺 2019-09-06 115746
==================================================================================*/
void app_dispatch(void)
{
	//��ʱ�����ѯ
	TaskProcess(0);
	//scan_rfid_tag();
	//Э�����
	//can_sed_loop_check(hcan1);
}
 
/*==================================================================================
* �� �� ���� test_loop_task
* ��    ���� None
* ��������:  ��������
* �� �� ֵ�� None
* ��    ע�� 
* ��    �ߣ� xiaozh
* ����ʱ�䣺 2019-09-29 170658
==================================================================================*/
void test_loop_task(void* argv)
{
//	static uint32_t frame_step = 0;
//	static uint8_t count_step = 0;
//	uint8_t s_count = 0;
////  test_send();
//	uint8_t sed_buff[255] = {0};
//	for(int i=0;i<sizeof(sed_buff); i++)
//	{
//		sed_buff[i] = i;
//	}
//	
//	frame_step++;
//	sed_buff[s_count++] = frame_step>>24;
//	sed_buff[s_count++] = frame_step>>16;
//	sed_buff[s_count++] = frame_step>>8;
//	sed_buff[s_count++] = frame_step>>0;

//	//can_sed_link_pkg(0x03, 0xAE,  sed_buff,  count_step++);
	printf("box test \r\n");

}


