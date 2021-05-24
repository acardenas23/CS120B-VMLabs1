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

unsigned char x;//keypad input
unsigned char tmpB = 0x00;
unsigned char keypad = 0x00; //global var that will hold value of keypad
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
                                    state = kp_start;
                        break;
        }
        switch(state){
                case kp_start:
                        break;
                case kp_init:
			keypad = tmpB; //keypad will hold what the keypad value is
                        break;
                default:
                        break;
        }
        return state;

}

unsigned char keypadlock = 0x00;
enum UL_states {ul_start, ul_init, s1, s2, s3, s4, s5, unlocked};
int UnlockSM(int state) {
	unsigned char press = ~PINB & 0x80;
	switch(state){
		case ul_start:
			state = ul_init;
			break;
		case ul_init:
			keypadlock = 0x00;
			if(keypad == 0x0F){
				//if # if pressed
				state = s1;
			}else{
				state = ul_init;
			}
			break;
		case s1:
			keypadlock = 0x00;
			if(keypad == 0x01){
				state = s2;
			}else{
				state = ul_init;
			}
			break;
		case s2:
			keypadlock = 0x00;
			if(keypad == 0x02){
				state = s3;
			}else{
				state = ul_init;
			}
			break;
		case s3:
			keypadlock = 0x00;
			if(keypad == 0x03){
				state = s4;
			}else{
				state = ul_init;
			}
			break;
		case s4:
			keypadlock = 0x00;
			if(keypad == 0x04){
				state = s5;
			}else{
				state = ul_init;
			}
			break;
		case s5:
			keypadlock = 0x00;
			if(keypad == 0x05){
				state = unlocked;
			}else{
				state = ul_init;
			}
			break;
		case unlocked:
			keypadlock = 0x01;
			if(press == 0x80){
				//lock
				state = ul_init;
			}else{
				state = unlocked;
			}
			break;
	}
	PORTB = keypadlock;
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
	DDRC = 0x00; PORTC = 0xFF; //input

	//Declare array of tasks
	static task task1, task2;
	task *tasks[] = { &task1, &task2 };
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	const char start = 0;
	//Task1 KP_Tick()
	task1.state = start; //initial state
	task1.period = 50;
	task1.elapsedTime = task1.period;
	task1.TickFct = &KP_Tick;
	//Task 2 UnlockSM()
	task2.state = start;
	task2.period = 50;
	task2.elapsedTime = task2.period;
	task2.TickFct = &UnlockSM;
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
