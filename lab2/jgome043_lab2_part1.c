/*	jgome043_lab2_part1.c - 2016-03-30
 *	Name & E-mail: Joel Gomez - jgome043@ucr.edu
 *	CS Login: jgome043
 *	Partner(s) Name & E-mail: Eric Marcelo - emarc003@ucr.edu
 *	Lab Section: 021
 *	Assignment: Lab #2  Exercise #1 
 *	
 *	
 *	I acknowledge all content contained herein, excluding template or example 
 *	code, is my own original work.
 */ 


#include <avr/io.h>

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;  //set PORTA as inputs, set all pins to 1s
	DDRB = 0x00; PORTB = 0xFF;  //set PORTB as inputs, set all pins to 1s
	DDRC = 0xFF; PORTC = 0x00;  //set PORTC as outputs, set all pins to 0s
	
	unsigned char sum_a = 0;    //temp var for sum of PORTA
	unsigned char sum_b = 0;    //temp var for sum of PORTB
	
	while(1)
	{	
        //get sum of all bits of PORTA
		sum_a = (PINA & 0x01) + ((PINA >> 1) & 0x01) +
		((PINA >> 2) & 0x01) + ((PINA >> 3) & 0x01) +
		((PINA >> 4) & 0x01) + ((PINA >> 5) & 0x01) +
		((PINA >> 6) & 0x01) + ((PINA >> 7) & 0x01);
		
        //get sum of all bits of PORTB
		sum_b = (PINB & 0x01) + ((PINB >> 1) & 0x01) +
		((PINB >> 2) & 0x01) + ((PINB >> 3) & 0x01) +
		((PINB >> 4) & 0x01) + ((PINB >> 5) & 0x01) +
		((PINB >> 6) & 0x01) + ((PINB >> 7) & 0x01);
		
        //set PORTC to sum of PORTA and PORTB
		PORTC = sum_a + sum_b;
	}
}
