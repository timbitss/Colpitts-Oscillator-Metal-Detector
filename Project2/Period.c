// This program shows how to measure the period of a signal using timer 1 free running counter.

#define F_CPU 8000000UL
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "usart.h"

#define DEF_FREQ 2048L
#define OCR1_RELOAD ((F_CPU/(2*DEF_FREQ))+1)

volatile unsigned int reload;

unsigned int cnt = 0;

// 'Timer 1 output compare A' Interrupt Service Routine
ISR(TIMER1_COMPA_vect)
{
	OCR1A = OCR1A + reload;
	PORTC ^= 0b00000001; // Toggle PB2 PORTB ^= 0b00000011; 
}

void wait_1ms(void)
{
	unsigned int saved_TCNT1;
	
	saved_TCNT1=TCNT1;
	
	while((TCNT1-saved_TCNT1)<(F_CPU/1000L)); // Wait for 1 ms to pass
}

void waitms(int ms)
{
	while(ms--) wait_1ms();
}

#define PIN_PERIOD (PINB & 0b00000010)

// GetPeriod() seems to work fine for frequencies between 30Hz and 300kHz.
long int GetPeriod (int n)
{
	int i, overflow;
	unsigned int saved_TCNT1a, saved_TCNT1b;
	
	overflow=0;
	TIFR1=1; // TOV1 can be cleared by writing a logic one to its bit location.  Check ATmega328P datasheet page 113.
	while (PIN_PERIOD!=0) // Wait for square wave to be 0
	{
		if(TIFR1&1)	{ TIFR1=1; overflow++; if(overflow>5) return 0;}
	}
	overflow=0;
	TIFR1=1;
	while (PIN_PERIOD==0) // Wait for square wave to be 1
	{
		if(TIFR1&1)	{ TIFR1=1; overflow++; if(overflow>5) return 0;}
	}
	
	overflow=0;
	TIFR1=1;
	saved_TCNT1a=TCNT1;
	for(i=0; i<n; i++) // Measure the time of 'n' periods
	{
		while (PIN_PERIOD!=0) // Wait for square wave to be 0
		{
			if(TIFR1&1)	{ TIFR1=1; overflow++; if(overflow>1024) return 0;}
		}
		while (PIN_PERIOD==0) // Wait for square wave to be 1
		{
			if(TIFR1&1)	{ TIFR1=1; overflow++; if(overflow>1024) return 0;}
		}
	}
	saved_TCNT1b=TCNT1;
	if(saved_TCNT1b<saved_TCNT1a) overflow--; // Added an extra overflow.  Get rid of it.

	return overflow*0x10000L+(saved_TCNT1b-saved_TCNT1a);
}

int main(void)
{
	long int count;
	float T, f;

	
	reload=OCR1_RELOAD; // Reload value for default output frequency 

	DDRC=0b00000001; // PB1 (pin 15) and PB0 (pin 14) are our outputs
	PORTC |= 0x01; // PB0=NOT(PB1)
	TCCR1B |= _BV(CS10);   // set prescaler to Clock/1
	TIMSK1 |= _BV(OCIE1A); // output compare match interrupt for register A
	
	cli(); // clear global interupts
	
	usart_init(); // Configure the usart and baudrate
	
	DDRB  &= 0b11111101; // Configure PB1 as input
	PORTB |= 0b00000010; // Activate pull-up in PB1, PB0 is pulled down
	DDRB |= 0x01; //setting as output
	PORTB |= 0x01; //set output pin to 1 initially (turning LED off)
	

	// Turn on timer with no prescaler on the clock.  We use it for delays and to measure period.
	TCCR1B |= _BV(CS10); // Check page 110 of ATmega328P datasheet

	waitms(500); // Wait for putty to start
	printf("Period measurement using the free running counter of timer 1.\n"
	       "Connect signal to PB1 (pin 15).\n");
	
	while (1)
	{
		count=GetPeriod(100);
		if(count>0)
		{
			T=count/(F_CPU*100.0);
			f=1/T;
			printf("f=%fHz (count=%lu)     \r", f, count);
			if(f > 3100.0){ //turn LED on
			PORTB &= 0b00000010; 
			sei(); //turn on buzzer
			}
			else{
			PORTB = 0b00000011; //turn LED off
			cli(); //turn off buzzer
			}
		}
		else
		{
			printf("NO SIGNAL                     \r");
		}
		waitms(200);
	}
}