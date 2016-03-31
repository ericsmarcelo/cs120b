/*	jgome043_lab2_part3.c - 3/30/2016 5:51:41 PM
 *	Name & E-mail: Joel Gomez - jgome043@ucr.edu
 *	CS Login: jgome043
 *	Partner(s) Name & E-mail: Eric Marcelo - emarc003@ucr.edu
 *	Lab Section: 021
 *	Assignment: Lab #2  Exercise #3
 *	Exercise Description:
 *
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */


#include <avr/io.h>

//function to set bit
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
 //function to get bit value
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;	//set PORTA to inputs
	DDRC = 0xFF; PORTC = 0x00;	//set PORTC to outputs
	
	unsigned char fuel_level = 0;	//var to hold numerical fuel level
	
	while(1)
	{
		fuel_level = (PINA & 0x0F);	//get current fuel level
		
		//check if key is in ignition, driver is seated and seatbelt 
		//not fastened
		if (GetBit(PINA, 4) && GetBit(PINA, 5) && !GetBit(PINA, 6)) {
			PORTC = SetBit(PORTC, 7, 1);	//illuminate seatbelt
											//warning lamp
		}
		else {
			PORTC = SetBit(PORTC, 7, 0);	//clear seatbelt
											//warning lamp
		}
		
		//check if fuel level is near empty
		if (fuel_level <= 4) {
			PORTC = SetBit(PORTC, 6, 1);	//illum low fuel lamp
		}
		else {
			PORTC = SetBit(PORTC, 6, 0);	//clear low fuel lamp
		}
		
		//check fuel level, illuminate/clear appropriate LED segments
		if (fuel_level >= 1) {
			PORTC = SetBit(PORTC, 5, 1);
		}
		else {
			PORTC = SetBit(PORTC, 5, 0);
		}
		if (fuel_level >= 3) {
			PORTC = SetBit(PORTC, 4, 1);
		}
		else {
			PORTC = SetBit(PORTC, 4, 0);
		}
		if (fuel_level >= 5) {
			PORTC = SetBit(PORTC, 3, 1);
		}
		else {
			PORTC = SetBit(PORTC, 3, 0);
		}
		if (fuel_level >= 7) {
			PORTC = SetBit(PORTC, 2, 1);
		}
		else {
			PORTC = SetBit(PORTC, 2, 0);
		}
		if (fuel_level >= 10) {
			PORTC = SetBit(PORTC, 1, 1);
		}
		else {
			PORTC = SetBit(PORTC, 1, 0);
		}
		if (fuel_level >= 13) {
			PORTC = SetBit(PORTC, 0, 1);
		}
		else {
			PORTC = SetBit(PORTC, 0, 0);
		}
	}
}
