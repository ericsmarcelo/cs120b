/*	jgome043_lab5_partB2.c - 4/13/2016 3:32:16 AM
 *	Name & E-mail: Joel Gomez - jgome043@ucr.edu
 *	CS Login: jgome043
 *	Partner(s) Name & E-mail: Eric Marcelo - emarc003@ucr.edu
 *	Lab Section: 021
 *	Assignment: Lab #5  Exercise #B2
 *
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */


#include <avr/io.h>
#include "timer.h"
#include "io.c"

enum States { START, INIT, PLAY, PAUSE } state;      // state vars
unsigned char tmpB;                     // temp var for PORTB manip
unsigned char rev;                      // flag to determine direction
unsigned char btn;                      // var to store button state
unsigned char wait;                     // flag to help handle pause
unsigned char cnt;                      // counter for LED 
unsigned char score;                    // player's score
const unsigned char* win = "YOU'RE THE BEST!";  // winner message
const unsigned char* lose = "YOU'RE THE WORST"; // loser message

void increment() {
    score = (score < 9) ? score + 1 : score;    // inc score if within range
    LCD_Cursor(1);                              // set LCD cursor
    LCD_WriteData(score + '0');                 // write current score
}

void decrement() {
    score = (score > 0) ? score - 1 : score;    // dec score if within range
    LCD_Cursor(1);                              // set LCD cursor
    LCD_WriteData(score + '0');                 // write current score
}

void Tick() {
    
    btn = ~PINA & 0x01;                 // get current state if PINA

    // State Transitions
    switch (state) {
        case START:
            state = INIT; break;        // transition to INIT state
        case INIT:
            rev = 0;                    // initialize rev flag
            wait = 0;                   // initialize wait flag
            state = PLAY; break;        // transition to PLAY state
        case PLAY:
            // check for btn press
            if (btn) {
                state = PAUSE;          // transition to PAUSE state
                wait = 1;               // set wait flag
                cnt = 0;                // reset counter
                if (tmpB & 0x02) {
                    increment();        // increment score
                    // check if player is awesome
                    if (score == 9) {
                        LCD_DisplayString(1, win);  // display winning msg
                    }
                }
                else {
                    decrement();        // decrement score
                    // check if player is not awesome
                    if (score == 0) {
                        LCD_DisplayString(1, lose); // display loser msg
                    }
                }
            }
            cnt++;                      // increment counter
            break;
        case PAUSE:
            // check for button and wait flag
            if (!btn && wait) {
                wait = 0;   //clear wait flag
            }
            // check for button and wait flag not set
            else if (btn && !wait) {
                // check if game over
                if (score == 9 || score == 0) {
                    state = INIT;
                }
                else {
                    state = PLAY;           //transition to PLAY state
                }
            }
            break;
        default:
            state = START;              // error likely happened, restart
            break;
    }

    // State Actions
    switch (state) {
        case INIT:
            LCD_ClearScreen();                      // clear LCD
            tmpB = (tmpB & 0x00) | 0x01;            // initialize temp var
            score = 5;                              // initialize temp var
            LCD_Cursor(1);                          // set LCD cursor
            LCD_WriteData(score + '0'); break;      // write initial score to LCD
        case PLAY:
            // check that counter is on even number (2x period = 300ms)
            if (cnt % 2) {
                tmpB = (!rev) ? (tmpB << 1) : (tmpB >> 1);  // illum next LED
            }
            // check if LED is at low edge
            if (tmpB <= 0x01 && rev) {
                rev = 0;    // return to normal direction
            }
            // check if LED is at high edge
            else if (tmpB >= 0x04 && !rev) {
                rev = 1;    // reverse direction
            }
            break;
        default:
            break;
    }

    PORTB = tmpB;               // set PORTB
}

int main(void)
{
    DDRA = 0x00; PORTA = 0xFF;	// set PORTA to inputs and initialize
    DDRB = 0xFF; PORTB = 0x00;	// set PORTB to outputs and initialize
    DDRC = 0xFF; PORTC = 0x00;  // data lines
    DDRD = 0xFF; PORTD = 0x00;  // LCD control lines

    LCD_init();

    TimerSet(150);             // set Timer period
    TimerOn();                  // enable Timer

    state = START;              // initialize state

    while(1)
    {
        Tick();                 // step through states
        while (!TimerFlag);     // Wait for a period
        TimerFlag = 0;          // reset the timer flag
    }
}