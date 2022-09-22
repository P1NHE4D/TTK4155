/*
 * adc.h
 *
 * Created: 16.09.2022 10:40:26
 *  Author: thomahl
 */ 


#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>

enum channel_t {JOYSTICK_X, JOYSTICK_Y, LEFT_SLIDER, RIGHT_SLIDER};

void adc_init(void);
uint8_t adc_read(channel_t channel);
void adc_calibrate(void);


#endif /* ADC_H_ */