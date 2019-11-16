// ISRs

#include "header.h"

ISR(ADC_vect)
{
	//Global variables used :
	// ir_value : contains the value of the intensity of IR reflexion
	// ain_th_high : High theshold (rizing hysteresis)
	// ain_th_low : Low theshold (falling hysteresis)
	// t_capture & t_postscale : reading of the time elapsed since last detected cycle
	// ir_history_b and ir_history_a: hold the last two values of ir_value 
	// last_state : used to remember which hysteresis level to use
	ir_value = ADC>>2;			
	if ((ir_value > ain_th_high) && (last_state==0)) //a cycle is detected 
	{
		//n_cap++;
		last_state = 1; //for next edge detection, use the falling hysteresis for next detection
		t_capture=TCNT1;TCNT1 = 0;
		t_postscale=post_scaller; post_scaller=0;
	}

	if ((ir_value < ain_th_low) && (last_state==1)) //a low level is detected
	{
		last_state = 0; //for next edge detection, use the rising hysteresis for next detection
	}

	if (delta(ir_history_a,ir_value) > 10)
	{
		ir_history_b = ir_history_a;
		ir_history_a = ir_value;
		ir_avg = (ir_history_a+ir_history_b)/2;
		ain_th_high = ir_avg + 5;
		ain_th_low = ir_avg - 5;
	}
}



ISR(TIMER1_OVF_vect)
{
	//LED_ON;
	post_scaller++;
}


