
// IKA TACH SOURCE CODE
// V0.1 BY IBRAHIM KAMAL
// (c) WWW.IKALOGIC.COM
// Protected by Creative Commons License.
//Updated to accept two sensors

#include "header.h"

char PROGMEM line1[] = "1RPM:            ";
char PROGMEM line2[] = "2RPM:            ";

uchar delta(uchar v1, uchar v2)
{
	if (v1 > v2)
	{
		return (v1 - v2);
	}
	else
	{
		return (v2 - v1);
	}
}

int main()
{
	sensor_id = 0;
	long time_count;
	long rpm_x10, last_rpm_x10[2] = {0, 0};

	uchar i;
	char animation[4], anim_counter[2] = {0, 0};
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
	format(line1, line2);

	gl_set[0].rpm_avg = 0;
	gl_set[0].ain_th_low = 20;
	gl_set[0].ain_th_high = 45;

	gl_set[1].rpm_avg = 0;
	gl_set[1].ain_th_low = 20;
	gl_set[1].ain_th_high = 45;

	LED_OFF;
	ADMUX = 2 + sensor_id;
	ADCSRA = (1 << ADEN) | (1 << ADATE) | (1 << ADIE) | (1 << ADSC) | 5;
	SENSOR_ON;
	LED_ON;
	LCD_CONTRAST = 0;

	long int time_with_current_sensor_id = 0;
	long int time_count_prev = 0;

	while (1)
	{
		time_count = t_capture + (t_postscale * 65536);
		rpm_x10 = 187500000 / time_count;
		if (last_rpm_x10[sensor_id] == 0)
		{
			for (i = 0; i < MAX_NUM_MEAS; i++)
			{
				gl_set[sensor_id].rpm_his[i] = 0;
			}
		}
		if (rpm_x10 > 40000)
			rpm_x10 = last_rpm_x10[sensor_id];
		if (last_rpm_x10[sensor_id] != rpm_x10)
		{
			last_rpm_x10[sensor_id] = rpm_x10;

			if(sensor_id == 0) {locate(14);} else {locate(78);}
			lcd_send_4b_mode((uchar)animation[(uchar)anim_counter[sensor_id]]);
			anim_counter[sensor_id]++;
			if (anim_counter[sensor_id] > 3)
				anim_counter[sensor_id] = 0;
			if (gl_set[sensor_id].n_measures < MAX_NUM_MEAS)
				gl_set[sensor_id].n_measures++;
			for (i = 0; i < MAX_NUM_MEAS - 1; i++)
			{
				gl_set[sensor_id].rpm_his[i] = gl_set[sensor_id].rpm_his[i + 1];
			}
			gl_set[sensor_id].rpm_his[MAX_NUM_MEAS - 1] = rpm_x10;

			gl_set[sensor_id].rpm_avg = 0;
			for (i = 0; i < MAX_NUM_MEAS; i++)
			{
				gl_set[sensor_id].rpm_avg += gl_set[sensor_id].rpm_his[i];
			}
			gl_set[sensor_id].rpm_avg = gl_set[sensor_id].rpm_avg / gl_set[sensor_id].n_measures;
		}

		if(time_count >= time_count_prev)
		{
			time_with_current_sensor_id += time_count - time_count_prev;
			time_count_prev = time_count;
		}
		else
		{
			time_with_current_sensor_id += time_count;
		}
		
		if(time_with_current_sensor_id > 20000000)
		{
			time_with_current_sensor_id = 0;
			if(sensor_id==1)
			{
				sensor_id = 0;
				locate(15);
				lcd_send_4b_mode((uchar)'1');
				locate(79);
				lcd_send_4b_mode((uchar)'_');
				ADMUX = 2;
			} 
			else
			{
				sensor_id = 1;
				locate(15);
				lcd_send_4b_mode((uchar)'_');
				locate(79);
				lcd_send_4b_mode((uchar)'2');
				ADMUX = 3;
			}
		}

		refresh_counter++;

		if (refresh_counter > 20000)
		{
			refresh_counter = 0;
			lcd_send_floatx10(gl_set[0].rpm_avg, 5, 7, 1); //rpm_x10
			lcd_send_floatx10(gl_set[1].rpm_avg, 69, 7, 1); //rpm_x10
		}
	}
	return 0;
}
