/*	Author: acard079
 *  Partner(s) Name: 
 *	Lab Section:21
 *	Assignment: Lab #9  Exercise #2
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

unsigned char ThreeLEDs; //shared variable
unsigned char BlinkingLED;
unsigned char tmpB;

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
			tmpB = BlinkingLED | ThreeLEDs; //combine two states
			PORTB = tmpB;
			break;

	}
}

int main(void) {
	DDRB = 0xFF; PORTB = 0x00;

	TimerSet(100); //100s period
	TimerOn();

	unsigned char i = 0x00;
	unsigned char j = 0x00;

	while (1) {

		if(i%3 == 0){
			ThreeLEDsSM();
		}
		if(j%10 == 0){
			BlinkingLEDSM();
		}
		CombineLEDsSM();

		while(!TimerFlag);
		++i;
		++j;
		TimerFlag = 0;
	}
    return 1;
}
