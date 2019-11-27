
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
void switch_sensor()
{
	if (sensor_id == 1)
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
	measurement_started = 1;
	measurement_zero = 0;
	measurement_done = 0;
}

int main()
{
	sensor_id = 0;
	long time_count;
	long rpm_sum;

	uchar i, j;
	char animation[4];
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

	for (j = 0; j < 2; j++)
	{
		gl_set[j].rpm_avg = 0;
		gl_set[j].ain_th_low = 20;
		gl_set[j].ain_th_high = 45;

		for (i = 0; i < MAX_NUM_MEAS; i++)
		{
			gl_set[j].rpm_his[i] = 0;
		}
		gl_set[j].last_rpm_indx = 0;
		gl_set[j].anim_counter = 0;
		gl_set[j].anim_counter_old = 0;
	}

	LED_OFF;
	switch_sensor();
	ADCSRA = (1 << ADEN) | (1 << ADATE) | (1 << ADIE) | (1 << ADSC) | 5;
	SENSOR_ON;
	LED_ON;
	LCD_CONTRAST = 0;

	while (1)
	{
		time_count = t_capture + (t_postscale * 65536);
		if (time_count > 312500)
		{
			gl_set[sensor_id].rpm_his[gl_set[sensor_id].last_rpm_indx++] = 0;
			measurement_done = 1;
		}
		if (measurement_done)
		{
			rpm_sum = 0;
			for (i = 0; i < MAX_NUM_MEAS; i++)
			{
				rpm_sum += gl_set[sensor_id].rpm_his[i];
			}
			gl_set[sensor_id].rpm_avg = rpm_sum / MAX_NUM_MEAS;
			switch (sensor_id)
			{
			case 0:
				lcd_send_floatx10(gl_set[0].rpm_avg, 5, 7, 1); //rpm_x10
			case 1:
				lcd_send_floatx10(gl_set[1].rpm_avg, 69, 7, 1); //rpm_x10
			}
			switch_sensor();
		}
		if (gl_set[0].anim_counter != gl_set[0].anim_counter_old)
		{
			gl_set[0].anim_counter_old = gl_set[0].anim_counter;
			locate(14);
			lcd_send_4b_mode((uchar)animation[(uchar)gl_set[0].anim_counter]);
		}
		if (gl_set[1].anim_counter != gl_set[1].anim_counter_old)
		{
			gl_set[1].anim_counter_old = gl_set[1].anim_counter;
			locate(78);
			lcd_send_4b_mode((uchar)animation[(uchar)gl_set[0].anim_counter]);
		}
	}
	return 0;
}
