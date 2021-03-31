#include "ex_device.h"
#include "led_beep.h"
#include "interface.h"
#include "includes.h"

static int8_t doorStateBak = -1;//门状态备份
static uint8_t checkStatus = 0;//自检状态

ctl_led_msg_t led_ctr={
	.sta=0,
	.max_light=0,
	.light=0,
	.change_speed=0,
};

extern timer_t mx_time;

ret_msg_t temp_control(uint8_t state,uint8_t temp)
{	
	ret_msg_t ret = BOX_UNKNOW_PARAM;
	switch(state & 0x80)
	{
		case 0x00:
		  	g_sys_info.heat_state = 0;
		  	HAL_GPIO_WritePin(PTC_CTRL_GPIO_Port, PTC_CTRL_Pin, GPIO_PIN_RESET);
      		g_sys_info.sys_set_temp = ((state & 0x0F) << 8) | temp;//更新系统温度
      		ret = BOX_SUCCESS;
		break;
		
		case 0x80:
		  	g_sys_info.heat_state = 1;
		  	HAL_GPIO_WritePin(PTC_CTRL_GPIO_Port, PTC_CTRL_Pin, GPIO_PIN_SET);
		  	g_sys_info.sys_set_temp = ((state & 0x0F) << 8) | temp;//更新系统温度
			ret = BOX_SUCCESS;
		break;
		
		default:
			ret = BOX_UNKNOW_PARAM;
		break;
	}
	
	return ret;
}

void heat_auto_control(void)
{
	
	if(g_sys_info.sys_temp < g_sys_info.sys_set_temp)      //温度超过设定温度，关闭主加热
	{
		HAL_GPIO_WritePin(PTC_CTRL_GPIO_Port, PTC_CTRL_Pin, GPIO_PIN_RESET);
		g_sys_info.heat_state = 0;
	}
	else
	{
		HAL_GPIO_WritePin(PTC_CTRL_GPIO_Port, PTC_CTRL_Pin, GPIO_PIN_SET);//温度低于设定温度，开启主加热
		g_sys_info.heat_state = 1;
	}
}

ret_msg_t door_control(uint8_t state)
{
	ret_msg_t ret = BOX_UNKNOW_PARAM;
	switch(state)
	{	
		case 01:
			MOTOR_ENABLE_OUT();
		  	SERVO_SET_PWM(2400);           //open
			mx_time.motor_time = 0;
			ret = BOX_SUCCESS;
		break;
		case 02:
			MOTOR_ENABLE_OUT();
		  	SERVO_SET_PWM(10);            //waiting close
		  	mx_time.motor_time = 0;
		  	ret = BOX_SUCCESS;
		break;
		default: 
			ret = BOX_UNKNOW_PARAM;
		  break;
	}
	return ret;
}
void door_open(void)
{
	door_control(1);
}

void door_close(void)
{
	door_control(2);
}

ret_msg_t led_control(uint8_t state,uint8_t bright)
{	
	ret_msg_t ret = BOX_UNKNOW_PARAM;
	switch(state & 0xF0)
	{
		case 0x00://灭
			led_ctr.sta = 0;
			led_ctr.max_light = 0;
			led_ctr.light = 0;
			led_ctr.change_speed = 0;
			ret = BOX_SUCCESS;		
		break;
		
		case 0x10://亮
			led_ctr.sta = 1;
			led_ctr.max_light = 2 * bright + 2;
			led_ctr.light = led_ctr.max_light;
			led_ctr.change_speed = 0;
			ret = BOX_SUCCESS;
		break;
		
		case 0x20://渐灭
			led_ctr.sta = 2;
			led_ctr.max_light = 2 * bright + 2;
			led_ctr.light = led_ctr.light;
			led_ctr.change_speed = -(state & 0x07) - 1;
			ret = BOX_SUCCESS;
		break;
		
		case 0x30://渐变到设定亮度
			led_ctr.sta = 3;
			led_ctr.max_light = 2 * bright + 2;
			led_ctr.light = led_ctr.light;
			if(led_ctr.light < led_ctr.max_light)
				led_ctr.change_speed = (state & 0x07) + 1;
			else
				led_ctr.change_speed =- (state & 0x07) - 1;
			ret = BOX_SUCCESS;
		break;
		
		case 0x40://持续闪烁，由暗开始
			led_ctr.sta = 4;
			led_ctr.max_light = 2 * bright + 2;
			led_ctr.light = 0;
			led_ctr.change_speed = (state & 0x07) + 1;
			ret = BOX_SUCCESS;
		break;
		
		case 0x50://暗亮暗
			led_ctr.sta = 5;
			led_ctr.max_light = 2 * bright + 2;
			led_ctr.light = 0;
			led_ctr.change_speed = (state & 0x07) + 1;
			ret = BOX_SUCCESS;
		break;
		
		case 0x60://亮暗亮
			led_ctr.sta = 6;
			led_ctr.max_light = 2 * bright + 2;
			led_ctr.light = led_ctr.max_light;
			led_ctr.change_speed = -(state & 0x07) - 1;
			ret = BOX_SUCCESS;
		break;
		
		case 0x70://持续闪烁，由1/8开始
			led_ctr.sta = 7;
			led_ctr.max_light = 2 * bright + 2;
			led_ctr.light = led_ctr.max_light / 8;
			led_ctr.change_speed = (state & 0x07) + 1;
			ret = BOX_SUCCESS;
		break;
		
		case 0x80://持续闪烁，由1/4开始
			led_ctr.sta = 8;
			led_ctr.max_light = 2 * bright + 2;
			led_ctr.light = led_ctr.max_light / 4;
			led_ctr.change_speed = (state & 0x07) + 1;
			ret = BOX_SUCCESS;
		break;
		
		default:
			ret = BOX_UNKNOW_PARAM;
		  break;
	}
	return ret;
}

