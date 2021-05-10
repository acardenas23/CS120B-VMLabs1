/*	Author: acard079
 *  Partner(s) Name: 
 *	Lab Section:21
 *	Assignment: Lab #8  Exercise #2
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

enum states {START, WAIT, UPORDOWN, HOLD, INC, DEC, SOUNDOFF, SOUNDON} state;
double tone[8] = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25};
unsigned char i = 0x00;

//modifying code from lab6
void TickFct()
{
	unsigned char button = 0x00;
	button = ~PINA & 0x07;
	switch(state){
		case START:
			state = WAIT;
			break;
		case WAIT:
			if(button == 0x04){
				state = SOUNDON;
			}else{
				state = WAIT;
			}
			break;
		case UPORDOWN:
			if(button == 0x01){
				state = INC;
			}else if(button == 0x02){
				state = DEC;
			}else if(button == 0x04){
				state = SOUNDOFF;
			}else{
				state = UPORDOWN;
			}
			break;
		case HOLD:
			if((button != 0x01) && (button != 0x02)){
				state = UPORDOWN; //button has been unpressed
			}else{
				state = HOLD; //one of the buttons is still being pressed
			}
			break;
		case INC:
			state = HOLD;
			break;
		case DEC:
			state = HOLD;
			break;
		case SOUNDOFF:
			if(button == 0x04){
				//button is still being pressed
				state = SOUNDOFF;
			}else{
				state = WAIT;//go to wait for button to be pressed again
			}
			break;
		case SOUNDON:
			if(button == 0x04){
				//button is still being pressed
				state = SOUNDON;
			}else{
				state = UPORDOWN;
			}
			break;
		default:
			state = START;
			break;
	}
	switch(state){
		case START:
			i = 0x00;
			set_PWM(0);
			//use i to iterate through array
			break;
		case WAIT:
			set_PWM(0); //off in wait state
			break;
		case UPORDOWN:
			break;
		case HOLD:
			break;
		case INC:
			if(i < 0x07){
				i = i + 0x01;
			}
			set_PWM(tone[i]);
			break;
		case DEC:
			if(i > 0x00){
				i = i - 0x01;
			}
			set_PWM(tone[i]);
			break;
		case SOUNDOFF:
			set_PWM(0);
			i = 0x00;
			break;
		case SOUNDON:
			set_PWM(tone[i]);
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
	TimerSet(100);
	TimerOn();	
	PWM_on();

    while (1) {
	    TickFct();
	    while(!TimerFlag);
	    TimerFlag = 0;

    }
    return 1;
}
