/*	Author: Ana Cardenas
 *  Partner(s) Name: 
 *	Lab Section:21
 *	Assignment: Lab #7  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned short x = 0x000; 
unsigned short max = 0x00;
//Value of ADC register is now stored in variable x

void ADC_init(){
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}
int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;

    /* Insert your solution below */
	ADC_init();
    while (1) {
	    x = ADC;
	    max = 0xCB;  //max is 1101 1100
	    unsigned char lower8 = (char)x;
	    unsigned char upper2 = (char)(x>>8);

	    if(x >= (max/2)){
		    PORTB = 0x01;
	    }else{
		    PORTB = 0x00;
	    }

    }
    return 1;
}
