/*	[emarc003]_lab9_part1.c - $date$
 *	Name & E-mail:  - Eric Marcelo, emarc003@ucr.edu
 *	CS Login: emarc003
 *	Partner(s) Name & E-mail:  - Joel Gomez, jgome043@ucr.edu
 *	Lab Section: 021
 *	Assignment: Lab 9  Exercise 1 
 *	Exercise Description: Connect LEDs to PB0, PB1, PB2, and PB3. 
 *  In one state machine (Three LEDs), output to a shared variable 
 *  (threeLEDs) the following behavior: set only bit 0 to 1, then only bit 1, then only
 *  bit 2 in sequence for 1 second each. In a second state machine (Blinking LED), 
 *  output to a shared variable (bilnkingLED) the following behavior: 
 *  set bit 3 to 1 for 1 second, then 0 for 1 second. In a third state machine 
 *  (Combine LEDs), combine both shared variables and output to the PORTB. 
 *  Note: only one SM is writing to outputs. Do this for the rest of the quarter.
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
//SM Global variables
unsigned char ThreeLEDs;
unsigned char blinkingLED;

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

enum three{start, wait, on, off} triple;

void tick_Three(){
	static unsigned char i, j;
	switch (triple)// STATES
	{
		case start:
		    triple = wait;
			break;
		case wait: 
		    triple = on;
			break;
		case on:// stay here until count expires / reset variables
		   if(i < 2) {
			   triple = on;
			   }else if (i >= 2){
			   j++;
			   i = 0;
			   triple =  off;
		   }
			break;
		case off:// stay here until count expires / reset variables
		    if(i < 2) {
				triple = off;
			}else if (i >= 2){
				i = 0;
		        triple =  on;
			}
			break;
		default:
		    triple  = start;
		   break;
	}// STATES
	switch (triple)// ACTIONS
	{
		case start:
		    break;
		case wait:
		    i = 0;
			j = 0;
			ThreeLEDs = 0x01;
			break;
		case on://count
		    i++;
			break;
		case off:// set the global value of ThreeLEDs / count
		    if(j < 3){
				ThreeLEDs = (0x01 << j);
			}else if(j >= 3){
				ThreeLEDs = 0x01;
				j = 0;
			}
			i++;
			break;
		default:
		    break;
		
	}// ACTIONS
	
}
///////////////////////////////////////////////////////////////

enum blinker{init, pause, Three, Zero} blink;
void tick_Blink(){
	static unsigned char i;
	switch(blink)// STATES
	{
		case init:
		    blink = pause;
			break;
		case pause:
		    blinkingLED = 0x08;
		    blink = Three;
			break;
		case Three:// wait until the count expires / reset variables
		    if(i < 2){
				blink = Three;
			}else if (i >= 2){
				i = 0;
				blink = Zero;
			}
			break;
		case Zero:// wait until the count expires / reset variables
		   if(i < 2){
			   blink = Zero;
		  }else if (i >= 2){
			   i = 0;
			   blink = Three;
		   }
		   break;
		   default:
		      blink = init;
			  break;
	}// STATES
	switch(blink)// ACTIONS
	{
		case init:
		    break;
		case pause:
		    i = 0;
			blinkingLED = 0x00;
			break;
		case Three:
		    i++;
			blinkingLED = 0x00;
			break;
		case Zero:
		    i++;
			blinkingLED = 0x01;
			break;
	   default:
	        break;
		
	}// ACTIONS
}
////////////////////////////////////////////////////////////////]

enum mix{begin,delay, out1, out2} all;
void tick_Out(){
	static unsigned char i;
	switch(all)// STATES
	{
		case begin:
		    all = delay;
			break;
		case delay:
		    all = out1;
			break;
		case out1:
		    if(i < 2){
				all = out1;
			}else if(i >= 2){
				all = out2;
			}
			break;
		case out2:
		     if(i < 2){
			     all = out2;
			 }else if(i >= 2){
			     all = out1;
		     }
		     break;
	    default:
		    all = begin;
		    break;
		
	}// STATES
	switch(all)// ACTIONS
	{
		case begin:
		    break;
		case delay:
		    break;
		case out1:
		    i++;
			PORTB = ThreeLEDs | blinkingLED;
			break;
		case out2:
		    i++;
			PORTB = ThreeLEDs | blinkingLED;
			break;
		default:
		    break;
	}// STATES
}

int main(void)
{
	DDRB = 0xFF; PORTB = 0x00;//set PORTB to output and initialize to 0
	blink = init;
	triple = start;
	all = begin;
	TimerSet(1000);//1000 ms
	TimerOn();
	
	while(1)
	{
		// TODO: Please write your application code
		tick_Three();
		tick_Blink();
		tick_Out();
		while(!TimerFlag){}
		TimerFlag = 0;
	}
}