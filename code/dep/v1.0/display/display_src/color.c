/*
 * color.c
 *
 *  Created on: 29/9/2015
 *      Author: slopez
 */

#include "color.h"

void color_set_value(color_t* color, uint8_t red, uint8_t green, uint8_t blue)
{
	color->red = red;
	color->green = green;
	color->blue = blue;
}
