/*
 * ByggernKybInf.c
 *
 * Created: 02.09.2022 14:26:58
 * Author : thomahl
 */ 

#include <avr/io.h>
#include <avr/sleep.h>


int main(void)
{
    /* Replace with your application code */
    while(1) 
	{
		// Set PC0 to 1
		PORTC |= 1;
		
		// Set PC0 to 0
		PORTC &= ~1;
	}
}

