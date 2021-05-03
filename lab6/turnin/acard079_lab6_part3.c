/*	Author: Ana Cardenas Beltran
 *  Partner(s) Name: 
 *	Lab Section:21
 *	Assignment: Lab #6  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#endif

enum states {START, WAIT, WAIT2, HOLDINC, HOLDDEC, INC, DEC, RESET} state;


void TickFct()
{
	unsigned char button = 0x00;
	button = ~PINA & 0x03;
	switch(state){
		case START:
			state = WAIT;
			break;
		case WAIT:
			state = WAIT2;
			break;
		case WAIT2:
			if(button == 0x01){
				state = INC;
			}else if(button == 0x02){
				state = DEC;
			}else if(button == 0x03){
				state = RESET;
			}else if(button == 0x00){
				state =  WAIT2;
			}
			break;
		case HOLDINC:
			if(button == 0x01){
				state = INC;
			}else if(button == 0x03){
				state = RESET;
			}else if(button == 0x02){
				state = DEC;
			}else{
				state = WAIT2;
			}
			break;
		case HOLDDEC:
			if(button == 0x02){
				state = DEC;
			}else if(button == 0x03){
				state = RESET;
			}else if(button == 0x01){
				state = INC;
			}else{
				state = WAIT2;
			}
			break;

		case INC:
			if(button == 0x02){
				state = DEC;
			}else if(button == 0x03){
				state = RESET;
			}else if(button == 0x01){
				state = HOLDINC;
			}else{
				state = WAIT2;
			}
			
			break;
		case DEC:
			if(button == 0x01){
				state = INC;
			}else if(button == 0x03){
				state = RESET;
			}else if(button == 0x02){
				state = HOLDDEC;
			}else{
				state = WAIT2;
			}
			break;
		case RESET:
			if(button == 0x03){
				state = RESET;
			}else{
				state = WAIT2;
			}
			break;
		default:
			state = START;
			break;
	}
	switch(state){
		case START:
			PORTB = 0x07;
			break;
		case WAIT:
			PORTB = 0x07;
			break;
		case WAIT2:
			break;
		case HOLDINC:
			break;
		case HOLDDEC:
			break;

		case INC:
			if(PINB < 0x09){
				PORTB = PINB + 0x01;
			}else{
				PORTB = 0x09;
			}
			break;
		case DEC:
			if(PINB > 0x00){
				PORTB = PINB - 0x01;
			}else{
				PORTB = 0x00;
			}
			break;
		case RESET:
			PORTB = 0x00;
			break;
		default:
			PORTB = 0x07;
			break;
	}
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF; // PORTA is input
	DDRB = 0xFF; PORTB = 0x00; //PORTB is output
	TimerSet(100);
	TimerOn();
	state = START;
    /* Insert your solution below */
    while (1) {
	    TickFct();

	    while(!TimerFlag);
	    TimerFlag = 0;
    }
    return 1;
}

