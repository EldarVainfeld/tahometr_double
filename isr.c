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
	gl_set[sensor_id].ir_value = ADC>>2;			
	if ((gl_set[sensor_id].ir_value > gl_set[sensor_id].ain_th_high) && (gl_set[sensor_id].last_state==0)) //a cycle is detected 
	{
		gl_set[sensor_id].last_state = 1; //for next edge detection, use the falling hysteresis for next detection
		t_capture=TCNT1;TCNT1 = 0;
		t_postscale=post_scaller; post_scaller=0;
	}

	if ((gl_set[sensor_id].ir_value < gl_set[sensor_id].ain_th_low) && (gl_set[sensor_id].last_state==1)) //a low level is detected
	{
		gl_set[sensor_id].last_state = 0; //for next edge detection, use the rising hysteresis for next detection
	}

	if (delta(gl_set[sensor_id].ir_history_a,gl_set[sensor_id].ir_value) > 10)
	{
		gl_set[sensor_id].ir_history_b = gl_set[sensor_id].ir_history_a;
		gl_set[sensor_id].ir_history_a = gl_set[sensor_id].ir_value;
		gl_set[sensor_id].ir_avg = (gl_set[sensor_id].ir_history_a+gl_set[sensor_id].ir_history_b)/2;
		gl_set[sensor_id].ain_th_high = gl_set[sensor_id].ir_avg + 5;
		gl_set[sensor_id].ain_th_low = gl_set[sensor_id].ir_avg - 5;
	}
}



ISR(TIMER1_OVF_vect)
{
	//LED_ON;
	post_scaller++;
}


