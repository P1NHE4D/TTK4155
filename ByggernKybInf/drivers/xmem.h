/*
 * sram.h
 *
 * Created: 09.09.2022 15:10:21
 *  Author: thomahl
 */ 

#ifndef XMEM_H_
#define XMEM_H_


// Start address for the SRAM
extern const uint16_t SRAM_BASE_ADDRESS;

// Taken from Lab Support Data > Misc. Resources > sram_test.c
extern const uint16_t SRAM_SIZE;

// initialize xmem. All other functions in this file require xmem_init to be ran first
void xmem_init(void);

// write the byte 'data' to the 12-bit address 'addr'
void sram_write(uint8_t data, uint16_t addr);

// read a byte from sram at the 12-bit address 'addr'
uint8_t sram_read(uint16_t addr);

// Tests the SRAM driver using a modified version of the code provided at
// Lab Support Data > Misc. Resources > sram_test.c
void sram_test();


#endif /* XMEM_H_ */