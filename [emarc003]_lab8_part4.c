/*	[emarc003]_lab8_part4.c - $date$
 *	Name & E-mail:  - Eric Marcelo, emarc003@ucr.edu
 *	CS Login: emarc003
 *	Partner(s) Name & E-mail:  - Joel Gomez, jgome043@ucr.edu
 *	Lab Section: 021
 *	Assignment: Lab #  Exercise # 
 *	Exercise Description:
 *	
 *	
 *	I acknowledge all content contained herein, excluding template or example 
 *	code, is my own original work.
 */ 


#include <avr/io.h>
void tick(){
	const unsigned short sig = ADC;
	
	//if(sig >= 0x8F){
	if(sig < 0x8F){
		PORTB = 0xFF;
	}else if((sig > 0xA1) && (sig < 0xB3)){//161
		PORTB = 0x7F;
	}else if((sig > 0xB3) && (sig < 0xC5)){//179
		PORTB = 0x3F;
	}else if((sig > 0xC5) && (sig < 0xD7)){//197
		PORTB = 0x1F;
	}else if((sig > 0xD7) && (sig < 0xE9)){//215
		PORTB = 0x0F;
	}else if((sig > 0xE9) && (sig < 0xFB)){//233
		PORTB = 0x07;
	}else if((sig > 0xFB) && (sig < 0x10D)){//251
		PORTB = 0x03;
	}else if(sig > 0x10D){//269
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