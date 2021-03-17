#ifndef  __OLED_DRV_H
#define  __OLED_DRV_H
#include "spi_class.h"
#include "oled_interface.h"



/**
	*宏：屏幕状态寄存器
	*/
//----------SSD1315(IC) start--------------------	
#define SET_LOWER_COLUMN_ADDRES				0x00
#define MIN_LOWER_COLUMN_ADDRES				0x00

#define SET_HIGHER_COLUMN_ADDRESS			0x10
#define MIN_HIGHER_COLUMN_ADDRESS			0x00

#define SET_DISPLAY_START_LINE				0x40
#define POR_DISPLAY_START_LINE				0x00

#define SET_MEMORY_ADDRESS_MODE			  0x20
#define SET_HORIZONTAL_ADDRESS_MODE		0x00
#define SET_VERTICAL_ADDRESS_MODE		  0x01
#define SET_PAGE_ADDRESS_MODE			    0x02
#define SET_PORTARIT_HORIZONTAL_ADDRESS_MODE			0x04

#define SET_HORIZONTAL_RANG			      0x21
#define SET_START_COLUMN	            0x00
#define SET_END_COLUMN  	            0x0F

#define SET_VERTICAL_ADDRESS			    0x22
#define SET_START_PAGE	              0x00
#define SET_END_PAGE  	              0x3F

#define SET_CONTRAST									0x81
#define HIGH_CONTRAST									0xFF
#define LOW_CONTRAST									0x5F
#define MID_CONTRAST									0xB0

#define SET_CHARGE_PUMP								0x8D
#define ENBALE_CHARGE_PUMP						0x14

#define LEFT_ROTATION									0xA1
#define RIGHT_ROTATION								0xA0

#define ENTIRE_DISPLAY_OFF						0xA5
#define ENTIRE_DISPLAY_ON							0xA4

#define NORMAL_DISPLAY								0xA6
#define REVERSE_DISPLAY								0xA7

#define SET_MULTIPLEX_RATION					0xA8
#define MULTIPLEX_RATION				    	0x3F

#define SELECT_IREF							      0xAD
#define ENABLE_INTERNAL					      0x10
#define ENABLE_EXTERNAL					      0x00

#define DISPLAY_OFF										0xAE
#define DISPLAY_ON										0xAF

#define SET_ROW_ADDRESS								0xB0
#define MIN_ROW_ADDRESS								0x00

#define REVERSE_SCAN_DIRECTION				0xC8
#define FORWARD_SCAN_DIRECTION				0xC0

#define SET_DISPLAY_OFFSET						0xD3
#define REC_DISPLAY_OFFSET						0x00

#define SET_FREQUENCY									0xD5
#define POR_FREQUENCY									0x50
#define REC_FREQUENCY									0x90  //RECOMMEND

#define SET_CHARGE_PERIOD							0xD9
#define REC_CHARGE_PERIOD							0x22

#define SET_HARDWARE_CONFIG				    0xDA
#define REC_HARDWARE_CONFIG				    0x12


#define SET_VCOM_DESELECT_LEVEL				0xDB
#define POR_VCOM_DESELECT_LEVEL				0x35
#define REC_VCOM_DESELECT_LEVEL	      0x30


#define SET_VSEGM_LEVEL								0xDC
#define POR_VSEGM_LEVEL								0x35
#define PROPOSED_VSEGM_LEVEL					0x30

#define SET_POR_DISCHARGE_LEVEL				0x33


#define MAX_SCREEN_NUM			2			//最大屏幕数量

#define X_MAX_PIXEL					128		//最大像素点个数
#define Y_MAX_PIXEL					64		//最大像素点个数

#define DISP_WIDTH        16
#define DISP_HEIGHT       64

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
	uint8_t frame_buffer[DISP_HEIGHT][DISP_WIDTH];
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
void screen_aversion(uint8_t screen,uint8_t row,uint8_t col);
void oled_write_byte(uint8_t screen,uint8_t value,uint8_t cmd_type);
#endif
