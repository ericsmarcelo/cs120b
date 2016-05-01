/*	[emarc003]_lab7_part2.c - $5/1/2016$
 *	Name & E-mail:  - Eric Marcelo, emarc003@ucr.edu
 *	CS Login: emarc003
 *	Partner(s) Name & E-mail:  - Joel Gomez, jgome043@ucr.edu
 *	Lab Section: 021
 *	Assignment: Lab 7  Exercise 2
 *	Exercise Description: Using the ATmega1284’s PWM functionality, 
 *  design a system where the notes: C4, D, E, F, G, A, B, and
 *  C5, from the table at the top of the lab, can be generated 
 *  on the speaker by scaling up or down the eight note scale. 
 *  Three buttons are used to control the system. One button toggles 
 *  sound on/off. The other two buttons scale up, or down, the 
 *  eight note scale.
 *  Criteria:
 *    The system should scale up/down one note per button press.
 *    When scaling down, the system should not scale below a C.
 *    When scaling up, the system should not scale above a C.
 *  Hints:
 *    Breaking the system into multiple synchSMs could make this part easier.
 *	
 *	
 *	I acknowledge all content contained herein, excluding template or example 
 *	code, is my own original work.
 */ 


#include <avr/io.h>

//////////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////
enum notes {init, wait, up, down, ret} tune;
enum states {start, ready, rel, pulse, off} state;
double music[8] = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25};
	//{C4, D4, E4, F4, G4, A4, B4, C5}//
unsigned char i;
double Hz;

void tick_Tune(){//TUNE
	switch(tune){//Tune states
		case init:
		   tune = wait;
	   	   break;
		case wait://higher frequency / lower frequency / wait
		   if((~PINA & 0x03) == 0x01){
			   tune = up;
		   }else if((~PINA & 0x03) == 0x02){
			   tune = down;
		   }else if((~PINA & 0x03) == 0x00){
		       tune = wait;
		   }
		   break;
		case up://higher frequency
		       tune = ret;
		   break;
		case down://lower frequency
		       tune = ret;
		   break;
		case ret:// wait for both buttons' release / return to waiting 
		   if(((~PINA & 0x03) == 0x01) || ((~PINA & 0x03) == 0x02)){
			   tune = ret;
		   }else if(!((~PINA & 0x03) == 0x01) && !((~PINA & 0x03) == 0x02)){
			   tune = wait;
		   }
		   break;
		default://default state
		       tune = wait;
		   break;
	}//Tune states
	
	switch(tune){//Tune actions
		case init://initial state
		    i = 0;
			break;
		case wait:// initialize Hz to the first element of the array
		    Hz = music[i];
			break;
		case up:// higher frequency if i < 7 / i = 7
		    if (i < 7){
				i++;
			}else if(i >= 7){
				i = 7;
			}
			break;
		case down://lower frequency if i >= 1 / i = 0
		    if(i >= 1){
				i--;
			}else if(i == 0){
				i = 0;
			}
			break;
		case ret:
			break;
		default:// default Hz to music[0] / C4
		    i = 0;
			break;
	}//Tune actions
}//TUNE

void tick_Play(){//SOUND
	switch(state){//sound states
		case start://start / move to ready
		    state = ready;
			break;
		case ready:// press button / wait
		     state = (~PINA & 0x04) ? rel : ready;
			break;
		case rel:// button release / pulse
		    state = (~PINA & 0x04) ? rel : pulse; 
			break;
		case pulse:// pulse / toggle off
		    state = (~PINA & 0x04) ? off : pulse;
			break;
		case off:// off / back to ready state
		    state = (~PINA & 0x04) ? off : ready;
			break;
		default:// default to the ready state
		    state = ready;
			break;
		
	}//sound states
	
	switch(state){//sound actions
		case start://initialize variables
		    Hz = 0;
		    set_PWM(0);
			break;
		case ready://sound is off
		    set_PWM(0);
			break;
		case rel://do nothing
			break;
		case pulse://sound is on
		    set_PWM(Hz);
			break;
		case off://do nothing
			break;
		default:
		    break;
		
	}//sound actions
}//SOUND



int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;//set PORTA to input and instantiate inputs
	DDRB = 0xFF; PORTB = 0x00;//PB6 speaker output
	//initialize the state machines
	state = start;
	tune = init;
	PWM_off();// call turn off
	PWM_on();// call turn on
	
	while(1)
	{
		// Call tune, then call Play
		tick_Tune();
		tick_Play();
		
	}
}