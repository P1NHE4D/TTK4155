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
	
	// TA har 3 og 41 (:
	// delte opp i 16
	// proparg 1
	
	// set CAN bit timings (while we are in configuration mode)

	// cnf1
	uint8_t sjw = 0;     // re-synchronization jump width
	uint8_t brp = 1;     // (TA: 3) baudrate prescaler            (1/500000)/((2*(x+1))/16000000) = 8 OR (2*(x+1)/16000000 = (1/500000)/8
	
	// cnf2
	uint8_t sam = 0;     // sampling mode (just has to be the same as on node 1) (assuming this is the same as "smp")
	uint8_t btlmode = 1; // "if this bit is set to 1 the length of PS2 is determined by the PHSEG2 of CNF3"
	uint8_t phseg1 = 3;  //  (TA: 6)
	uint8_t prseg = 0;   // (TA: 1) programming time segment      (1.66) (assume same as "propag")

	// cnf3
	uint8_t sof = 0;     // "If CANCTRL.CLKEN = 0, Bit is don't care"
	uint8_t wakfil = 0;  // TODO what is this?
	uint8_t phseg2 = 3;  // (TA: 5) TODO what is this?

	uint8_t cnf1 = 0
		| ((sjw & 0b11)     << 6)
		| ((brp & 0b111111) << 0)
		;
	
	uint8_t cnf2 = 0
		| ((btlmode & 0b1)     << 7)
		| ((sam & 0b1)         << 6)
		| ((phseg1 & 0b111)    << 3)
		| ((prseg & 0b111)     << 0)
		;
	
	uint8_t cnf3 = 0
		| ((sof & 0b1)         << 7)
		| ((wakfil & 0b1)      << 6)
		| ((phseg2 & 0b111)    << 0)
		;
	
	// set cnf1, cnf2, cnf3 in mcp2515
	// TODO is this OK? no. Go away.
	
	
	mcp2515_write(MCP_CNF1, &cnf1, 1);
	mcp2515_write(MCP_CNF2, &cnf2, 1);
	mcp2515_write(MCP_CNF3, &cnf3, 1);
	
	/*
	uint8_t cnf1_ta = 0b0000011;
	mcp2515_write(MCP_CNF1, &cnf1_ta, 1);
	uint8_t cnf2_ta = 0b10110001;
	mcp2515_write(MCP_CNF2, &cnf2_ta, 1);
	uint8_t cnf3_ta = 0b00000101;
	mcp2515_write(MCP_CNF3, &cnf3_ta, 1);
	*/
	
	uint8_t* value_cnf1 = mcp2515_read(MCP_CNF1, 1);
	uint8_t* value_cnf2 = mcp2515_read(MCP_CNF2, 1);
	uint8_t* value_cnf3 = mcp2515_read(MCP_CNF3, 1);
	
	printf("What was written to CNF1 CNF2 CNF3: %d %d %d\n\r", *value_cnf1, *value_cnf2, *value_cnf3);
	
			
	// Select loopback
	
	mcp2515_bit_modify(
		MCP_CANCTRL,
		7<<5,        // 3 MSB, REQOP<0:2>
		0<<6         // REQOP1
	);
			
	value = mcp2515_read(MCP_CANSTAT, 1)[0];
	if ((value & MODE_MASK) != MODE_LOOPBACK) {
		printf("MCP2515 is NOT in loopback mode after setting it during init!\n\r");
	}
	if ((value & MODE_MASK) != MODE_NORMAL) {
		printf("MCP2515 is NOT in normal mode after setting it during init!\n\r");
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