/*
 * adc.h
 *
 * Created: 16.09.2022 10:40:26
 *  Author: thomahl
 */ 


#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>


typedef struct pos_t {
	int8_t x, y;
} pos_t;


void adc_init(void);

uint8_t adc_read(uint8_t channel);

void adc_calibrate(void);

pos_t pos_read(void);

#endif /* ADC_H_ */
