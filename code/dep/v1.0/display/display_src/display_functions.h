/*
 * display_functions.h
 *
 *  Created on: 29/9/2015
 *      Author: slopez
 */

#ifndef DISPLAY_DISPLAY_SRC_DISPLAY_FUNCTIONS_H_
#define DISPLAY_DISPLAY_SRC_DISPLAY_FUNCTIONS_H_

#include <msp430.h>
#include <stdint.h>

#include "display_hal.h"
#include "color.h"
#include "font.h"

#include "../../utils.h"

void	display_functions_write_pixel(color_t color, uint16_t x, uint16_t y);
void 	display_functions_write_line(color_t color, uint16_t origin_x, uint16_t origin_y,
									 	 	 	 	uint16_t end_x, uint16_t end_y);

void	display_functions_write_char(char character, color_t color, color_t bg_color,
													 uint16_t x, uint16_t y);
void	display_functions_write_string(char* string, color_t color, color_t bg_color,
													 uint16_t x, uint16_t y);

void	display_functions_sleep();
void	display_functions_wake_up();

#endif /* DISPLAY_DISPLAY_SRC_DISPLAY_FUNCTIONS_H_ */
