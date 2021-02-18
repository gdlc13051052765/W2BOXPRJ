#ifndef  __ADC_DRV_H
#define  __ADC_DRV_H

#include <stdint.h>


#define MAX_ACD_NUM				32
#define MAX_CONV_NUM			24	//���ɼ�����,����Ϊ2��������



void adc_scan_loop_task(void* argv);

uint16_t get_adc_ev(void);
uint16_t get_adc_temp(void);

#endif
