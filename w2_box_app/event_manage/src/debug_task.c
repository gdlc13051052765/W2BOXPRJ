#include "debug_task.h"
#include "main.h"
#include "def_fifo.h"


DEF_SAFE_FIFO_U8(cmd_fifo, uint16_t, uint8_t)


 
static _cmd_cache mCmd_Cache = 
{
	.is_value				= 0,	//�Ƿ������Ч����
	.rev_count			= 0,
	.r_point				= 0,
};

//�����ʽ  [�����ַ���]�ո�[����1]�ո�[����2]�ո�[...]\r\n 

static void DEF_NAME_FUNC(help)(void *argv);
static void DEF_NAME_FUNC(shell)(void *argv);

_cmd_list  mCmd_List_Table[] = 
{
	CMD_FUNC_DEF(help),
	CMD_FUNC_DEF(shell),
};

//help
static void DEF_NAME_FUNC(help)(void *argv)
{
	printf("cmd frame: [�����ַ���] [����1] [����2] ...\n");
}
//entyr shell
static void DEF_NAME_FUNC(shell)(void *argv)
{
	printf("cmd frame: [�����ַ���] [����1] [����2] ...\n");
}

/*==================================================================================
* �� �� ���� push_cmd_cache
* ��    ���� None
* ��������:  �ѽ��յ��������ַ��ŵ�����
* �� �� ֵ�� None
* ��    ע�� None
* ��    �ߣ� xiaozh
* ����ʱ�䣺 2019-11-12 172859
==================================================================================*/
void push_cmd_cache(uint8_t ch)
{
	if(ch == '\r')
	{
		mCmd_Cache.is_value  = 0x01;		//�ӵ�����������
	}
	
	mCmd_Cache.rev_buff[mCmd_Cache.r_point++] = ch;
	mCmd_Cache.rev_count++;
	mCmd_Cache.is_value++;
}

/*==================================================================================
* �� �� ���� execute_cmd
* ��    ���� None
* ��������:  �����
* �� �� ֵ�� None
* ��    ע�� None
* ��    �ߣ� xiaozh
* ����ʱ�䣺 2019-11-12 172859
==================================================================================*/
static void execute_cmd(const char *cmd_buf, uint16_t len)
{

}

/*==================================================================================
* �� �� ���� cmd_string_handle
* ��    ���� None
* ��������:  �ַ��������
* �� �� ֵ�� None
* ��    ע�� None
* ��    �ߣ� xiaozh
* ����ʱ�䣺 2019-11-12 172859
==================================================================================*/ 
p_func find_func(char *cmd_str)
{ 
	for(int i=0; i<  MAX_NUM(mCmd_List_Table); i++)
	{ 
		printf("cmd = %s\n", mCmd_List_Table[i].cmd_name);
	}
	
	return 0;
}


/*==================================================================================
* �� �� ���� cmd_handle_func_task
* ��    ���� None
* ��������:  �ַ��������
* �� �� ֵ�� None
* ��    ע�� None
* ��    �ߣ� xiaozh
* ����ʱ�䣺 2019-11-12 172859
==================================================================================*/ 
void cmd_handle_func_task(void)
{
	p_func pfunc = 0;
	char cmd_str[16] = {0};
	uint8_t param_tab[8] = {0};
	uint8_t *p = 0;
	uint8_t point = 0;
	
	if(!mCmd_Cache.is_value)
	{
		return ;
	}
	
	//�����������
	do{
		
		//��ȡ����
		do{cmd_str[point++] =  mCmd_Cache.rev_buff[mCmd_Cache.r_point++]; mCmd_Cache.rev_count--;}while(cmd_str[point-1] != '\r');
		
		//���Һ���
		pfunc = find_func(cmd_str);
		
		//ִ������
		pfunc(param_tab);
		
		//��������
		(mCmd_Cache.is_value>0)?(mCmd_Cache.is_value--):(mCmd_Cache.is_value=0);
	}while(mCmd_Cache.is_value);
	
	//��ջ���
	mCmd_Cache.rev_count = 0;
	mCmd_Cache.r_point = 0;
	 
}

