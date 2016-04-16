/*	[emarc003]_lab4_part1.c - $4/13/2016$
 *	Name & E-mail:  - Eric Marcelo, emarc003@ucr.edu
 *	CS Login: emarc003
 *	Partner(s) Name & E-mail:  - Joel Gomez, jgome043@ucr.edu
 *	Lab Section: 021
 *	Assignment: Lab 4  Exercise 1
 *	Exercise Description: (From an earlier lab) A car has a fuel-level sensor that sets PA3..PA0 
 *  to a value between 0 (empty) and 15 (full). A series of LEDs connected to PC5..PC0 should 
 *  light to graphically indicate the fuel level. If the fuel level is 1 or 2, PC5 lights. 
 *  If the level is 3 or 4, PC5 and PC4 light. 5-6 lights PC5..PC3. 7-9 lights PC5..PC2. 10-12 
 *  lights PC5..PC1. 13-15 lights PC5..PC0. Also, PC6 connects to a "Low fuel" icon, which should
 *  light if the level is 4 or less. Use buttons on PA3..PA0 and mimic the fuel-level sensor with presses.
 *	
 *	
 *	I acknowledge all content contained herein, excluding template or example 
 *	code, is my own original work.
 */ 


#include <avr/io.h>
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b){
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}

unsigned char GetBit(unsigned char x, unsigned char k){
	return ((x & (0x01 << k)) != 0);
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; //set PORTA to input (0x00)
	DDRC = 0xFF; PORTC = 0x00; //set PORTC to output (0xFF) and initialize to 0x00
	unsigned char tempC = 0x00; //variable used for bit manipulation
	unsigned char tempA = 0x00;
	//unsigned char i;
	
	while(1)
	{
		tempA = ~PINA;
		
		//fuel indicator light sequence
		if(tempA <= 0x04){// fuel is 4 or less PC6 "Low fuel" icon
			tempC = SetBit(tempC, 6, 1);
		}
		//clear "Low fuel" icon if fuel is greater than level 4
		else{
			tempC = SetBit(tempC, 6, 0);
		}
		if(tempA >= 0x01){//fuel is level 1 or 2 PC5
			tempC = SetBit(tempC, 5, 1);
		}
		else{
			tempC = SetBit(tempC, 5, 0);
		}
		if(tempA >= 0x03){//fuel is level 3 or 4 PC4
			tempC = SetBit(tempC, 4, 1);
		}
		else{
			tempC = SetBit(tempC, 4, 0);
		}
		if(tempA >= 0x05){//fuel is level 5-6 PC3
			tempC = SetBit(tempC, 3, 1);
		}
		else {
			tempC = SetBit(tempC, 3, 0);
		}
		if(tempA >= 0x07){//fuel is level 7-9 PC2
			tempC = SetBit(tempC, 2, 1);
		}
		else {
			tempC = SetBit(tempC, 2, 0);
		}
		if(tempA >= 0x0A){//fuel is level 10-12 PC1
			tempC = SetBit(tempC, 1, 1);
		}
		else {
			tempC = SetBit(tempC, 1, 0);
		}
		if(tempA >= 0x0D){//fuel is level 13-15 PC0
			tempC = SetBit(tempC, 0, 1);
		}
		else{
			tempC = SetBit(tempC, 0, 0);
		}
		PORTC = tempC; // output the fuel level and icons
	}
	return 0;
}
