/*	jgome043_lab2_part4.c - 3/30/2016 8:22:13 PM
 *	Name & E-mail: Joel Gomez - jgome043@ucr.edu
 *	CS Login: jgome043
 *	Partner(s) Name & E-mail: Eric Marcelo - emarc003@ucr.edu
 *	Lab Section: 021
 *	Assignment: Lab #1  Exercise #4
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
	
	unsigned char val = 0;		//temp var for PORTA values
	unsigned char upper_a = 0;	//temp var for PA7...PA4
	unsigned char lower_a = 0;	//temp var for PA3...PA0
    unsigned char tempC = 0;    //temp var for PORTC buffer

	while(1)
	{
		val = PINA;	//get PORTA values;
		lower_a = (val & 0x0F) << 4;	//store bits PA3...PA0 and shift
										//shift them to upper bits places
		upper_a = (val & 0xF0) >> 4;	//store bits PA7...PA4 and shift
										//shift them to lower bits places
		tempC = ((PORTC & 0x00) | lower_a) | upper_a;	//clear and store bits
        PORTC = tempC;                  //set PORTC
	}
}
