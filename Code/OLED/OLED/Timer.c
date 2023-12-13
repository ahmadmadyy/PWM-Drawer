#include <avr/io.h>
#include "Timer.h"

void initPWM()
{
	// Set OC0A (Pin 6, PD6) as output
	DDRD |= (1 << PD6);

	// Set Timer0 in Normal mode
	TCCR0A = 0x00;

	// Set prescaler to 8
	TCCR0B = (1 << CS02) | (1 << CS01) | (0 << CS00);

	// Enable Phase Correct PWM mode, non-inverting output on OC0A
	TCCR0A |= (1 << WGM01) | (1 << WGM00) | (0 << COM0A0) | (1 << COM0A1);

	// Initial duty cycle = 0%
	OCR0A = 0;
	
	// Enable Output Compare Interrupt 
	//TIMSK |= (1<<OCIE0);
}

void setPWMDutyCycle(uint16_t value)
{
	// Calculate the corresponding PWM value for the given duty cycle
	uint16_t pwmValue = (255.0/8192.0) * (uint16_t) value;

	// Set the OCR0A register with the calculated PWM value
	OCR0A = (uint8_t)pwmValue;
	_delay_ms(10);
}

