/*	[emarc003]_lab8_part3.c - $5/2/2016$
 *	Name & E-mail:  - Eric Marcelo, emarc003@ucr.edu
 *	CS Login: emarc003
 *	Partner(s) Name & E-mail:  - Joel Gomez, jgome043@ucr.edu
 *	Lab Section: 021
 *	Assignment: Lab 8  Exercise 3 
 *	Exercise Description: Design a system where an LED is illuminated if
 *  enough light is detected from the photo resistor.
 *  Criteria:
 *    If the result of the ADC is >= MAX/2, the LED is illuminated.
 *    If the result of the ADC is < MAX/2, the LED is turned off.
 *  Notes:
 *    MAX is the highest ADC value observed from part 2 of the lab.
 *    Video Demonstration:
 *	
 *	
 *	I acknowledge all content contained herein, excluding template or example 
 *	code, is my own original work.
 */ 


#include <avr/io.h>
void tick(){
	const unsigned short sig = ADC;
	
	if(sig >= 0x8F){
		PORTB = 0x00;
	}else if(sig < 0x8F){
		PORTB = 0x01;
	}
	
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
	
	ADC_init();
	
	while(1)
	{
		// TODO: Please write your application code
		tick();
	}
}