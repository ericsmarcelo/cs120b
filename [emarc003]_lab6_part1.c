/*	[emarc003]_lab6_part1.c - $4/20/2016$
 *	Name & E-mail:  - Eric Marcelo, emarc003@ucr.edu
 *	CS Login: emarc003
 *	Partner(s) Name & E-mail:  - Joel Gomez, jgome043@ucr.edu
 *	Lab Section: 021
 *	Assignment: Lab 6  Exercise 1
 *	Exercise Description: Design a synchSM that toggles PB4 on for 2ms and off 
 *  for 2ms as long as a button on PA2 is in the on position. Connect your 
 *  speaker's red wire to PB4 and the black wire to ground.
 *	
 *	
 *	I acknowledge all content contained herein, excluding template or example 
 *	code, is my own original work.
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char TimerFlag = 0;//TimerISR() sets this to 1. C programmer should clear to 0
//Internal variables for mapping AVR's ISR to our cleaner TimerISR model
unsigned long _avr_timer_M = 1; //Start count from here, down to 0. Default 1ms
unsigned long _avr_timer_cntcurr = 0;//Current internal count of 1 ms ticks
//SM variables

enum states{start, wait, on, off} state;

int tick(){
	switch (state)//states
	{
		case start:
		    state = wait;
			break;
		case wait://wait for button press
		    state = (~PINA & 0x04) ? on : wait;
			break;
		case on://if button is presses switch to off on tick / reset
		    state = (~PINA & 0x04) ? off : wait;
			break;
		case off://if button is pressed switch to on on tick / reset
		    state = (~PINA & 0x04) ? on : wait;
			break;
		default:// default state
		    state = start;
			break;
	}//states
	switch (state)//actions
	{
		case start://initialize port
		    PORTB =  0x00;
			break;
		case wait://initialize port
		    PORTB =  0x00;
			break;
		case on:// set port to on
		    PORTB =  0x10;
			break;
		case off:// set port to off
		    PORTB =  0x00;
			break;
		default://default action
			break;
	}//actions
}

//////////////////////////////////////////////////////////////



////////// Timer functions ///////////////
void TimerOn(){
	//AVR timer/counter controller register TCCR1
	TCCR1B = 0x0B;  //bit3 = 0: CTC mode (clear timer on compare)
	//bit2//bit1//bit0 = 011: pre-scalar / 64
	//00001011: 0x0B
	//so, 8 MHz clock or 8000000 / 64 = 125, 000 ticks/s
	//Thus, TCNT1 register will count at 125,000 ticks/s
	
	// AVR output compare register OCR1A.
	OCR1A = 125;	// Timer interrupt will be generated when TCNT1==OCR1A
	// We want a 1 ms tick. 0.001 s * 125,000 ticks/s = 125
	// So when TCNT1 register equals 125,
	// 1 ms has passed. Thus, we compare to 125.
	
	// AVR timer interrupt mask register
	TIMSK1 = 0x02; // bit1: OCIE1A -- enables compare match interrupt
	//Initialize avr counter
	TCNT1=0;
	
	_avr_timer_cntcurr = _avr_timer_M;
	//TimerISR will be called every _avr_timer_cntcurr milliseconds
	
	//Enable global interrupts
	SREG |= 0x80;// 0x80: 10000000
}

void TimerOff(){
	TCCR1B = 0x00; //bit3//bit2//bit1//bit0 = 0000: timer off
}

void TimerISR(){
	TimerFlag = 1;
}

//In our approach, theC Programmer does not touch this ISR, but rather TimerISR()
ISR(TIMER1_COMPA_vect){
	// CPU automatically calls when TCNT1 == OCR1 (every 1 ms per TimerOn settings)
	_avr_timer_cntcurr--; // Count down to 0 rather than up to TOP
	if (_avr_timer_cntcurr == 0){// results in a more efficient compare
		TimerISR();// Call the ISR that the user uses
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M){
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

/////////////////////////////////////////////////////////////////////////////
int main(void)
{   
	DDRA = 0x00; PORTA = 0xFF;//set PORTA to input
	DDRB = 0xFF; PORTB = 0x00;//set PORTB to output and initialize to 0
	TimerSet(2);//2 ms
	TimerOn();
	
	while(1)
	{
		// TODO: Please write your application code 
		tick();
		while(!TimerFlag){}
			TimerFlag = 0;
	}
}