/*
 * lcd_primitives.h
 *
 *  Created on: 26/05/2016
 *      Author: slopez
 */

#ifndef LCD_PRIMITIVES_H_
#define LCD_PRIMITIVES_H_

#include <stdlib.h>

#include "lcd_hal.h"

#define DMA_MAX_CHUNK_SIZE 50000

void lcd_draw_pixel(lcd_t *lcd,
                    uint16_t x_pos, uint16_t y_pos,
                    color_t *color);
void lcd_draw_line(lcd_t *lcd,
                   uint16_t origin_x, uint16_t origin_y,
                   uint16_t end_x, uint16_t end_y,
                   color_t *color);
void lcd_draw_rectangle(lcd_t *lcd_t,
                        uint16_t x_pos, uint16_t width,
                        uint16_t y_pos, uint16_t height,
                        color_t *color);
void lcd_draw_string(lcd_t *lcd,
                     wchar_t *string,
                     const uint8_t *current_font,
                     color_t *char_color, color_t *bg_color,
                     uint16_t x_pos, uint16_t y_pos);
void lcd_draw_background(lcd_t *lcd, color_t *bg_color);

#endif /* LCD_PRIMITIVES_H_ */