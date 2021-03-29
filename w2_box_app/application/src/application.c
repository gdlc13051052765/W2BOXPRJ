#include "application.h"
#include "includes.h"


_App_Param mApp_Param= 
{
	.sys_init_complete = 0,	//系统初始化标志，没有初始化完成，不允许执行中断
	.can_addr 		= 2,			//空地址
};

sys_info_t g_sys_info;

/*==================================================================================
* 函 数 名： systerm_init
* 参    数： None
* 功能描述:  系统初始化
* 返 回 值： None
* 备    注： 
* 作    者： xiaozh
* 创建时间： 2019-09-24 111301
==================================================================================*/
void systerm_init(void)
{
//中断向量表初始化
#if  EN_DEBUG
	SCB->VTOR = FLASH_BASE | 0X3000; /* Vector Table Relocation in Internal FLASH. */
#else

#endif
	
	HAL_Init();

	__disable_irq();   // 关闭总中断

	//读取配置参数
	read_config_param(&mApp_Param);
	//系统板级初始化
	bsp_init();
	st_crc_init(); 

	//can协议初始化
	can_instance_init(hcan1);
	
	//外设初始化
	inter_init();
	
	//使能外部中断
	__enable_irq();    // 开启总中断
	
	//初始化OLED, 字库
	oled_gt_init();
//	oled_gt_assic_init();

//	//关门
//	door_close();

	//读卡器初始化
	rfid_st25r3916_init();
	rfid_context_init();
	
	//开启任务
//	EnableTask(TASK_RFID_READ);

	//系统初始化完成
	mApp_Param.sys_init_complete = 0x01;
	mApp_Param.rfid_retry_count = 0x02;
	printf("slave box init complete\n");
	
	//运行系统中断
	HAL_Delay(100+((mApp_Param.can_addr<10)?(mApp_Param.can_addr*10):(100)));
	
	//使能中断
	systerm_init_complete();

	printf("can addr = %d CC ADDR IS = %d\n", mApp_Param.can_addr, mApp_Param.cc_can_addr);
	//main_oled_test();
}

/*==================================================================================
* 函 数 名： HAL_SysTick_Callback
* 参    数： None
* 功能描述:  系统滴答回调
* 返 回 值： None
* 备    注： 
* 作    者： xiaozh
* 创建时间： 2019-09-06 115746
==================================================================================*/
void HAL_SysTick_Callback(void)
{
	//标记任务
	TaskRemarks();
}

/*==================================================================================
* 函 数 名： app_dispatch
* 参    数： None
* 功能描述:  系统任务调度
* 返 回 值： None
* 备    注： 
* 作    者： xiaozh
* 创建时间： 2019-09-06 115746
==================================================================================*/
void app_dispatch(void)
{
	//定时任务查询
	TaskProcess(0);
	//scan_rfid_tag();
	//协议解析
	//can_sed_loop_check(hcan1);
}
 
/*==================================================================================
* 函 数 名： test_loop_task
* 参    数： None
* 功能描述:  测试任务
* 返 回 值： None
* 备    注： 
* 作    者： xiaozh
* 创建时间： 2019-09-29 170658
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


