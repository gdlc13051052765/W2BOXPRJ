#include "debug_task.h"
#include "main.h"
#include "def_fifo.h"


DEF_SAFE_FIFO_U8(cmd_fifo, uint16_t, uint8_t)


 
static _cmd_cache mCmd_Cache = 
{
	.is_value				= 0,	//是否接收有效命令
	.rev_count			= 0,
	.r_point				= 0,
};

//命令格式  [命令字符串]空格[参数1]空格[参数2]空格[...]\r\n 

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
	printf("cmd frame: [命令字符串] [参数1] [参数2] ...\n");
}
//entyr shell
static void DEF_NAME_FUNC(shell)(void *argv)
{
	printf("cmd frame: [命令字符串] [参数1] [参数2] ...\n");
}

/*==================================================================================
* 函 数 名： push_cmd_cache
* 参    数： None
* 功能描述:  把接收到的命令字符放到缓存
* 返 回 值： None
* 备    注： None
* 作    者： xiaozh
* 创建时间： 2019-11-12 172859
==================================================================================*/
void push_cmd_cache(uint8_t ch)
{
	if(ch == '\r')
	{
		mCmd_Cache.is_value  = 0x01;		//接到的完整命令
	}
	
	mCmd_Cache.rev_buff[mCmd_Cache.r_point++] = ch;
	mCmd_Cache.rev_count++;
	mCmd_Cache.is_value++;
}

/*==================================================================================
* 函 数 名： execute_cmd
* 参    数： None
* 功能描述:  命令处理
* 返 回 值： None
* 备    注： None
* 作    者： xiaozh
* 创建时间： 2019-11-12 172859
==================================================================================*/
static void execute_cmd(const char *cmd_buf, uint16_t len)
{

}

/*==================================================================================
* 函 数 名： cmd_string_handle
* 参    数： None
* 功能描述:  字符串命令处理
* 返 回 值： None
* 备    注： None
* 作    者： xiaozh
* 创建时间： 2019-11-12 172859
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
* 函 数 名： cmd_handle_func_task
* 参    数： None
* 功能描述:  字符串命令处理
* 返 回 值： None
* 备    注： None
* 作    者： xiaozh
* 创建时间： 2019-11-12 172859
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
	
	//进行命令解析
	do{
		
		//获取命令
		do{cmd_str[point++] =  mCmd_Cache.rev_buff[mCmd_Cache.r_point++]; mCmd_Cache.rev_count--;}while(cmd_str[point-1] != '\r');
		
		//查找函数
		pfunc = find_func(cmd_str);
		
		//执行命令
		pfunc(param_tab);
		
		//修正参数
		(mCmd_Cache.is_value>0)?(mCmd_Cache.is_value--):(mCmd_Cache.is_value=0);
	}while(mCmd_Cache.is_value);
	
	//清空缓存
	mCmd_Cache.rev_count = 0;
	mCmd_Cache.r_point = 0;
	 
}

