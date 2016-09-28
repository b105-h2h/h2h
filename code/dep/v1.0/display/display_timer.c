/*
 * display_timer.c
 *
 *  Created on: 29/9/2015
 *      Author: Smau
 */

#include "display_timer.h"

extern display_t display;
extern ecg_data_t last_sample;
extern int bpm;

// Timer A2 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER2_A0_VECTOR
__interrupt void Timer2_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER2_A0_VECTOR))) Timer2_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
	static ecg_data_t signal_data_point;					//Temporary storage variable
	static const color_t signal_color = COLOR_GREEN;
	static uint8_t count = 0;
	static char numberino[5];
	uint16_t gie = __get_SR_register() & GIE; //Store current GIE state
	/*
	 * Paint ECG value every display tick
	 */
	__disable_interrupt();                    //Make this operation atomic


	display_interface_write_signal(&display.display_interface, &last_sample, signal_color);			//Write it
	if (++count > 20)
	{
		count = 0;
		itoa(bpm,numberino);
	    display_functions_write_string(numberino, COLOR_RED,
									   display.display_interface.menubar_window_bg_color, 0x60, 0xC0);
	}

	__bis_SR_register_on_exit(gie);                   //Restore original GIE state
}

/*
 * Sweep time is limited between 500 ms and 10 sg
 */
static void display_set_sweep_time(int disp_sweep_time_ms)
{
	//Interrupt period -> Calculated from display sweep time and horizontal display resolution
	uint16_t timer_compare_value;

	switch (disp_sweep_time_ms)
	{
		case SWEEP_TIME_10000:
			timer_compare_value = 62500;
			break;
		case SWEEP_TIME_5000:
			timer_compare_value = 31250;
			break;
		case SWEEP_TIME_2000:
			timer_compare_value = 12500;
			break;
		case SWEEP_TIME_1000:
			timer_compare_value = 6250;
			break;
		case SWEEP_TIME_500:
			timer_compare_value = 3125;
			break;
		case SWEEP_TIME_200:
			timer_compare_value = 1250;
			break;
		default:
			break;
	}

	//TODO
	//Make enum type
	//TODO
	//These time values are too fast for our screen, the only one
	//that works now is 10 sg. There are two solutions: using larger
	//sweep times or trying to improve the performance of the function
	//display_interface_write_signal @ display_interface.c, focusing
	//in the most time consuming task which turns out to be the screen
	//clearing operation (TESTED on 29/09/2015)

	TA2CCR0 = timer_compare_value;						//2000 equals to 1 ms @ 2 MHz
}

void display_timer_setup()
{
	//Timer A2 register configuration
		TA2CTL = TASSEL__SMCLK | MC__STOP | TACLR;		//Clock source = SMCLK
														//Mode = Stop mode
														//Reset timer A3
		TA2CCTL0 &= ~CCIFG;								//Clear interrupt flag

		display_set_sweep_time(SWEEP_TIME_10000);		//Sweep time
}

void display_timer_start()
{
	TA2CCTL0 |= CCIE;									//Enable compare interrupt
	TA2CTL = TASSEL__SMCLK | MC__UP | TACLR;			//Start timer
}
