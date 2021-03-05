#ifndef  __OLED_DRV_H
#define  __OLED_DRV_H
#include "spi_class.h"
#include "oled_interface.h"

#define MAX_SCREEN_NUM			2			//�����Ļ����

#define X_MAX_PIXEL					128		//������ص����
#define Y_MAX_PIXEL					64		//������ص����

#define DISP_WIDTH        128
#define DISP_HEIGHT        8

typedef  enum
{
	FONT_12 = 12,		//12*12  12����
	FONT_16 = 16,		//16*16  16����
	FONT_24 = 24,		//24*24	 24����
	FONT_32 = 32,		//32*32	 32����
}_Font_Size;

typedef enum
{
	SCREEN_LEFT = 0,		//����
	SCREEN_RIGHT,				//����
}_Screen_Type;

typedef enum
{
	BLACK = 0,		//��
	WHITE,				//��
}_Font_Color;

typedef struct
{
	uint8_t type;
	uint8_t mode;
	uint8_t frame_buffer[DISP_WIDTH][DISP_HEIGHT];
}_Screen_Info,*_pScreen_Info;

typedef struct
{
	uint8_t x;
	uint8_t y;
	_Screen_Type screen;				//��Ļ
	_Font_Color  font_color;		//������ɫ
	_Font_Size 	 font_size;			//�����С ��16 24 32��
	uint8_t      font_scale; 		//����ת������ϵ����Ĭ��Ϊ1������������
	uint8_t      *p_text;				//Ҫ��ʾ���ַ�������
}_Font_Info, *_pFont_Info;

#define  _FONT_INIT()  {0, 0, SCREEN_LEFT, WHITE, FONT_16, 1, NULL}

typedef  unsigned long  (*gt_get_func)(uint8_t , uint8_t, uint8_t, uint8_t, uint8_t*);	//��ȡ�ֿ�оƬ����


#define OLED_CMD  				0	//д����
#define OLED_DATA 				1	//д����
 
#define OLED_DC_LOW()	 			do{HAL_GPIO_WritePin(GPIOB, OLED_DC_Pin, GPIO_PIN_RESET);}while(0)
#define OLED_DC_HIGH()  		do{HAL_GPIO_WritePin(GPIOB, OLED_DC_Pin, GPIO_PIN_SET);}while(0)

#define ALL_RES_HIGH()			do{GPIOB->BSRR = OLED_RES1_Pin; GPIOB->BSRR = OELD_RES2_Pin;}while(0)
#define ALL_RES_LOW()				do{GPIOB->BSRR = (uint32_t)OLED_RES1_Pin << 16u; GPIOB->BSRR = (uint32_t)OELD_RES2_Pin << 16u;}while(0)
	
void oled_gt_init(void);
void show_read_tag(uint8_t read_num, uint8_t real_num);
void oleddrv_disp(_Disp_Param pmsg );
void screen_show_bmp(uint8_t screen, uint8_t x_s, uint8_t y_s, uint8_t x_e, uint8_t y_e, uint8_t *c_buff, uint8_t color);
void show_upgrade_tag(uint16_t read_num);
void show_lost_num_tag(uint16_t read_num);
void oled_show_string(uint8_t num,uint8_t *data);
#endif
