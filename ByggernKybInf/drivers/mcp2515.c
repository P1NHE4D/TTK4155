/*
 * mcp2515.c
 *
 * Created: 07.10.2022 10:51:17
 *  Author: thomahl
 */ 

#include <stdint.h>
#include <avr/io.h>
#include <stdlib.h>
#include "mcp2515.h"
#include "spi.h"
#include "uart.h"


void mcp2515_select() {
	PORTB &= ~(1<<PINB4); // Select CAN-controller
}

void mcp2515_deselect() {
	PORTB |= (1<<PINB4); // Deselect CAN-controller
}

void mcp2515_init() {
	uint8_t value;
	
	SPI_init(); // Initialize SPI
	mcp2515_reset(); // Send reset-command
	
	// Self-test
	value = mcp2515_read(MCP_CANSTAT, 1)[0];
	if ((value & MODE_MASK) != MODE_CONFIG) {
		printf("MCP2515 is NOT in configuration mode after reset!\n");
	}
	stdout = &UART_STREAM;
	
	// Select loopback
	
	mcp2515_bit_modify(
		MCP_CANCTRL,
		7<<5,        // 3 MSB, REQOP<0:2>
		1<<6         // REQOP1
	);
			
	value = mcp2515_read(MCP_CANSTAT, 1)[0];
	if ((value & MODE_MASK) != MODE_LOOPBACK) {
		printf("MCP2515 is NOT in loopback mode after setting it during init!\n");
	}
}

void mcp2515_reset() {
	mcp2515_select();
	SPI_transmit(MCP_RESET);
	mcp2515_deselect();
}

uint8_t* mcp2515_read(uint8_t address, uint8_t length) {
	mcp2515_select();
	
	SPI_transmit(MCP_READ); // Send read instruction
	SPI_transmit(address);  // Send address
	
	uint8_t* bytes_read = malloc(sizeof(uint8_t) * length);
	for (int i = 0; i < length; i++) {
		// Read ith byte
		bytes_read[i] = SPI_receive();
	}
	
	mcp2515_deselect();
	
	return bytes_read;
}

void mcp2515_write(uint8_t address, uint8_t* data, uint8_t data_length) {	
	mcp2515_select();
	SPI_transmit(MCP_WRITE); // Send read instruction
	SPI_transmit(address);   // Send address
	for (int i = 0; i < data_length; i++) {
		// Transmit ith byte
		SPI_transmit(data[i]);
	}
	mcp2515_deselect();
}

void mcp2515_rts_tx0() {
	mcp2515_select();
	SPI_transmit(MCP_RTS_TX0);
	mcp2515_deselect();
}

void mcp2515_bit_modify(uint8_t address, uint8_t mask, uint8_t data) {
	mcp2515_select();
		
	SPI_transmit(MCP_BITMOD);
		
	SPI_transmit(address);
	SPI_transmit(mask);
	SPI_transmit(data);
		
	mcp2515_deselect();
}

uint8_t mcp2515_rx_status() {
	mcp2515_select();
	SPI_transmit(MCP_RX_STATUS);
	uint8_t value;
	value = SPI_receive();
	mcp2515_deselect();
	return value;
}