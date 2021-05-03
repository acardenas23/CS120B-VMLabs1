/*	Author: Ana Cardenas Beltran
 *  Partner(s) Name: 
 *	Lab Section:21
 *	Assignment: Lab #6  Exercise #2
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

enum states{start, init, b0, b1, b2, b3, b0hold, b1hold, b2hold, b3hold, reset} state;
void Tick()
{
	//unsigned char prev = 0x00;
	unsigned char tmpA0 = 0x00;
	switch(state){
		//prev = 0x00; //will use mealy to know which state it was previously in before being reset
		tmpA0 = ~PINA & 0x01;
		case start:
			state = init;
			break;
		case init:
			state = b0;
			break;
		case b0:
			if(tmpA0){
				state = b0hold;
			}else{
				//prev = 0x01;
				state = b1;
			}
			break;
		case b0hold:
			if(tmpA0){
				state = b0hold; //stay in b0hold while button is pressed
			}else if(!tmpA0){
				state = reset; //go to reset once button is depressed
			}
			break;
		case b1:
			if(tmpA0){
				state = b1hold; //go to hold state if button is pressed
			}else{
				state = b2; //if button is not pressed and prev = 1, got to b2
			}
			break;
		case b1hold:
			if(tmpA0){
				state = b1hold;
			}else if(!tmpA0){
				state = reset;
			}
			break;
		case b2:
			//prev = 0x03;
			if(tmpA0){
				state = b2hold;
			}else{
				state = b3; //0x02
			}
			break;
		case b2hold:
			if(tmpA0){
				state = b2hold;
			}else if(!tmpA0){
				state = reset;
			}
			break;
		case b3:
			if(tmpA0){
				state = b3hold;
			}else{
				state = b0;
			}
			break;
		case b3hold:
			if(tmpA0){
				state = b3hold;
			}else{
				state = b0;
			}
			break;
		case reset:
			state = init;
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
		case b3:
			PORTB = 0x02;
			break;
		case b0hold:
			break;
		case b1hold:
			break;
		case b2hold:
			break;
		case b3hold:
			break;
		case reset:
			PORTB = 0x01;
			break;
		default:
			PORTB = 0x01;
	}
}
int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF; //PORTA is input
	DDRB = 0xFF; PORTB = 0x00; //PORTB is output
	TimerSet(100); //100*3 = 300
	TimerOn();
	unsigned char i = 0x00;
	unsigned char tmpA0 = 0x00;
    /* Insert your solution below */

    while (1) {
	   tmpA0 = ~PINA & 0x01;

	    if(tmpA0){
		    state = reset;
	    }else if(i%3 == 0){
		    Tick();
	    }
	    
	    while(!TimerFlag);
	    ++i;
	    TimerFlag = 0;

    }
    return 1;
}
