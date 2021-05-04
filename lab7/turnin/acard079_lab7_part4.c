/*	Author: Ana Cardenas
 *  Partner(s) Name: 
 *	Lab Section:21
 *	Assignment: Lab #7  Exercise #4
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
	    max = 0xCB;  //max is 1101 1100 = 203/8 = 25.37

	    if(x <= 0x19){
		    PORTB = 0x01;
	    }else if(x <= 0x32){
		    PORTB = 0x03;
	    }else if(x <= 0x4B){
		    PORTB = 0x07;
	    }else if(x <= 0x64){
		    PORTB = 0x0F;
	    }else if(x <= 0x7D){
		    PORTB = 0x1F;
	    }else if(x <= 0x96){
		    PORTB = 0x3F;
	    }else if(x <= 0xAF){
		    PORTB = 0x7F;
	    }else if(x >= 0xAF){
		    PORTB = 0xFF;
	    }


    }
    return 1;
}
