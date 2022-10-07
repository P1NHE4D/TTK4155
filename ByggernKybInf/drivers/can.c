/*
 * can.c
 *
 * Created: 07.10.2022 11:49:19
 *  Author: thomahl
 */ 

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "can.h"
#include "mcp2515.h"
#include "uart.h"


void CAN_init() {
	mcp2515_init();
}

void CAN_send(CAN_message_t msg) {
	// check if transmission is ongoing		
	while (true) {
		uint8_t control_register = mcp2515_read(MCP_TXB0CTRL, 1)[0];
		if (!(control_register & (1 << 3))) {
			break;
		}
	}
		
	// setup interrupt
	mcp2515_bit_modify(
		MCP_CANINTE,
		(1<<2),
		0
	);
	
	// construct CAN message buffer	
	uint8_t tx_buffer[14];
	for (int i = 0; i < 5; i++) {
		tx_buffer[i+1] = msg.id[i];
	}
	for (int i = 0; i < 8; i++) {
		tx_buffer[i+6] = msg.data[i];
	}
	
	// send CAN message
	mcp2515_write(MCP_TXB0CTRL, tx_buffer, 14);

	// request-to-send transmit buffer 0
	mcp2515_rts_tx0();
	
	// check if transmission is done
	while (true) {
		uint8_t control_register = mcp2515_read(MCP_TXB0CTRL, 1)[0];
		if (!(control_register & (1 << 3))) {
			break;
		}
	}
}

CAN_message_t CAN_receive() {
	// dumb implementation for now: just return whatever is in the rx0 buffer
		
	CAN_message_t received_msg;
	
	uint8_t* ptr_rx_buffer = mcp2515_read(MCP_RXB0CTRL, 14);
		
	for (int i = 0; i < 5; i++) {
		received_msg.id[i] = ptr_rx_buffer[i+1];
	}
	
	for (int i = 0; i < 8; i++) {
		received_msg.data[i] = ptr_rx_buffer[i+6];
	}
	
	free(ptr_rx_buffer);
	
	return received_msg;
}