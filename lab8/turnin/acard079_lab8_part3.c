/*	Author: acard079
 *  Partner(s) Name: 
 *	Lab Section:21
 *	Assignment: Lab #8  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "PWMHeader.h"
#include "timer.h"
#endif

enum states {START, init, hold, tone1, tone2, tone3, tone4, tone5, end} state;
double melody[5] = {329.63, 349.23, 392.00, 349.23, 392.00};
void TickFct()
{
	unsigned char button = 0x00;
	button = ~PINA & 0x01;
	switch(state){
		case START:
			state = init;
			break;
		case init:
			if(button == 0x01){
				state = tone1;
			}else{
				state = init;
			}
			break;
/*		case hold:
			if(button == 0x00){
				state = tone1; ///button released
			}else{
				state = hold;
			}
			break;*/
		case tone1:
			state = tone2;
			break;
		case tone2:
			state = tone3;
			break;
		case tone3:
			state = tone4;
			break;
		case tone4:
			state = tone5;
			break;
		case tone5:
			state = end;
			break;
		case end:
			if(button == 0x01){
				state = end; //if button was pressed
			}else if(button == 0x00){
				state = init;
			}
			break;
		default:
			state = START;
			break;
	}
	switch(state){
		case START:
			set_PWM(0);
			break;
		case init:
			set_PWM(0);
			break;
	/*	case hold:
			break;*/
		case tone1:
			set_PWM(melody[0]);
			break;
		case tone2:
			set_PWM(melody[1]);
			break;
		case tone3:
			set_PWM(melody[2]);
			break;
		case tone4:
			set_PWM(melody[3]);
			break;
		case tone5:
			set_PWM(melody[4]);
			break;
		case end:
			set_PWM(0);
			break;
		default:
			state = START;
			break;
	}
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	TimerSet(500);
	TimerOn();	
	PWM_on();

    while (1) {
	    TickFct();
	    while(!TimerFlag);
	    TimerFlag = 0;

    }
    return 1;
}
