// INI

#include "header.h"

void setup_timers()
{
	//setup timer 0 for contast pwm
	TCCR0A = (1<<COM1B1)|(1<<WGM01)|(1<<WGM00);
	TCCR0B = (1<<CS00); 
	OCR0B = 25; 
	TIMSK0 = (0<<OCIE0A);

	//Setup timer1 for RPM counting
	TCCR1A = 0;
	TCCR1B = 3;
	TIMSK1 = (1<<TOIE1);


}


void setup_io()
{
	DIDR0 = (1<<ADC2D) | (1<<ADC3D); //disable digital input on ADC2 and ADC3
	DDRB &= ~(1<<PORTB1); //set push button as input
	PORTB |= (1<<PORTB1); //activte the pull up resistor for the push button
	
	LCD_EN_PORT |= _BV(LCD_EN);
  LCD_RS_PORT &= ~(_BV(LCD_RS));	
}
















