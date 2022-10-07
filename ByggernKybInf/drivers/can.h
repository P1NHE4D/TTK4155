/*
 * can.h
 *
 * Created: 07.10.2022 11:40:58
 *  Author: thomahl
 */ 


#ifndef CAN_H_
#define CAN_H_

#include <stdbool.h>

typedef struct {
	uint16_t id;
	bool rtr;
	uint8_t dlc;
	uint8_t data[8];
} CAN_standard_message_t;

void CAN_init();

void CAN_send(CAN_standard_message_t msg);

CAN_standard_message_t CAN_receive();

#endif /* CAN_H_ */