#include <avr/interrupt.h>

volatile unsigned char TimerFlag = 0; //TimerISR() sets to 1.C programmer should set to 0

//Internal variables for mapping VR's ISR to our cleaner TimerISR model
unsigned long _avr_timer_M = 1; //start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0;

void TimerOn() {
	//AVR timer/counter controller register TCCR1
	TCCR1B = 0x0B; //bit3: CTC mode (clear timer on compare)
			//00001011: 0x0B
			//will count at 125,000 ticks
	//AVR output compare register 0CR1A
	OCR1A = 125; //Timer interrupt will be generated when TCNT1 == 0CR1A
			//1ms tick. 0.001s*125000 = 125
	//AVR timer interrupt mask register
	TIMSK1 = 0x02;

	//Initialize AVR counter
	TCNT1 = 0;
	_avr_timer_cntcurr = _avr_timer_M;
	//TimerISR will be called every _avr_timer_cntcurr ms
	
	SREG |= 0x80;
}
void TimerISR(){
		TimerFlag = 1;
	}

	ISR(TIMER1_COMPA_vect){
		//CPU automatically calls when TCNT1 == OCR1 (every 1 ms per TimerOn settings)
		_avr_timer_cntcurr--; //Count down to 0 rather than up to TOP
		if (_avr_timer_cntcurr == 0){
			TimerISR();
			_avr_timer_cntcurr = _avr_timer_M;
		}
	}

	//Set TimerISR() to tick every M ms
void TimerSet(unsigned long M){
		_avr_timer_M = M;
		_avr_timer_cntcurr = _avr_timer_M;
	}


