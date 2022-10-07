/*
 * can.h
 *
 * Created: 07.10.2022 11:40:58
 *  Author: thomahl
 */ 


#ifndef CAN_H_
#define CAN_H_

typedef struct {
	uint8_t id[5];
	uint8_t data[8];
} CAN_message_t;

void CAN_init();

void CAN_send(CAN_message_t msg);

CAN_message_t CAN_receive();

#endif /* CAN_H_ */