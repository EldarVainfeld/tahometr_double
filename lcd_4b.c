
#include "header.h"

#define V_CONTRAST_LOW	2100
//#define V_CONTRAST_LOW	2050
#define V_CONTRAST_HIGH	3750

char PROGMEM  lcd_ini[8]  = {0x03,0x03,0x03,0x02,0x28,0x0F,0x0C,0x06}; 



void lcd_send_4b_mode(unsigned char data){
	
  LCD_DDR |= (LCD_MASK); //ensure data pins are set as outputs (in case it is a shared bus)
	
	LCD_EN_PORT |= _BV(LCD_EN);
	_delay_ms (1);
	
	LCD_PORT &= ~(LCD_MASK); //erase previous data
	LCD_PORT |=  ((data>>4)<<LCD_SHIFT)&LCD_MASK;

	LCD_EN_PORT &= ~(_BV(LCD_EN));
	//_delay_ms (1);

	LCD_EN_PORT |= _BV(LCD_EN);
	_delay_ms (1);
	
	LCD_PORT &= ~(LCD_MASK); //erase previous data
	LCD_PORT |=  ((data&LCD_MASK)<<LCD_SHIFT);


	LCD_EN_PORT &= ~(_BV(LCD_EN));
	_delay_ms (1);
} 

void lcd_ini_sequence()
{
		
		_delay_ms(250); //wait for LCD to start up
		ini_lcd_4_bit_mode();
		cls();
		_delay_ms(10); //wait for LCD to start up
		ini_lcd_4_bit_mode();
		cls();
}
void cls(){     // clear the display
   LCD_RS_PORT &= ~(_BV(LCD_RS));
   lcd_send_4b_mode(0x01);   
   LCD_RS_PORT |= _BV(LCD_RS);
}

void clr_line(uchar start_add){     // clear line
	uchar i;
	locate(start_add);
	for (i=0; i<16; i++)
	{
		lcd_send_4b_mode(' ');  
	}
}

void put_str(uchar pos, char* str){     
	uchar i;
	locate(pos);
	for (i = 0; i< 16; i++){				
		lcd_send_4b_mode(pgm_read_byte(str + i));
	}
}


void ini_lcd_4_bit_mode(){
   unsigned char i;
	 
   
   for (i = 0; i< 8; i++){
	 	//test = pgm_read_byte(lcd_ini + i);
    lcd_send_4b_mode(pgm_read_byte(lcd_ini + i));
   }
   LCD_RS_PORT |= _BV(LCD_RS);
}

void locate(unsigned char pos){
   pos = pos | (1 << 7);
   LCD_RS_PORT &= ~(_BV(LCD_RS));
   lcd_send_4b_mode(pos);   
   LCD_RS_PORT |= _BV(LCD_RS);
}

void lcd_send_floatx10(long data,uchar location, uchar len, uchar d_len)
{
	//d_len = number of digits after the decimal point
	char buffer[16];
	
	uchar i,y,temp,int_len;
	t=0;

	if (data < 0)
	{
		buffer[0] = 'N';
	}
	else
	{
		ltoa((long)(data), buffer, 10);
	}

	locate(location);

	i=0;
	if (d_len == 2)
	{
		temp = 100;
	}else{
		temp = 10;	
	}

	if( data >= temp){
		if (strlen(buffer)>len)
		{
			int_len = len;
		}
		else
		{
			int_len = strlen(buffer);
		}

		for(i=0; i<(int_len-d_len);i++)
		{
			lcd_send_4b_mode(buffer[i]);
			t++;
		}
		lcd_send_4b_mode('.');
		t++;
		for(y=0; y<(d_len);y++)
		{
			lcd_send_4b_mode(buffer[i]);
			i++;	
			t++;
		}
		
	}else{
		lcd_send_4b_mode('0');
		t++;
		lcd_send_4b_mode('.');
		t++;
		for(y=0; y<d_len;y++)
		{
			if(y < strlen(buffer))
			{
				lcd_send_4b_mode(buffer[y]);	
				i++;
				t++;
			}
		}
	}

	//fill remaining digits with spaaces
	while (t<(len))//was 4
	{
		lcd_send_4b_mode(' ');
		t++;
	}
}








/*void lcd_power(uchar on)
{     
   LCD_RS_PORT &= ~(_BV(LCD_RS));
   lcd_send_4b_mode((1<<3)|(on<<2));   
   LCD_RS_PORT |= _BV(LCD_RS);
}*/

void lcd_contrast()
{


	
}

void format(char* line1,char* line2){     // reformat the display
    unsigned char i;
		locate(0);
		for (i = 0; i< 16; i++){				
			lcd_send_4b_mode(pgm_read_byte(line1 + i));
		}
		locate(64);
		for (i = 0; i< 16; i++){
			lcd_send_4b_mode(pgm_read_byte(line2 + i));
		}
}


/*void xformat(unsigned int line1,unsigned int  line2){     // reformat the display from string sotred in xeeprom
    unsigned char i;
		locate(0);
		for (i = 0; i< 16; i++){				
			lcd_send_4b_mode(xeeprom_read_byte(line1 + i));
		}
		locate(64);
		for (i = 0; i< 16; i++){
			lcd_send_4b_mode(xeeprom_read_byte(line2 + i));
		}
}*/


