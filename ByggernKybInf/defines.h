/*
 * defines.h
 *
 * Created: 20.09.2022 13:27:43
 *  Author: thomahl
 */ 


#ifndef DEFINES_H_
#define DEFINES_H_

#define FOSC 4915200
#define BAUD 9600
#define COMPUTED_UBRR FOSC/16/BAUD-1

#define ADC_BASE_ADDRESS 0x1400
#define ADC_NUM_CHANNELS 4
#define ADC_CLK FOSC/2
#define ADC_T_CONV (9 * ADC_NUM_CHANNELS * 2) / ADC_CLK

#endif /* DEFINES_H_ */