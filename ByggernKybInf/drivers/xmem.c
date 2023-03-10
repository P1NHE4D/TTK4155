/*
 * sram.c
 *
 * Created: 09.09.2022 15:10:31
 *  Author: thomahl
 */ 

#include <stdio.h>
#include <stdlib.h>
#include "xmem.h"
#include "avr/io.h"

const uint16_t SRAM_BASE_ADDRESS = 0x1800;

const uint16_t SRAM_SIZE = 0x800;

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

uint8_t sram_read(uint16_t addr) {
	volatile char *ext_mem = (char*) SRAM_BASE_ADDRESS;
	uint8_t ret_val = ext_mem[addr];
	return ret_val;
}

void sram_test()
{
	uint16_t write_errors = 0;
	uint16_t retrieval_errors = 0;
	printf("Starting SRAM test...\n\r");
	// rand() stores some internal state, so calling this function in a loop will
	// yield different seeds each time (unless srand() is called before this function)
	uint16_t seed = rand();
	// Write phase: Immediately check that the correct value was stored
	srand(seed);
	for (uint16_t i = 0; i < SRAM_SIZE; i++) {
		uint8_t some_value = rand();
		sram_write(some_value, i);
		uint8_t retreived_value = sram_read(i);
		if (retreived_value != some_value) {
			printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\n\r", i, retreived_value, some_value);
			write_errors++;
		}
	}
	// Retrieval phase: Check that no values were changed during or after the write phase
	srand(seed);
	// reset the PRNG to the state it had before the write phase
	for (uint16_t i = 0; i < SRAM_SIZE; i++) {
		uint8_t some_value = rand();
		uint8_t retreived_value = sram_read(i);
		if (retreived_value != some_value) {
			printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\n\r", i, retreived_value, some_value);
			retrieval_errors++;
		}
	}
	printf("SRAM test completed with \n\r%4d errors in write phase and \n\r%4d errors in retrieval phase\n\n\r", write_errors, retrieval_errors);
}