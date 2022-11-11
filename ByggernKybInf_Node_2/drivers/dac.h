
/*
 * dac.h
 *
 * Created: 11.11.2022 09:53:03
 *  Author: thomahl
 */ 

#ifndef DAC_H_
#define DAC_H_


#include <stdint.h>

void dac_init();

// NOTE: our DAC is 12-bit
void dac_convert(uint16_t value);

#endif /* DAC_H_ */