uint16_t g_led_light[513]={0,178,185,192,198,205,213,220,227,235,243,251,259,268,276,285,294,303,312,322,331,341,351,361,372,382,393,404,415,427,439,450,462,475,487,500,513,526,539,553,567,581,595,610,625,640,655,670,686,702,718,735,751,768,785,803,821,839,857,875,894,913,933,952,972,992,1012,1033,1054,1075,1097,1119,1141,1163,1186,1209,1232,1256,1279,1304,1328,1353,1378,1403,1429,1455,1481,1508,1535,1562,1590,1617,1646,1674,1703,1732,1762,1792,1822,1852,1883,1915,1946,1978,2010,2043,2076,2109,2143,2177,2211,2246,2281,2317,2352,2389,2425,2462,2500,2537,2575,2614,2653,2692,2731,2771,2812,2853,2894,2935,2977,3020,3062,3105,3149,3193,3237,3282,3327,3373,3419,3465,3512,3559,3607,3655,3704,3753,3802,3852,3902,3953,4004,4056,4108,4160,4213,4266,4320,4374,4429,4484,4540,4596,4652,4709,4767,4825,4883,4942,5001,5061,5121,5182,5243,5305,5367,5430,5493,5557,5621,5685,5751,5816,5882,5949,6016,6084,6152,6220,6289,6359,6429,6500,6571,6643,6715,6788,6861,6935,7009,7084,7159,7235,7312,7389,7466,7544,7623,7702,7782,7862,7943,8024,8106,8189,8272,8355,8439,8524,8609,8695,8781,8868,8956,9044,9133,9222,9312,9402,9493,9585,9677,9770,9863,9957,10052,10147,10243,10339,10436,10534,10632,10730,10830,10930,11030,11132,11234,11336,11439,11543,11647,11752,11858,11964,12071,12178,12286,12395,12504,12614,12725,12836,12948,13061,13174,13288,13403,13518,13634,13750,13868,13986,14104,14223,14343,14464,14585,14707,14830,14953,15077,15201,15327,15453,15579,15707,15835,15964,16093,16223,16354,16486,16618,16751,16885,17019,17154,17290,17426,17564,17702,17840,17980,18120,18261,18402,18545,18688,18831,18976,19121,19267,19414,19561,19710,19859,20008,20159,20310,20462,20615,20768,20922,21077,21233,21390,21547,21705,21864,22024,22184,22345,22507,22670,22833,22998,23163,23329,23495,23663,23831,24000,24170,24340,24512,24684,24857,25031,25206,25381,25558,25735,25913,26091,26271,26451,26632,26814,26997,27181,27366,27551,27737,27924,28112,28301,28490,28681,28872,29064,29257,29451,29645,29841,30037,30234,30432,30631,30831,31032,31233,31436,31639,31843,32048,32254,32461,32669,32877,33087,33297,33508,33720,33933,34147,34362,34578,34794,35012,35230,35450,35670,35891,36113,36336,36560,36785,37010,37237,37464,37693,37922,38153,38384,38616,38849,39083,39318,39554,39791,40029,40268,40507,40748,40990,41232,41476,41720,41966,42212,42460,42708,42957,43208,43459,43711,43964,44218,44473,44730,44987,45245,45504,45764,46025,46287,46550,46814,47079,47345,47612,47880,48149,48419,48690,48962,49235,49510,49785,50061,50338,50616,50895,51175,51457,51739,52022,52306,52592,52878,53166,53454,53744,54034,54326,54618,54912,55207,55503,55799,56097,56396,56696,56997,57300,57603,57907,58212,58519,58826,59135,59445,59755,60067,60380,60694,61009,61325,61642,61961,62280,62601,62922,63245,63569,63894,64220,64547,64875,65205,65535};

