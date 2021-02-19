#ifndef  __EX_DEVICE_H
#define  __EX_DEVICE_H

#include "main.h"
  
 
#define   LIGHT_ENABLE_OUT()				do { \
																					 TIM3->CCER &= ~(TIM_CCER_CC1E << TIM_CHANNEL_1);\
																					 TIM3->CCER |=  (uint32_t)(TIM_CCx_ENABLE << TIM_CHANNEL_1);\
																					} while(0)

																					
#define   LIGHT_DISABLE_OUT()				do { \
																					 TIM3->CCER &= ~(TIM_CCER_CC1E << TIM_CHANNEL_1);\
																					TIM3->CCER |=  (uint32_t)(TIM_CCx_DISABLE << TIM_CHANNEL_1);\
																					} while(0)

#define  LIGHT_SET_PWM(value)					(TIM3->CCR1 = value)
																					
			

#define  MOTOR_ENABLE_OUT()						do{GPIOB->BSRR = GPIO_PIN_3;}while(0)
#define  MOTOR_DISABLE_OUT()					do{GPIOB->BSRR = (uint32_t)GPIO_PIN_3 << 16u;}while(0)
																					
#define   SERVO_ENABLE_OUT()				do { \
																					 TIM2->CCER &= ~(TIM_CCER_CC1E << TIM_CHANNEL_3);\
																					 TIM2->CCER |=  (uint32_t)(TIM_CCx_ENABLE << TIM_CHANNEL_3);\
																					} while(0)

																					
#define   SERVO_DISABLE_OUT()				do { \
																					 TIM2->CCER &= ~(TIM_CCER_CC1E << TIM_CHANNEL_3);\
																					TIM2->CCER |=  (uint32_t)(TIM_CCx_DISABLE << TIM_CHANNEL_3);\
																					} while(0)

#define  SERVO_SET_PWM(value)				(TIM2->CCR3 = value)
 
#define READ_HALL_STATUE()					HAL_GPIO_ReadPin(HALL_GPIO_Port, HALL_Pin)		//1:CLOSE  0:OPEN

																					
typedef enum
{
	Bright_to_Dark=0,		//��������
	Dark_to_Bright,		//������
	
}_Light_Type;

//�Լ�״̬����λ
typedef enum
{
	Olen_bit = 0,
	Heart_bit,
	Led_bit,
	Rfid_bit,
	Door_bit,
	DoorSta_bit,
}_Check_State;

typedef struct ctl_led_msg {
	uint8_t sta;//״̬
	int16_t max_light;//�������
	int16_t light;//ʵʱ����
	int8_t change_speed;//�仯�ٶ�
}ctl_led_msg_t;

uint16_t get_temperature(void); //��ȡ�¶�ֵ
uint8_t get_door_state(void);	//��ȡ��״̬
void door_open(void);	
void door_close(void);
ret_msg_t door_control(uint8_t state);
ret_msg_t led_control(uint8_t state,uint8_t bright);
ret_msg_t temp_control(uint8_t state,uint8_t temp);
void led_change(void);//����LED���ȵ�������
uint8_t get_box_checkStatus(void);
void box_report_check_status(void);
																					
#endif
