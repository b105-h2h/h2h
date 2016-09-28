/*
 * color.h
 *
 *  Created on: 29/9/2015
 *      Author: slopez
 */

#ifndef COLOR_H_
#define COLOR_H_

#include <stdint.h>

#define COLOR_RED       {.red=0xFF,.green=0x00,.blue=0x00,.color565=0xF800}
#define COLOR_GREEN     {.red=0x00,.green=0xFF,.blue=0x00,.color565=0x07E0}
#define COLOR_BLUE      {.red=0x00,.green=0x00,.blue=0xFF,.color565=0x001F}

#define COLOR_YELLOW    {.red=0xFF,.green=0xFF,.blue=0x00,.color565=0xFFE0}
#define COLOR_CYAN      {.red=0x00,.green=0xFF,.blue=0xFF,.color565=0x07FF}
#define COLOR_MAGENTA   {.red=0xFF,.green=0x00,.blue=0xFF,.color565=0xF81F}

#define COLOR_WHITE     {.red=0xFF,.green=0xFF,.blue=0xFF,.color565=0xFFFF}
#define COLOR_BLACK     {.red=0x00,.green=0x00,.blue=0x00,.color565=0x0000}

typedef struct color_t
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
        uint16_t color565;
}
color_t;

void color_set_value(color_t *color, uint8_t red, uint8_t green, uint8_t blue);
void color_get_inverse(color_t *color, color_t *inv_color);

#endif /* COLOR_H_ */