void led_change(void)//更改LED亮度调节因数
{
	led_ctr.light += led_ctr.change_speed;//根据因数调整亮度
	switch(led_ctr.sta)
	{	
		case 2://渐灭
			if(led_ctr.light <= 0)
			{
				led_ctr.light = 0;
				led_ctr.change_speed = 0;
			}
		break;
		
		case 3://渐变到设定亮度
			if((led_ctr.max_light - led_ctr.light) / led_ctr.change_speed < 1)
			{
				led_ctr.light = led_ctr.max_light;
				led_ctr.change_speed = 0;
			}
		break;
		
		case 4://持续闪烁，由暗开始
			if(led_ctr.light >= led_ctr.max_light || led_ctr.light <= 0)
				led_ctr.change_speed *= -1;
		break;
		
		case 5://暗亮暗
			if(led_ctr.light >= led_ctr.max_light)
				led_ctr.change_speed *= -1;
			else if(led_ctr.light <= 0)
			{
				led_ctr.light = 0;
				led_ctr.change_speed = 0;
			}
		break;
			
		case 6://亮暗亮
			if(led_ctr.light <= 0)
				led_ctr.change_speed *= -1;
			else if(led_ctr.light >= led_ctr.max_light)
			{
				led_ctr.light = led_ctr.max_light;
				led_ctr.change_speed = 0;
			}
		break;
		
		case 7://持续闪烁，由1/8开始
			if(led_ctr.light >= led_ctr.max_light || led_ctr.light <= (led_ctr.max_light / 8) )
				led_ctr.change_speed *= -1;
		break;
		
		case 8://持续闪烁，由1/4开始
			if(led_ctr.light >= led_ctr.max_light || led_ctr.light <= (led_ctr.max_light / 4) )
				led_ctr.change_speed *= -1;
		break;			
	}
	
	if(led_ctr.light > 512)//亮度值过高
	{
		LIGHT_SET_PWM(g_led_light[512]);
	} else if(led_ctr.light<0)//亮度值过低
	{
		LIGHT_SET_PWM(g_led_light[0]);
	} else {
		LIGHT_SET_PWM(g_led_light[led_ctr.light]);
	}
}

uint8_t get_door_state(void)
{
	if(READ_HALL_STATUE())
	{
		return 0x00;
	}
	else 
	{
		return 0x80;
	}
}

/*==================================================================================
* 函 数 名： set_debug_led_state
* 参    数： None
* 功能描述:  设置调试灯状态
* 返 回 值： None
* 备    注： 
* 作    者： xiaozh
* 创建时间： 2019-11-04 141233
==================================================================================*/
void debug_led_control(uint8_t state)
{
	if(state != 0)
	{
	 MAIN_LED_GPIO_Port->BRR = (uint32_t)MAIN_LED_Pin;
	}
	else
	{
	 MAIN_LED_GPIO_Port->BSRR = (uint32_t)MAIN_LED_Pin;
	}
}
static void send_door_status_msg(void)
{
	uint8_t send_buff[8] = {0};
	
	printf("door status change");
	send_buff[0] = get_door_state();	//门状态
//	send_buff[0] = BOX_SUCCESS;
	can_upload_event_to_android(BOX_Android_UP_DOOR_STATUS, send_buff, 1);
} 
/*==================================================================================
* 函 数 名： box_loop_door_status_task
* 参    数： None
* 功能描述:  box定时查询门状态
* 返 回 值： None
* 备    注： 
* 作    者： lc
* 创建时间： 2021-02-07 141233
==================================================================================*/
void loop_door_status_task(void* argv)
{
	//uint8_t send_buff;

	if(doorStateBak !=READ_HALL_STATUE())
	{
		doorStateBak = READ_HALL_STATUE();
		send_door_status_msg();
	}
}
/*==================================================================================
* 函 数 名： box_report_check_task
* 参    数： None
* 功能描述:  box主动上报自检状态信息
* 返 回 值： None
* 备    注： 
* 作    者： lc
* 创建时间： 2021-02-07 141233
==================================================================================*/
void box_report_check_status(void)
{
	uint8_t send_buff[8] = {0};

	//门状态
	send_buff[1] = READ_HALL_STATUE();	
	if(send_buff[1])
		checkStatus |= (1<<DoorSta_bit); 
	else	
		checkStatus &= ~(1<<DoorSta_bit); 	
	//RFID状态
	if(get_rfid_status())
		checkStatus |= (get_rfid_status()<<Rfid_bit); 
	else
		checkStatus &= ~(get_rfid_status()<<Rfid_bit); 
	
	//can_send_one_pkg_to_Android_by_link(BOX_Android_UP_DOOR_STATUS, 0, &checkStatus, 1);
	can_upload_event_to_android(BOX_Android_UP_CHECK_STATUS, &checkStatus, 1);
}
/*==================================================================================
* 函 数 名： get_box_checkStatus
* 参    数： None
* 功能描述:  查询自检状态信息
* 返 回 值： None
* 备    注： 
* 作    者： lc
* 创建时间： 2021-02-07 141233
===================================================================================*/
uint8_t get_box_checkStatus(void)
{
	return checkStatus;
}


void ex_device_test()
{
	HAL_Delay(5000);
	door_open();
	led_control(51, 255);
	//BEEP_ON();
	HAL_Delay(10000);
	door_close();
	led_control(35, 0);
	//BEEP_OFF();
}


