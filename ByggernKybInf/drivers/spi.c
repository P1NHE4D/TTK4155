/*
 * spi.c
 *
 * Created: 07.10.2022 09:41:03
 *  Author: thomahl
 */ 

#include <avr/io.h>
#include "spi.h"

void SPI_init() {
	/* Set MOSI and SCK output, all others input */
	DDRB |= (1<<PB5)|(1<<PB7)|(1<<PB4);
	
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR |= (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}

void SPI_transmit(uint8_t data) {
	SPI_transmit_receive(data);
}

uint8_t SPI_receive() {
	return SPI_transmit_receive(0);
}

uint8_t SPI_transmit_receive(uint8_t data) {
	/* Start transmission */
	SPDR = data;
		
	/* Wait for transmission complete */
	while (!(SPSR & (1<<SPIF))) {}

	/* Return data register */
	return SPDR;
}