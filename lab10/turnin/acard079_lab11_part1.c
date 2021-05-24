/*	Author: acard079
 *  Partner(s) Name: 
 *	Lab Section:21
 *	Assignment: Lab #10  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *	https://drive.google.com/drive/folders/1RYPZ16sYkxQSHkCKZ8bhibP1qWL_GU8s?usp=sharing
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "keypad.h"
#include "scheduler.h"
#include "timer.h"
#endif

unsigned char x;//keypad input
unsigned char tmpB = 0x00;
enum KP_states {kp_start, kp_init};
int KP_Tick(int state)
{
	switch(state){
		case kp_start:
			state = kp_init;
			break;
		case kp_init:
			x = GetKeypadKey();
			switch(x) {
				case '\0': tmpB = 0x1F; break;
				case '1': tmpB = 0x01; break; //hex equivalent
		        	case '2': tmpB = 0x02; break; 
				case '3': tmpB = 0x03; break;
				case '4': tmpB = 0x04; break;
				case '5': tmpB = 0x05; break;
				case '6': tmpB = 0x06; break;
				case '7': tmpB = 0x07; break;
				case '8': tmpB = 0x08; break;
				case '9': tmpB = 0x09; break;
				case 'A': tmpB = 0x0A; break;
				case 'B': tmpB = 0x0B; break;
				case 'C': tmpB = 0x0C; break; 
				case 'D': tmpB = 0x0D; break;
				case '*': tmpB = 0x0E; break;
				case '0': tmpB = 0x00; break;
				case '#': tmpB = 0x0F; break;	
				default: tmpB = 0x1B; break;
			}
			state = kp_init;
			break;
		default:
			state = kp_start;
			break;
	}
	switch(state){
		case kp_start:
			break;
		case kp_init:
			PORTB = tmpB;
			break;
		default:
			break;
	}
	return state;

}



int main(void) {
	unsigned char x;
	DDRB = 0xFF; PORTB = 0x00; //portb set to output, output inits 0s
	DDRC = 0xF0; PORTC = 0x0F;

	//Declare array of tasks
	static task task1;
	task *tasks[] = { &task1 };
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	const char start = 0;
	//Task1 KP_Tick()
	task1.state = start; //initial state
	task1.period = 50;
	task1.elapsedTime = task1.period;
	task1.TickFct = &KP_Tick;

	TimerSet(50);
	TimerOn();

	unsigned short i;

	while(1) {
		for( i = 0; i < numTasks; i++) {
			if(tasks[i]->elapsedTime == tasks[i]->period ) {
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 50;
		}
		while(!TimerFlag);
		TimerFlag = 0;
    }
    return 1;
}
