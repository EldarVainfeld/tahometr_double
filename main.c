
// IKA TACH SOURCE CODE
// V0.1 BY IBRAHIM KAMAL
// (c) WWW.IKALOGIC.COM
// Protected by Creative Commons License.


#include "header.h"

char PROGMEM  line1[]=	  "1RPM:            ";	
char PROGMEM  line2[]=	  "2RPM:            ";


uchar delta(uchar v1,uchar v2)
{
	if (v1>v2)
	{
		return (v1-v2);
	}
	else
	{
		return (v2-v1);
	}
}


int main()
{
	long time_count;
	long rpm_x10,last_rpm_x10=0,rpm_avg=0;
	float contrast_calculator;
	uchar i;
	char animation[4],anim_counter=0;
	animation[0] = 0xD9;
	animation[1] = 0xC9;
	animation[2] = 0xDA;
	animation[3] = 0xC8;



	setup_timers();

	setup_io();

	LCD_EN_DDR |= _BV(LCD_EN);
	LCD_RS_DDR |= _BV(LCD_RS);
	lcd_ini_sequence();
	LED_ON
	LCD_BL_ON
	LCD_CONTRAST_ON
	sei();
	format(line1,line2);

	ain_th_low = 20;
	ain_th_high = 45;
	LED_OFF;
	ADMUX = 2; ADCSRA = (1<<ADEN)|(1<<ADATE)|(1<<ADIE)|(1<<ADSC)|5;

	LCD_CONTRAST = 0;
	while(1)
	{

		if (BUTTON_PRESSED)
		{
			ADMUX = 2; 
			SENSOR_ON;
			LED_ON;
			
			time_count = t_capture + (t_postscale*65536);
			rpm_x10 = 187500000/time_count;
			if (last_rpm_x10 == 0)
			{
				for (i=0;i<30;i++)
				{
					rpm_his[i] = 0;
				}
			}
			if (rpm_x10 > 40000)rpm_x10 = last_rpm_x10;
			if (last_rpm_x10 != rpm_x10)
			{
				last_rpm_x10 = rpm_x10;
				
				locate(77);
				lcd_send_4b_mode((uchar)animation[(uchar)anim_counter]);
				anim_counter++;
				if (anim_counter > 3)anim_counter = 0;
				if (n_measures < 30) n_measures++;
				for (i=0;i<29;i++)
				{
					rpm_his[i] = rpm_his[i+1];
				}
				rpm_his[29] = rpm_x10;
			
				rpm_avg = 0;
				for (i=0;i<30;i++)
				{
					rpm_avg += rpm_his[i];
				}
				rpm_avg = rpm_avg / n_measures;
			}
			

			refresh_counter++;

			if (refresh_counter > 20000)
			{
				refresh_counter=0;
				lcd_send_floatx10(rpm_avg ,68, 7, 1);//rpm_x10
			}
		}
		else
		{
			last_rpm_x10 = 0;
			refresh_counter = 0;
			LED_OFF;
			SENSOR_OFF;
/*
			//update the contrast level according to battery level
			ADMUX = 0xE;
			v_bg = ir_value;
			if (v_bg > 0)
			{
				LCD_CONTRAST = ((13197)/(v_bg) - 188); //equation to convert V_bg to LCD_contrast level.
			}
*/	
		}		
	}
	return 0;
}
