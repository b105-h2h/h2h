/*
 * main.c
 *
 *  Created on: 29/9/2015
 *      Author: Smau
 */

#include <msp430.h>

#include "clocks.h"

#include "afe/afe.h"
#include "buzzer/buzzer.h"
#include "display/display.h"
#include "display/display_src/display_functions.h"
#include "touch/touch.h"
#include "filters/filters.h"

#define FS 500
//buzzer_t buzzer;

display_t display;
//touch_t touch;
ecg_data_t last_sample, last_sample_1;
int bpm,flag;

int main()
{
	volatile uint8_t flagerino = 0;
	volatile int maxerino_pos = 0,sample_counter = 0;
	volatile int32_t threshold = 0x00000FFF,current_value = 0, maxerino = 0xFFFFFFFF, prev_value = 0;

    WDTCTL = WDTPW | WDTHOLD;		//Stop watchdog timer
    bpm = 0;
    flag = 0;
    ecg_data_clear(&last_sample);
    ecg_data_clear(&last_sample_1);

    /*
     * Setups
     */
    clocks_setup();

    afe_setup();
    display_setup(&display);
//    touch_setup(&touch);
//    buzzer_setup(&buzzer);

    /*
     * MCU setup
     */

    PM5CTL0 &= ~LOCKLPM5;			//Disable the GPIO power-on default high-impedance mode

    /*
     * Initializations
     */
    afe_init();
    display_init(&display);
//    touch_initialize(&touch);

    /*
     * Sheits
     */

    display_functions_write_string(" BPM:           ", COLOR_RED,
	  	  	  	   	   	   	   	   	   	   	   	   	   	   display.display_interface.menubar_window_bg_color,
														   0x00, 0xC0);
    display_functions_write_string(" DANGER: Apechusque ", COLOR_WHITE,
														   display.display_interface.menubar_window_bg_color,
														   0x00, 0xD0);

	__bis_SR_register(GIE);			//Enable global interrupts

	while(1)
	{
		if (flag > 0)
		{
			__disable_interrupt();
			current_value = filter_sample(last_sample_1.data);

			last_sample.data = current_value;

			flag = 0;

			if(current_value > threshold)
			{
				if (current_value > maxerino)
				{
					maxerino = current_value;
					maxerino_pos = sample_counter;
				}else if (flagerino == 0){
					threshold = ((maxerino * 7) >>3);
					flagerino = 1;
				}
			}else if (prev_value > threshold)
			{
				//bpm =  ((60 * FS) / sample_counter);
				bpm =   sample_counter;
				threshold = ((threshold * 7 + maxerino) >> 3);
				sample_counter = sample_counter - maxerino_pos - 1;
				maxerino = 0xFFFFFFFF;
			}else{
				threshold = ((threshold * 127) >> 7);
			}

			sample_counter++;
			prev_value = current_value;
			__enable_interrupt();
		}
	}
}
