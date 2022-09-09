/*
 * sram.c
 *
 * Created: 09.09.2022 15:10:31
 *  Author: thomahl
 */ 

#include "avr/io.h"

void xmem_init(void) {
	// "External SRAM operation is enabled by setting the SRE bit in the MCUCR register"
	MCUCR |= (1 << SRE);
	// Release P7-P4 from external memory (we want to use it for JTAG)
	SFIOR |= (1 << XMM2);
}

void sram_write(uint8_t data, uint16_t addr) {
	volatile char *ext_mem = (char*) SRAM_BASE_ADDRESS;
	ext_mem[addr] = data;
}

uint8_t sram_init(uint16_t addr) {
	volatile char *ext_mem = (char*) SRAM_BASE_ADDRESS;
	uint8_t ret_val = ext_mem[addr];
	return ret_val;
}