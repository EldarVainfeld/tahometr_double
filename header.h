
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

#define AIN0	OCR2A

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

//globals
long rpm_his[31];
unsigned int ambient_value,t_capture,t_postscale,time_a,time_b,refresh_counter;
uchar timer_1_cps,post_scaller,pos_scale_a,pos_scale_b,n_cap,ain_th_low,ain_th_high,ir_value,last_ir_value,last_state;
uchar ir_history_a,ir_history_b;
uchar ir_avg,t,n_measures;
uchar v_bg;
