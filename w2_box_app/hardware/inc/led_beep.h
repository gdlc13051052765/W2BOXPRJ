#ifndef  __LED_BEEP_H
#define  __LED_BEEP_H
#include "main.h"

ret_msg_t beep_control(uint8_t state);

																					
#define BEEP_ON()				  do{ HAL_GPIO_WritePin(GPIOA, BEEP_Pin, GPIO_PIN_SET);}while(0)
#define BEEP_OFF()				do{ HAL_GPIO_WritePin(GPIOA, BEEP_Pin, GPIO_PIN_RESET);}while(0)


#endif
