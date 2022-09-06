/*
 * ByggernKybInf.c
 *
 * Created: 02.09.2022 14:26:58
 * Author : thomahl
 */ 

#include <avr/io.h>

void UART_init(unsigned int ubrr) {
	// set baud rate
	UBRR0H = (unsigned char) (ubrr>>8);
	UBRR0L = (unsigned char) ubrr;
	
	// enable receiver and transmitter
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);
	
	// set frame format
	UCSR0C = (1<<URSEL0) | (1<<USBS0) | (3<<UCSZ00);
}

void UART_Transmit(unsigned char data) {
	// wait for empty transmit buffer
	while (!(UCSR0A & (1<<UDRE0))) {}
		
	// put data into buffer, sends the data
	UDR0 = data;
}

unsigned char UART_Receive(void) {
	// wait for data to be received
	while (!(UCSR0A & (1<<RXC0))) {}
		
	// get and return received data from buffer
	return UDR0;
}