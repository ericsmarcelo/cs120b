/*	[emarc003]_lab7_part1.c - $5/1/2016$
 *	Name & E-mail:  - Eric Marcelo, emarc003@ucr.edu
 *	CS Login: emarc003
 *	Partner(s) Name & E-mail:  - Joel Gomez, jgome043@ucr.edu
 *	Lab Section: 021
 *	Assignment: Lab 7  Exercise 1
 *	Exercise Description: Using the ATmega1284’s PWM functionality, 
 *  design a system that uses three buttons to select one of
 *  three tones to be generated on the speaker. When a button is pressed, 
 *  the tone mapped to it is generated on the speaker.
 *  Criteria:
 *   Use the tones C4, D4, and E4 from the table in the introduction section.
 *   When a button is pressed and held, the tone mapped to it is generated on the speaker.
 *   When more than one button is pressed simultaneously, the speaker remains silent.
 *   When no buttons are pressed, the speaker remains silent.
 *	
 *	
 *	I acknowledge all content contained herein, excluding template or example 
 *	code, is my own original work.
 */ 


#include <avr/io.h>
	
	enum states{start, wait, note} state;

// 0.954 hz is lowest frequency possible with this function,
// based on settings in PWM_on()
// Passing in 0 as the frequency will stop the speaker from generating sound
void set_PWM(double frequency) {
	static double current_frequency; // Keeps track of the currently set frequency
	// Will only update the registers when the frequency changes, otherwise allows
	// music to play uninterrupted.
	if (frequency != current_frequency) {
		if (!frequency) { TCCR3B &= 0x08; } //stops timer/counter
		else { TCCR3B |= 0x03; } // resumes/continues timer/counter
		// prevents OCR3A from overflowing, using prescaler 64
		// 0.954 is smallest frequency that will not result in overflow
		if (frequency < 0.954) { OCR3A = 0xFFFF; }
		// prevents OCR3A from underflowing, using prescaler 64
		// 31250 is largest frequency that will not result in underflow
		else if (frequency > 31250) { OCR3A = 0x0000; }
		// set OCR3A based on desired frequency
		else { OCR3A = (short)(8000000 / (128 * frequency)) - 1; }
		TCNT3 = 0; // resets counter
		current_frequency = frequency; // Updates the current frequency
	}
}
void PWM_on() {
	TCCR3A = (1 << COM3A0);
	// COM3A0: Toggle PB6 on compare match between counter and OCR3A
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
	// WGM32: When counter (TCNT3) matches OCR3A, reset counter
	// CS31 & CS30: Set a prescaler of 64
	set_PWM(0);
}
void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}
//// SM /////////////////////////////////
void tickNotes(){
	static double Hz;
	switch(state){//states
		case start:
		   state = wait;
		   break;
		case wait:
		   if(!(~PINA & 0x04) && !(~PINA & 0x02) && (~PINA & 0x01)){// 001 / C4
			   Hz = 261.63;
			   state = note;
		   }else if(!(~PINA & 0x04) && (~PINA & 0x02) && !(~PINA & 0x01)){// 010 / D4
			   Hz = 293.66;
			   state = note;
		   }else if ((~PINA & 0x04) && !(~PINA & 0x02) && !(~PINA & 0x01)){//100 / E4
			   Hz = 329.63;
			   state = note;
		   }else if(((~PINA & 0x07) == 0x03) || ((~PINA & 0x07) == 0x05) || ((~PINA & 0x07) == 0x06)){
		      state = wait;
		   }
		   break;
		case note:
		    if(((~PINA & 0x07) == 0x03) || ((~PINA & 0x07) == 0x05) || ((~PINA & 0x07) == 0x06) || ((~PINA & 0x07) == 0x00)){// if more than one button is pressed or no buttons / silent
		    state = wait; 
			}else{
				state = note;
			}
		   break;
		default:
		   break;
	}//states
	switch(state){//actions
		case start:// initialize port and frequency to 0
		    PORTB = 0x00;
			Hz = 0;
			break;
		case wait:
		    set_PWM(0);//set the pulse width to silence
			break;
		case note:
		    set_PWM(Hz);
			break;
		default:
			break;
	}//actions
}
/////////////////////////////////////////////////
	
int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;//PB6 is the speaker output
	state =  start;
	PWM_off();
	PWM_on();
	
	while(1)
	{
		// TODO: Please write your application code 
		tickNotes();
		
	}
}