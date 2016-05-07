/*	[emarc003]_lab9_part3.c - $date$
 *	Name & E-mail:  - Eric Marcelo, emarc003@ucr.edu
 *	CS Login: emarc003
 *	Partner(s) Name & E-mail:  - Joel Gomez, jgome043@ucr.edu
 *	Lab Section: 021
 *	Assignment: Lab 9  Exercise 3 
 *	Exercise Description: Buttons are connected to PA0 and PA1. 
 *  Output PORTB drives a bank of 4 LEDs. Pressing PA0
 *  increments a binary number displayed on the bank of LEDs 
 *  (stopping at 9). Pressing PA1 decrements the binary number 
 *  (stopping at 0). If both buttons are depressed (even if not initially simultaneously), 
 *  the display resets to 0. If a button is held, then the display continues to
 *  increment (or decrement) at a rate of once per second. However, if the button is held for 3
 *  seconds, the incrementing/decrementing occurs once per 400 ms. Use synchronous state
 *  machines captured in C.
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

////////////////////////////////////////// INCREMENT / DECREMENT //////////////
enum increm{start, wait, decr, out, incr, out1, reset} count;
	unsigned char time; // GLOBAL VARIABLE
void tick_inc(){
	static unsigned char tempB = 0;
	static unsigned char i;
	switch(count){// STATES
		case start:
		    count = wait;
			break;
		case wait:
		    if((~PINA & 0x03) == 0x01){//count up
				tempB = (tempB < 0x09) ? (tempB + 1) : tempB;
				count = incr;
			}else if((~PINA & 0x03) == 0x02){//count down
				tempB = (tempB >= 0x01) ? (tempB - 1) : tempB;
				count = decr;
			}else if ((~PINA & 0x03) == 0x00){//wait
				count = wait;
			}else if((~PINA & 0x03) == 0x03){
				count = reset;
			}
			break;
		case decr:// WAIT / STAY / OUT / RESET
		    if((~PINA & 0x03) == 0x00){
				i = 0;
				count = wait;
			}else if (((~PINA & 0x03) == 0x02) && (i < time)){
				count = decr;
			}else if(((~PINA & 0x03) == 0x02) && (i >= time)){
				count = out;
			}else if((~PINA & 0x03) == 0x03){
				count = reset;
			}
			break;
		case out:// reset counter
		    i = 0;
		    count = decr;
		    break;
		case incr:// WAIT / STAY / OUT1 / RESET
		    if((~PINA & 0x03) == 0x00){
				count = wait;
			}else if(((~PINA & 0x03) == 0x01) && (i < time)){
				count = incr;
			}else if(((~PINA & 0x03) == 0x01) && (i >= time)){
				count = out1;
			}else if((~PINA & 0x03) == 0x03){
				count = reset;
			}
			break;
		case out1:// reset counter
		    i = 0;
		    count = incr;
			break;
		case reset:
		    i = 0;
		    count = wait;
			break;
		default:
		   break;
	}// STATES
	switch(count){// ACTIONS
		case start:
		     break;
		case wait: 
		     i = 0;
		     PORTB = tempB;
			 break;
		case decr:// DISPLAY DECREMENT
		    i++;
			PORTB = tempB;
			 break;
		case out:// DECREASE IF GREATER THAN 0
		    if(tempB > 0x00){
			    tempB = tempB -1;
			    }else{
			    tempB = tempB;
		    }
			break;
		case incr:// DISPLAY INCREMENT
		    i++;
		    PORTB = tempB;
		    break;
		case out1:// INCREMENT IF LESS THAN 9
		     if(tempB < 0x09){
			     tempB = tempB + 1;
			     }else{
			     tempB = tempB;
		     }
			 break;
	    case reset:// RESET THE SYSTEM
		    tempB = 0x00;
		    break;
		default:// DO NOTHING
		    break;
	}// ACTIONS
	
}
////////////////////////////// CHANGE CADANCE ///////////////
enum states {init, sec, A0, A1, fast} t ;
void tick_time(){
	static unsigned short i, j;
	switch(t){
	    case init:
		    t = sec;
		    break;
		case sec:// COUNT LENGTH OF BUTTON PRESS
		    if((~PINA & 0x03) == 0x01){
			    t = A0;
			}else if((~PINA & 0x03) == 0x02){
			    t = A1;
			}else if((~PINA & 0x03) == 0x00){
			    t = sec;
		}
		    break;
		case A0:// STAY / CADANCE UP / RESET
		    if(((PINA & 0x03) == 0x01) && (i < 3)){
				t = A0;
			}else if (((~PINA & 0x03) == 0x01) && (i >= 300)){
				t = fast;
			}else{
				t = sec;
			}
		    break;
	    case A1:// STAY / CADANCE DOWN / RESET
		     if(((~PINA & 0x03) == 0x02) && (j < 3)){
			     t = A1;
			 }else if (((~PINA & 0x03) == 0x02) && (j >= 300)){
			     t = fast;
			 }else{
			     t = sec;
		     }
		     break;
		case fast:// CADANCE ADJUSTMENT
		    if(((~PINA & 0x03) == 0x01) || ((~PINA & 0x03) == 0x02)){
				t = fast;
			}else{
				t = sec;
			}
		    break;
		default:// default to initial state
		   t = init;
		   break;
		   
	}
	switch(t){
		case init:
		    break;
		case sec:// SET CADANCE
		    i = 0;
			j = 0;
		    time = 100;
		    break;
		case A0:// INCREASE I
		    i++;
			j = 0;
		    break;
		case A1:// INCREASE J
		    j++;
			i = 0;
		    break;
		case fast:// CADANCE UP
		    time = 40;
		    break;
		default:
		    break;
		
	}
}

//////////////////////////// MAIN THREAD ////////////
int main() {
	DDRA = 0x00; PORTA = 0xFF;//set PORTA to input
	DDRB = 0xFF; PORTB = 0x00;//set PORTC to output
	t = init;
	count = start;
	TimerSet(10);//10 ms
	TimerOn();

	while(1) {
		tick_time();// CALL TIMER FIRST / CALL INCREMENTS
		tick_inc();		
		while(!TimerFlag){}
		TimerFlag = 0;
	} // while (1)
} // Main