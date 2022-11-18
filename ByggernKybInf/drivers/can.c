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


void CAN_send(CAN_standard_message_t msg) {
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
	
	// TXBnSIDH bits 7-0 contain SID bits 10-3
	tx_buffer[1] = msg.id >> 3;
	// TXBnSIDL bits...
	// * 7-5 contain SID bits 2-0
	// * 3 contains EXIDE
	// * 1-0 contain EID17-16
	tx_buffer[2] = msg.id << 5;
	// ...middle bytes of ID bits are for extended, which we don't care about :) ...
	// TXBnDLC bits...
	// * 6 bit contains RTR
	// * 3-0 contain DLC 3-0
	tx_buffer[5] = (msg.rtr<<6)|(0b1111 & msg.dlc);
	// TXBnDm contain the data...
	for (int i = 0; i < msg.dlc; i++) {
			tx_buffer[6+i] = msg.data[i];
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

CAN_standard_message_t CAN_receive() {
	// dumb implementation for now: just return whatever is in the rx0 buffer
		
	CAN_standard_message_t received_msg;
	
	uint8_t status = mcp2515_rx_status();
	if (!(status & (0b11 << 6))) {
		received_msg.id = 0;
		return received_msg;
	}
	uint8_t* ptr_rx_buffer = mcp2515_read(MCP_RXB0CTRL, 14);
	
		
	// SID 10 to 3 are in TXBnSIDH bits 7-0, and SID 2-0 are in TXBnSIDL 7-5
	received_msg.id = (ptr_rx_buffer[1] << 3)|(ptr_rx_buffer[2] >> 5);
	// TXBnDLC bits...
	// * 6 bit contains RTR
	// * 3-0 contain DLC 3-0
	received_msg.rtr = (ptr_rx_buffer[5] & (1<<6) >> 6);
	received_msg.dlc = (ptr_rx_buffer[5] & 0b1111);
	// TXBnDm contain the data...
	for (int i = 0; i < received_msg.dlc; i++) {
		received_msg.data[i] = ptr_rx_buffer[6+i];
	}
	
	free(ptr_rx_buffer);

	// clear RX0IF and RX1IF (which is what we check at the top by reading rx status)	
	mcp2515_bit_modify(
		MCP_CANINTF,
		0b11,
		0
	);
	
	return received_msg;
}