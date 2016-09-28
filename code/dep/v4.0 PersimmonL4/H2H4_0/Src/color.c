/*
 * color.c
 *
 *  Created on: 29/9/2015
 *      Author: slopez
 */

#include "color.h"

uint16_t color_get_rgb565(uint8_t red, uint8_t green, uint8_t blue)
{
  uint16_t color565;
  color565 = (((uint16_t) red)   << 8 & 0xF800) |
             (((uint16_t) green) << 3 & 0x07E0) |
             (((uint16_t) blue)  >> 3 & 0x001F);
  
  return color565;
}

void color_set_value(color_t* color, uint8_t red, uint8_t green, uint8_t blue)
{
  color->red = red;
  color->green = green;
  color->blue = blue;

  color->color565 = color_get_rgb565(red, green, blue);
}

void color_get_inverse(color_t *color, color_t *inv_color)
{
  inv_color->red   = 255 - color->red;
  inv_color->green = 255 - color->green;
  inv_color->blue  = 255 - color->blue;
  
  inv_color->color565 = color_get_rgb565(inv_color->red,
                                         inv_color->green,
                                         inv_color->blue);
}