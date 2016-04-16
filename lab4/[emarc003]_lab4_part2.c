/*	[emarc003]_lab4_part2.c - $4/11/2016$
 *	Name & E-mail:  - Eric Marcelo, emarc003@ucr.edu
 *	CS Login: emarc003
 *	Partner(s) Name & E-mail:  - Joel Gomez, jgome043@ucr.edu
 *	Lab Section: 021
 *	Assignment: Lab 4  Exercise 2
 *	Exercise Description: (From an earlier lab) Buttons are connected to PA0 and PA1. 
 *  Output for PORTC is initially 0. Pressing PA0 increments PORTC (stopping at 9). 
 *  Pressing PA1 decrements PORTC (stopping at 0). If both buttons are depressed 
 *  (even if not initially simultaneously), PORTC resets to 0. If a reset occurs, 
 *  both buttons should be fully released before additional increments or decrements 
 *  are allowed to happen. Use LEDs (and resistors) on PORTC. Use a state machine 
 *  (asynchronous) captured in C.
 *	
 *	
 *	I acknowledge all content contained herein, excluding template or example 
 *	code, is my own original work.
 */ 


#include <avr/io.h>
enum SM_Inc_Dec_States { Start, SM_Inc_Dec_init, SM_Inc_Dec_Result, SM_Inc_Dec_Increment, SM_Inc_Dec_wait9, SM_Inc_Dec_verify9, SM_Inc_Dec_BOTH, SM_Inc_Dec_Decrement, SM_Inc_Dec_wait0, SM_Inc_Dec_verify0 } SM_Inc_Dec_State;
unsigned char tempB;

int TickFct_Inc_Dec() {
	switch(SM_Inc_Dec_State) { // Transitions
		case Start:// Start
			SM_Inc_Dec_State = SM_Inc_Dec_init;
			break;
		case SM_Inc_Dec_init://Initial state (same as start)
			if (1) {
				SM_Inc_Dec_State = SM_Inc_Dec_Result;
			}
			break;
		case SM_Inc_Dec_Result: // move to increment /  reset / move to decrement
			if (!(~PINA & 0x02) && (~PINA & 0x01) ) {// 01 
				SM_Inc_Dec_State = SM_Inc_Dec_Increment;
			}
			else if ((~PINA & 0x02) && (~PINA & 0x01)) {// 11
				SM_Inc_Dec_State = SM_Inc_Dec_BOTH;
			}
			else if ((~PINA & 0x02) && !(~PINA & 0x01)) {// 10
				SM_Inc_Dec_State = SM_Inc_Dec_Decrement;
			}
			break;
		case SM_Inc_Dec_Increment: // increment the count / reset
			if (1) {
				SM_Inc_Dec_State = SM_Inc_Dec_wait9;
			}
			else if ((~PINA & 0x02) && (~PINA & 0x01)) {// 11
				SM_Inc_Dec_State = SM_Inc_Dec_BOTH;
			}
			break;
		case SM_Inc_Dec_wait9: // wait for button release / move to verification / reset
			if (!(~PINA & 0x02) && (~PINA & 0x01)) {// 01
				SM_Inc_Dec_State = SM_Inc_Dec_wait9;
			}
			else if (!(~PINA & 0x02) && !(~PINA & 0x01)) {// 00
				SM_Inc_Dec_State = SM_Inc_Dec_verify9;
			}
			else if ((~PINA & 0x02) && (~PINA & 0x01)) {// 11
				SM_Inc_Dec_State = SM_Inc_Dec_BOTH;
			}
			break;
		case SM_Inc_Dec_verify9:// verify the total is less than or equal to nine / reset
			if (1) {
				SM_Inc_Dec_State = SM_Inc_Dec_Result;
			}
			else if ((~PINA & 0x02) && (~PINA & 0x01)) {// 11
				SM_Inc_Dec_State = SM_Inc_Dec_BOTH;
			}
			break;
		case SM_Inc_Dec_BOTH: // Reset the value / wait until both are released / then move to result
			if (((~PINA & 0x02) && (~PINA & 0x01)) || (!(~PINA & 0x02) && (~PINA & 0x01)) || ((~PINA & 0x02) && !(~PINA & 0x01))) {// 11 || 01 || 10
				SM_Inc_Dec_State = SM_Inc_Dec_BOTH;
			}
			else if (1) {
				SM_Inc_Dec_State = SM_Inc_Dec_Result;
			}
			break;
		case SM_Inc_Dec_Decrement:// Decrement the count button press
			if ((~PINA & 0x02) && !(~PINA & 0x01)) {//10
				SM_Inc_Dec_State = SM_Inc_Dec_wait0;
			}
			else if ((~PINA & 0x02) && (~PINA & 0x01)) {// 11
				SM_Inc_Dec_State = SM_Inc_Dec_BOTH;
			}
			break;
		case SM_Inc_Dec_wait0: // wait for button release / move to verification / reset
			if ((~PINA & 0x02) && !(~PINA & 0x01)) {// 10
				SM_Inc_Dec_State = SM_Inc_Dec_wait0;
			}
			else if (!(~PINA & 0x02) && !(~PINA & 0x01)) {// 00
				SM_Inc_Dec_State = SM_Inc_Dec_verify0;
			}
			else if ((~PINA & 0x02) && (~PINA & 0x01)) {// 11
				SM_Inc_Dec_State = SM_Inc_Dec_BOTH;
			}
			break;
		case SM_Inc_Dec_verify0:// Verify validity of total / reset
			if (1) {
				SM_Inc_Dec_State = SM_Inc_Dec_Result;
			}
			else if ((~PINA & 0x02) && (~PINA & 0x01)) {// 11
				SM_Inc_Dec_State = SM_Inc_Dec_BOTH;
			}
			break;
		default: // default
			SM_Inc_Dec_State = SM_Inc_Dec_init;
			break;
	} // Transitions

	switch(SM_Inc_Dec_State) { // State actions
		case SM_Inc_Dec_init:// initialize values
			PORTC = 0;
			tempB = 0;
			break;
		case SM_Inc_Dec_Result: // output result
			PORTC = tempB;
			break;
		case SM_Inc_Dec_Increment: // increment
			tempB = tempB + 1;
			break;
		case SM_Inc_Dec_wait9: // do nothing
			break;
		case SM_Inc_Dec_verify9: // verify the total value is less than or equal to 9
			if(tempB >9){
			tempB = 9;}
			break;
		case SM_Inc_Dec_BOTH: // reset
			tempB = 0;
			break;
		case SM_Inc_Dec_Decrement: // decrement the total value
			tempB = tempB - 1;
			break;
		case SM_Inc_Dec_wait0: // do nothing
			break;
		case SM_Inc_Dec_verify0: //verify the total value is greater than or equal to 0
			if(tempB > 9){
			tempB = 0;}
			break;
		default: // ADD default behaviour below
			break;
	} // State actions
}

int main() {
	
	SM_Inc_Dec_State = Start; // Initial state
	DDRA = 0x00; PORTA = 0xFF;//set PORTA to input
	DDRC = 0xFF; PORTC = 0x00;//set PORTC to output

	while(1) {
		TickFct_Inc_Dec();
	} // while (1)
} // Main
