/*	[emarc003]_lab8_part1.c - $5/2/2016$
 *	Name & E-mail:  - Eric Marcelo, emarc003@ucr.edu
 *	CS Login: emarc003
 *	Partner(s) Name & E-mail:  - Joel Gomez, jgome043@ucr.edu
 *	Lab Section: 021
 *	Assignment: Lab 8  Exercise 1 
 *	Exercise Description: Make sure your breadboard is wired according to the prelab. 
 *  The potentiometer is used to adjust the voltage supplied to the microcontroller for ADC . 
 *  Design a system that reads the 10-bit ADC result from the ADC register, and 
 *  displays the result on a bank of 10 LEDs.
 *	
 *	
 *	I acknowledge all content contained herein, excluding template or example 
 *	code, is my own original work.
 */ 


#include <avr/io.h>
void tick(){
	const unsigned short sig = ADC;
	
	unsigned char low = (char)sig; // creates the lower eight bits
	unsigned char high = (char)(sig >> 8); // the upper 2 bits
	PORTB = low;
	PORTD = high;
}

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	// in Free Running Mode, a new conversion will trigger whenever
	// the previous conversion completes.
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // set PORTA to input
	DDRB = 0xFF; PORTB = 0x00; // set PORTB to output and initialize to 0x00
	DDRD = 0xFF; PORTD = 0x00; // set PORTD to output and initialize to 0x00
	ADC_init();
	
	while(1)
	{
		// TODO: Please write your application code 
		tick();
	}
}