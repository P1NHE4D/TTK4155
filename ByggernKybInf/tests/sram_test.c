/*
 * sram.c
 *
 * Created: 09.09.2022 15:15:35
 *  Author: thomahl
 */

#include "../drivers/xmem.h"
#include <stdio.h>
#include <stdlib.h>

// This is Lab Support Data > Misc. Resources > sram_test.c modified to use our sram driver.
void SRAM_test(void)
{
	// Initialize xmem inside here, to make absolutely sure we don't forget to do this when testing SRAM...
	xmem_init();
	
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
	printf("SRAM test completed with \n%4d errors in write phase and \n%4d errors in retrieval phase\n\n\r", write_errors, retrieval_errors);
}