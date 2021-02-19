#ifndef  __MINI_DISPATCH_H
#define  __MINI_DISPATCH_H
#include <stdint.h>


typedef enum 
{
	TASK_ENABLE=1,
	TASK_DISABLE
}TASK_STA;

// 任务结构
typedef struct _TASK_COMPONENTS
{
	uint8_t 		Run;                 		//程序运行标记：0-不运行，1运行
	TASK_STA		RunState;								//运行状态，TASK_ENABLE：使能检测    TASK_DISABLE：不进行检测
	//		uint16_t    Priority;								//优先级
	uint32_t 		Timer;              		//计时器
	uint32_t 		ItvTime;              	//任务运行间隔时间，下一次填充时间
	void (*TaskHook)(void*);    		// 要运行的任务函数
} TASK_COMPONENTS;       					// 任务定义


// 任务清单
typedef enum _TASK_LIST
{ 
	TASK_CAN_SED = 0,						//CAN发送数据
	TASK_RFID_READ,							//rfid读卡
	TASK_ADC_CONV,							//ADC转换
	TASK_LOOP_DOOR,							//定时查询门状态
	TASK_SED_TAG_INFO,					//发送卡片信息
	
	TASK_TEST_LOOP,							//循环测试任务
	TASKS_MAX                   // 总的可供分配的定时任务数目
} TASK_LIST;

void TaskSetTimes(TASK_LIST Task , uint32_t Times);
void TaskRefresh(TASK_LIST Task);
uint8_t IsTaskEnable(TASK_LIST Task);
void EnableTask(TASK_LIST Task);
void DisableTask(TASK_LIST Task);
void TaskRemarks(void);
void TaskProcess(void*);
#endif
