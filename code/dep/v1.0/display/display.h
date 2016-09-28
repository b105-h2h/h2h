/*
 * display.h
 *
 *  Created on: 29/9/2015
 *      Author: slopez
 */

#ifndef DISPLAY_DISPLAY_H_
#define DISPLAY_DISPLAY_H_

#include <msp430.h>
#include <stdint.h>

#include "display_src/display_hal.h"
#include "display_src/display_interface.h"

#include "display_timer.h"

typedef struct display_t
{
	//Display interface
		display_interface_t display_interface;
}
display_t;

void	display_setup(display_t* display);
void	display_init(display_t* display);


void	display_write_pixel(display_t* display, color_t color, uint16_t x, uint16_t y);
void	display_write_char(display_t* display, char character, color_t color, color_t bg_color,
															   uint16_t x, uint16_t y);
void	display_write_string(display_t* display, char* string, color_t color, color_t bg_color,
													 	 	   uint16_t x, uint16_t y);
void	display_write_signal(display_t* display, ecg_data_t* signal_data, color_t color);


void	display_sleep(display_t* display);
void	display_wake_up(display_t* display);

void	display_screen_on(display_t* display);
void	display_screen_off(display_t* display);

#endif /* DISPLAY_DISPLAY_H_ */
