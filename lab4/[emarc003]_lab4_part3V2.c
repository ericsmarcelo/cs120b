/*	[emarc003]_lab4_part3V2.c - $date$
 *	Name & E-mail:  - Eric Marcelo, emarc003@ucr.edu
 *	CS Login: emarc003
 *	Partner(s) Name & E-mail:  - Joel Gomez, jgome043@ucr.edu
 *	Lab Section: 021
 *	Assignment: Lab 4  Exercise 3
 *	Exercise Description: (Challenge) Create your own festive lights display 
 *  with 6 LEDs connected to port PC5..PC0, lighting in some attractive sequence. 
 *  Pressing the button on PA0 changes the lights to the next configuration in 
 *  the sequence. Use a state machine (not synchronous) captured in C.
 *	
 *	
 *	I acknowledge all content contained herein, excluding template or example 
 *	code, is my own original work.
 */ 


#include <avr/io.h>
unsigned char tempC;
unsigned char i, j;

enum K_States { Start, K_Init, K_WAIT, K_press, K_P1, K_P2, K_P1_Result, K_P2_Result, K_WAITP1, K_WAITP2, WAITP1, WAITP2 } K_State;

int Tick() {
	switch(K_State) { // Transitions
		
		case Start: // start
			K_State = K_Init;
			break;
		case K_Init:// initial (same as start)
			if (1) {
				K_State = K_WAIT;
			}
			break;
		case K_WAIT:// wait for button press
			if (!(~PINA & 0x08) && !(~PINA & 0x04) && !(~PINA & 0x02) && (~PINA & 0x01)) {//0001
				K_State = K_press;
			}
			else if (!(~PINA & 0x01) && !(~PINA & 0x02) && !(~PINA & 0x04) && !(~PINA & 0x08)) {//0000
				K_State = K_WAIT;
			}
				break;
	    case K_press:// wait for button release
			if (~PINA & 0x01){//0001
				K_State = K_press;
			}
			else if (!(~PINA & 0x01) && !(~PINA & 0x02) && !(~PINA & 0x04) && !(~PINA & 0x08)) {//0000
				K_State = K_P1;
			}
		break;
		case K_P1:// pulse one / on button release move to pulse one result / switch to pulse two
			if (i < 7 && (!(~PINA & 0x01) && !(~PINA & 0x02) && !(~PINA & 0x04) && !(~PINA & 0x08))) {//0000 && i < 7
				K_State = K_P1_Result;
			}
			else if (i >= 7) {
				K_State = K_P2;
				j = 0;
			}
			break;
		case K_P2:// pulse two / on button release move to pulse two result / switch to pulse one
			if (j < 7 && (!(~PINA & 0x01) && !(~PINA & 0x02) && !(~PINA & 0x04) && !(~PINA & 0x08))) {//0000 && j < 7
				K_State = K_P2_Result;
			}
			else if (j >= 7) {
				K_State = K_P1;
				i = 0;
			}
			break;
		case K_P1_Result:// display result of pulse one / move to waiting / increment i
			if (1) {
				K_State = K_WAITP1;
				i++;
				}
			break;
		case K_P2_Result:// display result of pulse two / move to waiting / increment j
			if (1) {
				K_State = K_WAITP2;
				j++;
			}
			break;
		case K_WAITP1:// wait for next button press during pulse one
		   if (!(~PINA & 0x01) && !(~PINA & 0x02) && !(~PINA & 0x04) && !(~PINA & 0x08)){//0000
			   K_State = K_WAITP1;
		   }else if((~PINA & 0x01) && !(~PINA & 0x02) && !(~PINA & 0x04) && !(~PINA & 0x08))//0001
		       K_State = WAITP1;
			break;
		case K_WAITP2:// wait for next button press during pulse two
			if (!(~PINA & 0x01) && !(~PINA & 0x02) && !(~PINA & 0x04) && !(~PINA & 0x08)){//0000
				K_State = K_WAITP2;
			}else if((~PINA & 0x01) && !(~PINA & 0x02) && !(~PINA & 0x04) && !(~PINA & 0x08))//0001
			K_State = WAITP2;
		   break;
		case WAITP1: // wait for button release
		   if (!(~PINA & 0x01) && !(~PINA & 0x02) && !(~PINA & 0x04) && !(~PINA & 0x08)){//0000
			   K_State = K_P1;
		   }else if((~PINA & 0x01) && !(~PINA & 0x02) && !(~PINA & 0x04) && !(~PINA & 0x08))//0001
		   K_State = WAITP1;
		   break;
		case WAITP2:// wait for button release
			if (!(~PINA & 0x01) && !(~PINA & 0x02) && !(~PINA & 0x04) && !(~PINA & 0x08)){//0000
				K_State = K_P2;
			}else if((~PINA & 0x01) && !(~PINA & 0x02) && !(~PINA & 0x04) && !(~PINA & 0x08))//0001
			K_State = WAITP2;
			break;
		default:
		break;
    }
		switch(K_State) { // State actions
			case K_Init:// initialize variables
				tempC = 0x00;
				PORTB = 0x00;
				i = 0;
				j = 0;
				break;
			case K_WAIT:// do nothing
				break;
			case K_press:// initialize counters
				i = 0;
				j = 0;
				break;
			case K_P1:// reset tempC variable and initialize output
				tempC = 0x00;
				tempC = (tempC | (0x07 <<  i));
				break;
			case K_P2:// reset tempC variable and initialize output
				tempC = 0x00;
				tempC = (tempC | (0xE0 >> j));
				break;
			case K_P1_Result:// output for pulse one
				PORTC = tempC;
				break;
			case K_P2_Result://output for pulse two
				PORTC = tempC;
				break;
			case K_WAITP1:// do nothing
			   break;
			case K_WAITP2:// do nothing
			   break;
			case WAITP1:// do nothing
			   break;
			case WAITP2:// do nothing
			   break;
			default: // ADD default behavior below
			break;
		} // State actions

	}	
		
int main(void)
{
	K_State = Start; // Initial state
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00; // Init outputs
	
	while(1)
	{
		// TODO: Please write your application code 
		Tick();
	}
}