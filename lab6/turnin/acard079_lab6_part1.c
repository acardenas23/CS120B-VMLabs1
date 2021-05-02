/*	Author: Ana Cardenas Beltran
 *  Partner(s) Name: 
 *	Lab Section:21
 *	Assignment: Lab #6  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#endif


enum states{start, init, b0, b1, b2} state;
void Tick()
{
	switch(state){
		case start:
			state = init;
			break;
		case init:
			state = b1;
			break;
		case b0:
			state = b1;
			break;
		case b1:
			state = b2;
			break;
		case b2:
			state = b0;
			break;
		default:
			state = start;
	}
	switch(state){
		case start:
			PORTB = 0x01;
			break;
		case init:
			PORTB = 0x01;
			break;
		case b0:
			PORTB = 0x01;
			break;
		case b1:
			PORTB = 0x02;
			break;
		case b2:
			PORTB = 0x04;
			break;
		default:
			PORTB = 0x01;
	}
}
int main(void) {
    /* Insert DDR and PORT initializations */
	DDRB = 0xFF;
	PORTB = 0x00;
	TimerSet(1000);
	TimerOn();
    /* Insert your solution below */

    while (1) {

	    Tick();
	   
	    while(!TimerFlag);
	    TimerFlag = 0;
	

    }
    return 1;
}
