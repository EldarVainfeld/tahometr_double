
#define LCD_PORT	PORTD
#define LCD_DDR		DDRD
#define LCD_SHIFT		0
#define LCD_MASK	((1<<0)|(1<<1)|(1<<2)|(1<<3))

#define LCD_EN_PORT		PORTC
#define LCD_EN_DDR		DDRC
#define LCD_EN				(PORTC5)

#define LCD_RS_PORT		PORTD
#define LCD_RS_DDR		DDRD
#define LCD_RS				(PORTD4)

#define BL_PORT		PORTC
#define BL_DDR		DDRC
#define BL				PORTC4

#define LCD_BL_OFF BL_PORT &= ~(1<<BL);
#define LCD_BL_ON  BL_DDR |= (1<<BL);BL_PORT |= (1<<BL);

#define LCD_CONTRAST_ON  DDRD |= (1<<5);//TCCR0A = (1<<COM1B1)|(1<<WGM01)|(1<<WGM00);
#define LCD_CONTRAST	OCR0B

#define LED_PORT		PORTD
#define LED_DDR			DDRD
#define LED					PORTD7

#define LED_ON			DDRD |= (1<<LED); LED_PORT |= (1<<LED);
#define LED_OFF			LED_PORT &= (~(1<<LED));

#define SENSOR_ON 		DDRC |= (1<<PORTC1); PORTC |= (1<<PORTC1);
#define SENSOR_OFF 	PORTC &= (~(1<<PORTC1));

//ADC
#define ADC_CI ADCSRA |= (1<<ADIF);
#define ADC_START(channel)	ADMUX = channel; ADCSRA = (1<<ADEN)|(1<<ADSC)|2;
#define ADC_STOP	ADCSRA = 0;

//button interface

#define BUTTON_PRESSED (bit_is_clear(PINB,1))

#define uchar unsigned char

#include <avr/io.h>
#include <avr/interrupt.h>  
#include <util/delay.h>     
#include <avr/eeprom.h>
#include <string.h>
#include <stdlib.h>
#include <avr/pgmspace.h>   
#include <string.h>
#include <stdint.h> 
#include <avr/boot.h> 
#include <avr/wdt.h> 
#include <avr/sleep.h>
#include <avr/power.h>


void setup_timers();
void ini_lcd_4_bit_mode();
void lcd_send_4b_mode(unsigned char data);
void cls();
void lcd_contrast();
void lcd_large_str(char* str_ptr);
void lcd_send_floatx10(long data,uchar location, uchar len, uchar d_len);
void format(char* line1,char* line2);
void locate(unsigned char pos);
void reset();
void setup_io();
void setup_analog_comparator();
void refresh_ddisplay();
void lcd_ini_sequence();
void clr_line(uchar start_add);
uchar delta(uchar v1,uchar v2);
void put_str(uchar pos, char* str);
void switch_sensor();

#define MAX_NUM_MEAS (uchar)10

struct global_settings{
	// ir_value : contains the value of the intensity of IR reflexion
	// ain_th_high : High theshold (rizing hysteresis)
	// ain_th_low : Low theshold (falling hysteresis)
	// t_capture & t_postscale : reading of the time elapsed since last detected cycle
	// ir_history_b and ir_history_a: hold the last two values of ir_value 
	// last_state : used to remember which hysteresis level to use
    long rpm_avg;
    long rpm_his[MAX_NUM_MEAS + 1];
	uchar last_rpm_indx;
    uchar ain_th_low,ain_th_high,ir_value,last_state;
    uchar ir_history_a,ir_history_b;
    uchar ir_avg;
	char anim_counter, anim_counter_old;
};

//globals
struct global_settings gl_set[2]; // we are going to have two sets of variables for every sensor
uchar sensor_id; //the id-number of the current sensor
uchar measurement_started, measurement_done;
long rpm_x10, time_count;
unsigned int refresh_counter;
unsigned int t_capture,t_postscale;
uchar post_scaller;
uchar t; //used for lcd
