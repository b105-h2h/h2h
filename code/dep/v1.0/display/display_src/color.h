/*
 * color.h
 *
 *  Created on: 29/9/2015
 *      Author: slopez
 */

#ifndef DISPLAY_DISPLAY_SRC_COLOR_H_
#define DISPLAY_DISPLAY_SRC_COLOR_H_

#include <msp430.h>
#include <stdint.h>

#define COLOR_RED		(color_t){.red=0xFF,.green=0x00,.blue=0x00}
#define COLOR_GREEN		(color_t){.red=0x00,.green=0xFF,.blue=0x00}
#define COLOR_BLUE		(color_t){.red=0x00,.green=0x00,.blue=0xFF}

#define COLOR_YELLOW	(color_t){.red=0xFF,.green=0xFF,.blue=0x00}
#define COLOR_CYAN		(color_t){.red=0x00,.green=0xFF,.blue=0xFF}
#define COLOR_MAGENTA	(color_t){.red=0xFF,.green=0x00,.blue=0xFF}

#define COLOR_WHITE		(color_t){.red=0xFF,.green=0xFF,.blue=0xFF}
#define COLOR_BLACK		(color_t){.red=0x00,.green=0x00,.blue=0x00}

typedef struct color_t
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
}
color_t;

void	color_set_value(color_t* color, uint8_t red, uint8_t green, uint8_t blue);

#endif /* DISPLAY_DISPLAY_SRC_COLOR_H_ */
