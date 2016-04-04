/*	jgome043_lab2_part5.c - 3/30/2016 8:45:14 PM
 *	Name & E-mail: Joel Gomez - jgome043@ucr.edu
 *	CS Login: jgome043
 *	Partner(s) Name & E-mail: Eric Marcelo - emarc003@ucr.edu
 *	Lab Section: 021
 *	Assignment: Lab #2  Exercise #5
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
	DDRD = 0x00; PORTD = 0xFF;	//set PORTD to inputs
	DDRB = 0xFE; PORTB = 0x00;	//set PORTB to outputs and input
	
	unsigned char upper_weight = 0;     //temp var to hold upper 8 bits of 
                                        //weight
	unsigned short full_weight = 0;	    //var to hold store full 9 bit weight
	const unsigned char max = 70;       //min weight to trigger airbag
	const unsigned char min = 5;        //min weight to consider passenger
    unsigned char tempB = 0;            //temp var for PORTB buffer

	while(1)
	{
		upper_weight = PIND;	//store upper 8 bits of weight
		full_weight = upper_weight;
		full_weight = (full_weight << 1) + (PINB & 0x01);	//shift upper
													//8 bits of weight to
													//proper places and add
													//value of PB0
		
		//large enough passenger detected
		if (full_weight >= max) {
			tempB = SetBit(PORTB, 1, 1);	//enable airbag
			tempB = SetBit(PORTB, 2, 0);	//clear disabled light
		}
		//passenger too small for airbag
		else if (full_weight > min && full_weight < max) {
			tempB = SetBit(PORTB, 1, 0);	//disable airbag
			tempB = SetBit(PORTB, 2, 1);	//set disabled light
		}
		//no passenger
		else {
			tempB = SetBit(PORTB, 1, 0);	//disable airbag
			tempB = SetBit(PORTB, 2, 0);	//clear disabled light
		}
        PORTB = tempB;                  //set PORTB
	}
}
