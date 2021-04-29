/*	Author: Ana Cardenas Beltran
 *  Partner(s) Name: 
 *	Lab Section:21
 *	Assignment: Lab #5  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *		PA3..PA0 set to value from 0 to 15
 *		if level is 1 or 2-> PC5 lights.....
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA =  0xFF; //set PORTA as input
	DDRC = 0xFF; PORTC = 0x00; //set PORTC as output

	unsigned char tmpA = 0x00;
	unsigned char tmpC = 0x00;

	/* Insert your solution below */
    while (1) {
	    tmpA = (~PINA) & 0x0F;
	    tmpC = 0x00;
	  	    
		//13-15 lights PC5-PC0 
	    if(tmpA >= 0x0D){
		    tmpC = 0x3F;
	    }else if (tmpA >= 0x0A){
		    tmpC = 0x3E;
	    }else if (tmpA >= 0x07){
		    tmpC = 0x3C;
	    }else if (tmpA >= 0x05){
		    tmpC = 0x38;
	    }else if (tmpA >= 0x03){
		    tmpC = 0x70; //0111 0000
	    }else if (tmpA >= 0x01){
		    tmpC = 0x60; // 0110 0000
	    }else{
		    tmpC = 0x40;
	    }

	    PORTC = tmpC;
	   	    

    }
    return 1;
}

