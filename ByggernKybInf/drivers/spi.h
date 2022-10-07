/*
 * spi.h
 *
 * Created: 07.10.2022 09:30:57
 *  Author: thomahl
 */ 


#ifndef SPI_H_
#define SPI_H_

#include<stdint.h>

void SPI_init();

void SPI_transmit(uint8_t data);

uint8_t SPI_receive();

uint8_t SPI_transmit_receive(uint8_t data);


#endif /* SPI_H_ */