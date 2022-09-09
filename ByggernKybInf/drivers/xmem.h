/*
 * sram.h
 *
 * Created: 09.09.2022 15:10:21
 *  Author: thomahl
 */ 

#ifndef XMEM_H_
#define XMEM_H_


// Start address for the SRAM
uint16_t SRAM_BASE_ADDRESS = 0x1800;

// Taken from Lab Support Data > Misc. Resources > sram_test.c
uint16_t SRAM_SIZE = 0x800;

void xmem_init(void);

void sram_write(uint8_t data, uint16_t addr);

uint8_t sram_read(uint16_t addr);


#endif /* XMEM_H_ */