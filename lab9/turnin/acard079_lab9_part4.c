/*	Author: acard079
 *  Partner(s) Name: 
 *	Lab Section:21
 *	Assignment: Lab #9  Exercise #4
 *	Exercise Description: [optional - include for your own benefit]
 *	https://drive.google.com/drive/folders/1vk5WpeluW9iFS_83dnD_VrSd1OC4IBfG?usp=sharing
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#endif

static unsigned char ThreeLEDs; //shared variable
static unsigned char BlinkingLED;
static unsigned char Speaker; 
static unsigned char tmpB;

enum states1 {start1, b0, b1, b2} state1;
void ThreeLEDsSM(){
	switch(state1){
		case start1:
			state1 = b0;
			break;
		case b0:
			state1 = b1;
			break;
		case b1:
			state1 = b2;
			break;
		case b2:
			state1 = b0;
			break;
		default:
			state1 = start1;
			break;
	}
	switch(state1){
		case start1:
			break;
		case b0:
			ThreeLEDs = 0x01;
			break;
		case b1:
			ThreeLEDs = 0x02;
			break;
		case b2:
			ThreeLEDs = 0x04;
			break;
		default:
			ThreeLEDs = 0x01;
			break;

	}
}

enum states2 {start2, on, off} state2;
	//set bit3 to 1 for 1 second then 0 for 1 second
void BlinkingLEDSM(){
	switch(state2){
		case start2:
			state2 = on;
			break;
		case on:
			state2 = off;
			break;
		case off:
			state2 = on;
			break;
		default:
			state2 = on;
			break;
	}
	switch(state2){
		case start2:
			break;
		case on:
			BlinkingLED = 0x08;
			break;
		case off:
			BlinkingLED = 0x00;
			break;
		default:
			BlinkingLED = 0x08;
			break;
	}
}


static unsigned char button = 0x00; //global variable for button
enum BT_states { BT_off, BT_on, BT_wait } BT_state;
//passing button into function
void ButtonSM(unsigned char A2){
	switch(BT_state){
		case BT_off:
			if(A2){
				BT_state = BT_wait;
			}else{
				BT_state = BT_off;
			}
			break;
		case BT_on:
			if(A2){
				BT_state = BT_on;
			}else{
				BT_state = BT_wait;
			}
			break;
		case BT_wait:
			if(A2){
				BT_state = BT_on; //if button is being pressed, go to on state
			}else{
				BT_state = BT_off;
			}
			break;
		default:
			BT_state = BT_off;
			break;
}
	switch(BT_state){
		case BT_off:
			button = 0x00;
			break;
		case BT_on:
			button = 0x01;
			break;
		case BT_wait:
			break;
		default:
			button = 0x00;
			break;
	}
}

enum FR_states {FR_init, FR_inc, FR_dec} FR_state;
static unsigned char period = 0;
void FrequencySM(unsigned char A){
	switch(FR_state){
		case FR_init:
			if(A == 0x01){
				FR_state = FR_inc;
			}else if (A == 0x02){
				FR_state = FR_dec;
			}else{
				FR_state = FR_init;
			}
			break;
		case FR_inc:
			if(period > 0){
			period = period - 1;
			}
			FR_state = FR_init;
			break;
		case FR_dec:
			if(period < 10){
			period = period + 1;
			}
			FR_state = FR_init;
			break;
		default:
			FR_state = FR_init;
			break;

	}
}

enum SP_States {SP_on, SP_off} SP_state;
//turn speaker on when A2 is pressed
void SpeakerSM(){
	switch(SP_state){
		case SP_on:
			SP_state = SP_off;
			break;
		case SP_off:
			if(button){
				SP_state = SP_on;
			}else{
				SP_state = SP_off;
			}
			break;
		default:
			SP_state = SP_off;
			break;
	}
	switch(SP_state){
		case SP_on:
			Speaker = 0x10;
			break;
		case SP_off:
			Speaker = 0x00;
			break;
		default:
			Speaker = 0x00;
			break;

	}
}

enum states3 {start3} state3;
void CombineLEDsSM(){
	switch(state3){
		case start3:
			state3 = start3;
			break;
		default:
			state3 = start3;

	}
	switch(state3){
		case start3:
			tmpB = BlinkingLED | ThreeLEDs | Speaker; //combine states
			PORTB = tmpB;
			break;

	}
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;

	unsigned long i = 300;
	unsigned long j = 1000;
	unsigned long k = 1;
	unsigned char buttonA2 = 0x00;
	unsigned char buttonA  = 0x00;

	TimerSet(1);
	TimerOn();

	FR_state = FR_init;
	SP_state = SP_off;
	period = 5;
	
	while (1) {
		
		buttonA2 = (~PINA) & 0x04;
		buttonA = (~PINA) & 0x03;
		ButtonSM(buttonA2);
		if(i%300 == 0){
			ThreeLEDsSM();
			i = 0;

		}
		if(j%1000 == 0){
			BlinkingLEDSM();
			j = 0;
		}
		FrequencySM(buttonA);
		if(k >= period){
			//once k = period, call SpeakerSM
			SpeakerSM();
			k = 0;
		}

		CombineLEDsSM();

		while(!TimerFlag);
		TimerFlag = 0;
		++i;
		++j;
		++k;
	}
    return 1;
}
