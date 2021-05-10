/*	Author: acard079
 *  Partner(s) Name: 
 *	Lab Section:21
 *	Assignment: Lab #8  Exercise #1
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

enum states {start, init, ctone, dtone, etone} state;

void TickFct()
{
	unsigned char button = 0x00;
	button = ~PINA & 0x07;
	switch(state){
		case start:
			state = init;
			break;
		case init:
			if(button == 0x01){
				state = ctone;
			}else if(button == 0x02){
				state = dtone;
			}else if(button == 0x04){
				state = etone;
			}else{
				state = init;
			}
			break;
		case ctone:
			if(button == 0x01){
				state = ctone;
			}else{
				state = init;
			}
			break;
		case dtone:
			if(button == 0x02){
				state = dtone;
			}else{
				state = init;
			}
			break;
		case etone:
			if(button == 0x04){
				state = etone;
			}else{
				state = init;
			}
			break;
		default:
			state = start;
			break;
	}
	switch(state){
		case start:
			break;
		case init:
			set_PWM(0); //set_PWM(double frequency)
			//make frequency 0
			break;
		case ctone:
			set_PWM(261.63);
			break;
		case dtone:
			set_PWM(293.66);
			break;
		case etone:
			set_PWM(329.62);
			break; 
		default:
			set_PWM(0);
			break;

	}

}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	TimerSet(100); //100ms
	TimerOn();
	PWM_on();

    while (1) {
	    TickFct();
	    while (!TimerFlag);
	    TimerFlag = 0;

    }
    return 1;
}
