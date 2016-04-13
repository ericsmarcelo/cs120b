/*	jgome043_lab5_partB1.c - 4/13/2016 1:54:01 AM
 *	Name & E-mail: Joel Gomez - jgome043@ucr.edu
 *	CS Login: jgome043
 *	Partner(s) Name & E-mail: Eric Marcelo - emarc003@ucr.edu
 *	Lab Section: 021
 *	Assignment: Lab #5  Exercise #B1
 *
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */


#include <avr/io.h>
#include "io.c"
#include "timer.h"

enum States { START, INIT, WAIT, INC, DEC } state;      // state vars
unsigned char cnt;                      // counter
unsigned char input_a;                  // storage for PINA
unsigned char clk_cnt;                  // clock counter
const unsigned char sec = 8;            // periods per second

void increment() {
    cnt = (cnt < 9) ? cnt + 1 : cnt;    // inc cnt if within range
    LCD_Cursor(1);                      // set LCD cursor
    LCD_WriteData(cnt + '0');           // write current count
}

void decrement() {
    cnt = (cnt > 0) ? cnt - 1 : cnt;    // dec cnt if within range
    LCD_Cursor(1);                      // set LCD cursor
    LCD_WriteData(cnt + '0');           // write current count
}

void Tick() {
    
    input_a = ~PINA;                    // store PINA

    // State Transitions
    switch (state) {
        case START:
            state = INIT; break;        // transition to INIT state
        case INIT:
            state = WAIT; break;        // transition to WAIT state
        case WAIT:
            // check if ONLY btn0 is pressed
            if (input_a == 0x01) {
                increment();                        // increment counter
                state = INC;                        // transition to increment state
                clk_cnt++;                          // increment clock counter
            }
            // check if ONLY bt1 is pressed
            else if (input_a == 0x02) {
                decrement();                        // decrement counter
                state = DEC;                        // transition to decrement state
                clk_cnt++;                          // increment clock counter
            }
            // check if both buttons are pressed
            else if (input_a == 0x03) {
                state = INIT;                       // go back to INIT state
            }
            break;
        case INC:
            // check if button still pressed and 1 second has passed
            if (input_a == 0x01 && !(clk_cnt % sec)) {
                increment();                        // increment counter
            }
            // check if both buttons are pressed
            else if (input_a == 0x03) {
                state = INIT;                       // go back to INIT state
            }
            // check that button has been released
            else if (input_a ^ 0x01) {
                state = WAIT;                       // go back to WAIT state
                clk_cnt = 0;                        // clear clk_cnt
            }
            break;
        case DEC:
            if (input_a == 0x02 && !(clk_cnt % sec)) {
                decrement();                        // decrement counter
            }
            // check if both buttons are pressed
            else if (input_a == 0x03) {
                state = INIT;                       // go back to INIT state
            }
            // check that button has been released
            else if (input_a ^ 0x02) {
                state = WAIT;                       // go back to WAIT state
                clk_cnt = 0;                        // clear clk_cnt
            }
            break;
        default:
            state = START;              // error likely happened, restart
            break;
    }

    // State Actions
    switch (state) {
        case INIT:
            LCD_ClearScreen();  // clear LCD
            cnt = 0;            // initialize temp var
            clk_cnt = 0;      // initialize clock counter
            LCD_Cursor(1);
            LCD_WriteData(cnt + '0'); break;
        case INC:
            // increment the clock counter if button still held
            clk_cnt = (input_a & 0x01) ? clk_cnt + 1 : clk_cnt; break;
        case DEC:
            // increment the clock counter if button still held
            clk_cnt = (input_a & 0x02) ? clk_cnt + 1 : clk_cnt; break;
        default:
            break;
    }
}

int main(void)
{
    DDRA = 0x00; PORTA = 0xFF;	// set PORTA to inputs and initialize
    DDRC = 0xFF; PORTC = 0x00;  // data lines
    DDRD = 0xFF; PORTD = 0x00;  // LCD control lines

    LCD_init();

    TimerSet(125);             // set Timer period
    TimerOn();                  // enable Timer

    state = START;              // initialize state
    
	while(1)
	{
		Tick();                 // step through states
        while (!TimerFlag);     // Wait for a period
        TimerFlag = 0;          // reset the timer flag
	}
}
