#include "led_beep.h"

ret_msg_t beep_control(uint8_t state)
{
	ret_msg_t ret = BOX_UNKNOW_PARAM;

	switch (state)
	{
		case 1:
			BEEP_ON();
			ret = BOX_SUCCESS;
		break;
		case 0:
			BEEP_OFF();
			ret = BOX_SUCCESS;
		break;
		default:
			ret = BOX_UNKNOW_PARAM;
		break;
	}

	return ret;
}
