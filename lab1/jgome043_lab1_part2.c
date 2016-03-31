/*
 *  jgome043_lab1_part2.c
 *
 *  Created: 3/28/2016 6:26:15 PM
 *	Name & E-mail: Joel Gomez - jgome043@ucr.edu
 *	CS Login: jgome043
 *	Partner(s) Name & E-mail: Eric Marcelo - emarc003@ucr.edu
 *	Lab Section: 021
 *	Assignment: Lab #1  Exercise #2
 *	Exercise Description:
 *
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>


int main(void)
{
    DDRA = 0x00; PORTA = 0xFF;	//config port A to inputs, write to all 1
    DDRC = 0xFF; PORTC = 0x00;	//config port C to outputs, write to all 0

    const unsigned char spaces = 4;	//total parking space allocated
    unsigned char cntavail = 0;	//temp var to hold available spaces
    unsigned char occupied = 0;	//temp var to store spaces taken
    while (1)
    {
		/*
		PINA & 0x01 returns a 1 if PA0 is set
		(PINA >> %b) shifts bits right by %b
		(PINA >> 1) & 0x01 returns 1 if PA1 is set
		(PINA >> 2) & 0x01 returns 1 if PA2 is set
		(PINA >> 3) & 0x01 returns 1 if PA3 is set
		*/

		//sum up the currently occupied spaces
		occupied = (PINA & 0x01) + ((PINA >> 1) & 0x01) +
			((PINA >> 2) & 0x01) + ((PINA >> 3) & 0x01);

		cntavail = spaces - occupied;	//store the total spaces available
		PORTC = cntavail;	//update available space output on PORTC
    }
	return 0;
}
