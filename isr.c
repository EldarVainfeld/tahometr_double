// ISRs

#include "header.h"

ISR(ADC_vect)
{
	//Global variables used :
	// gl_set[sensor_id].ir_value : contains the value of the intensity of IR reflexion
	// gl_set[sensor_id].ain_th_high : High theshold (rizing hysteresis)
	// gl_set[sensor_id].ain_th_low : Low theshold (falling hysteresis)
	// t_capture & t_postscale : reading of the time elapsed since last detected cycle
	// gl_set[sensor_id].ir_history_b and gl_set[sensor_id].ir_history_a: hold the last two values of gl_set[sensor_id].ir_value
	// gl_set[sensor_id].last_state : used to remember which hysteresis level to use
	gl_set[sensor_id].ir_value = ADC >> 2;
	if ((gl_set[sensor_id].ir_value > gl_set[sensor_id].ain_th_high) && (gl_set[sensor_id].last_state == 0)) //a cycle is detected
	{
		gl_set[sensor_id].last_state = 1; //for next edge detection, use the falling hysteresis for next detection
		t_capture = TCNT1;
		TCNT1 = 0;
		t_postscale = post_scaller;
		post_scaller = 0;
		if (measurement_started)
		{
			measurement_started = 0;
		}
		else if (!measurement_done)
		{
			time_count = t_capture + (t_postscale * 65536);
			if (time_count)
			{
				rpm_x10 = 187500000 / time_count;
				if (1) //(rpm_x10 < 45000)
				{
					gl_set[sensor_id].rpm_his[gl_set[sensor_id].last_rpm_indx++] = rpm_x10;
					if (gl_set[sensor_id].last_rpm_indx >= MAX_NUM_MEAS)
					{
						gl_set[sensor_id].last_rpm_indx = 0;
					}
				}
			}
			measurement_done = 1;
		}
		gl_set[sensor_id].anim_counter++;
		if (gl_set[sensor_id].anim_counter > 3)
		{
			gl_set[sensor_id].anim_counter = 0;
		}
	}

	if ((gl_set[sensor_id].ir_value < gl_set[sensor_id].ain_th_low) && (gl_set[sensor_id].last_state == 1)) //a low level is detected
	{
		gl_set[sensor_id].last_state = 0; //for next edge detection, use the rising hysteresis for next detection
	}

	if (delta(gl_set[sensor_id].ir_history_a, gl_set[sensor_id].ir_value) > 10)
	{
		gl_set[sensor_id].ir_history_b = gl_set[sensor_id].ir_history_a;
		gl_set[sensor_id].ir_history_a = gl_set[sensor_id].ir_value;
		gl_set[sensor_id].ir_avg = (gl_set[sensor_id].ir_history_a + gl_set[sensor_id].ir_history_b) / 2;
		gl_set[sensor_id].ain_th_high = gl_set[sensor_id].ir_avg + 5;
		gl_set[sensor_id].ain_th_low = gl_set[sensor_id].ir_avg - 5;
	}
}

ISR(TIMER1_OVF_vect)
{
	//LED_ON;
	post_scaller++;
}
