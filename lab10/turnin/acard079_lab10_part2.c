/*s	Author: acard079
 *  Partner(s) Name: 
 *	Lab Section:21
 *	Assignment: Lab #10  Exercise #2
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

unsigned char x = 0x00;//keypad input
unsigned char prev = 0;
unsigned char keypadlock = 0x00;
enum UL_states {ul_start, ul_init, s1, s2, s3, s4, s5, unlocked, ul_release};
int UnlockSM(int state) {
	unsigned char press = ~PINB & 0x80;
	x = GetKeypadKey();
	switch(state){
		case ul_start:
			state = ul_init; break;
		case ul_init:
			prev = 0;
			keypadlock = 0x00;
			state = x == '#'? ul_release:ul_init;
		       	break;
		case s1:
			prev = 1;
			keypadlock = 0x00;
			if(x == '\0'){
				state = s1;
			}else if(x == '1'){
				state = ul_release;
			}else{
				state = ul_init;
			} 
			break;
		case s2:
			prev = 2;
			keypadlock = 0x00;
			if(x == '\0'){
				state = s2;
			}else if(x == '2'){
				state = ul_release;
			}else{
				state = ul_init;
			}
		       	break;
		case s3:
			prev = 3;
			keypadlock = 0x00;
			if(x == '\0'){
				state = s3;
			}else if(x == '3'){
				state = ul_release;
			}else{
				state = ul_init;
			}
			break;
		case s4:
			prev = 4;
			keypadlock = 0x00;
			if(x == '\0'){
				state = s4;
			}else if(x == '4'){
				state = ul_release;
			}else{
				state = ul_init;
			}
		       	break;
		case s5:
			prev = 5;
			keypadlock = 0x00;
			if(x == '\0'){
				state = s5;
			}else if(x == '5'){
				state = ul_release;
			}else{
				state = ul_init;
			}
		       	break;
		case ul_release:
			if((prev == 0) && (x == '\0')){
				state = s1;
			}else if((prev ==1) && (x == '\0')){
				state = s2;
			}else if((prev ==2) && (x == '\0')){
				state = s3;
			}else if((prev ==3) && (x == '\0')){
				state = s4;
			}else if((prev ==4) && (x == '\0')){
				state = s5;
			}else if((prev == 5) && (x == '\0')){
				state = unlocked;
			}else{
				state = ul_release;
			}
			break;
		case unlocked:
			keypadlock = 0x01;
			state = press == 0x80? ul_init: unlocked;
		       	break;
		default: state = ul_start; break;
	}
	switch(state){
		case ul_start:break;
		case ul_init:
			PORTB = 0x00;
			break;
		case s1:
			PORTB = 0x00;
			break;
		case s2:
			PORTB = 0x00;
			break;
		case s3:
			PORTB = 0x00;
			break;
		case s4:
			PORTB = 0x00;
			break;
		case s5:
			PORTB = 0x00;
			break;
		case ul_release:
			PORTB = 0x00;
			break;
		case unlocked:
			PORTB = 0x01;
			break;
		default: break;
	}
	return state;
}

/*
unsigned char inside = 0x00;
enum LK_states{lk_start, lk_init, lk_locked, lk_locked2};
int LockSM(int state) {
	unsigned char press =  ~PINB & 0x80; //PB7
	switch(state){
		case lk_start:
			inside = 0x00;
			state = lk_init;
			break;
		case lk_init:
			inside = 0x01;
			if(press == 0x80){
				state =lk_locked;
			}else{
				state = lk_init;
			}
			break;
		case lk_locked:
			inside = 0x00;
			if(press == 0x00){
				//button released
				state = lk_locked2;
			}else{
				state = lk_locked;
			}
			break;
		case lk_locked2:
			inside = 0x00; //remain locked
			if(keypadlock == 0x01){
				//if unlocked
				state = lk_init;
			}else{
				//stay locked
				state = lk_locked2;
			}
			break;
		default:
			state = lk_start;
			break;
	}
	return state;
}

*/


int main(void) {
	//DDRA = 0x0F; PORTA = 0xF0; //lower 4 are input, upper4 are output
	DDRB = 0x0F; PORTB = 0xF0; //lower 4 are input, upper 4 are output
	DDRC = 0xF0; PORTC = 0x0F; //input

	//Declare array of tasks
	static task task1;
	task *tasks[] = { &task1 };
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	const char start = 0;
	//Task1 KP_Tick()
	task1.state = start; //initial state
	task1.period = 50;
	task1.elapsedTime = task1.period;
	task1.TickFct = &UnlockSM;
	/*//Task 2 UnlockSM()
	task2.state = start;
	task2.period = 50;
	task2.elapsedTime = task2.period;
	task2.TickFct = &UnlockSM;*/
	//Task 3 LockSM()
	/*task3.state = start;
	task3.period = 50;
	task3.elapsedTime = task3.period;
	task3.TickFct = &LockSM;*/

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
