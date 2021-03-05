#ifndef  __OLED_DRV_H
#define  __OLED_DRV_H
#include "spi_class.h"
#include "oled_interface.h"

#define MAX_SCREEN_NUM			2			//最大屏幕数量

#define X_MAX_PIXEL					128		//最大像素点个数
#define Y_MAX_PIXEL					64		//最大像素点个数

#define DISP_WIDTH        128
#define DISP_HEIGHT        8

typedef  enum
{
	FONT_12 = 12,		//12*12  12号字
	FONT_16 = 16,		//16*16  16号字
	FONT_24 = 24,		//24*24	 24号字
	FONT_32 = 32,		//32*32	 32号字
}_Font_Size;

typedef enum
{
	SCREEN_LEFT = 0,		//左屏
	SCREEN_RIGHT,				//右屏
}_Screen_Type;

typedef enum
{
	BLACK = 0,		//黑
	WHITE,				//白
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
	_Screen_Type screen;				//屏幕
	_Font_Color  font_color;		//字体颜色
	_Font_Size 	 font_size;			//字体大小 （16 24 32）
	uint8_t      font_scale; 		//字体转换比例系数，默认为1，不进行缩放
	uint8_t      *p_text;				//要显示的字符串编码
}_Font_Info, *_pFont_Info;

#define  _FONT_INIT()  {0, 0, SCREEN_LEFT, WHITE, FONT_16, 1, NULL}

typedef  unsigned long  (*gt_get_func)(uint8_t , uint8_t, uint8_t, uint8_t, uint8_t*);	//获取字库芯片数据


#define OLED_CMD  				0	//写命令
#define OLED_DATA 				1	//写数据
 
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
