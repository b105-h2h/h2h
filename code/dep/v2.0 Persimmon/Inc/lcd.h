/*
 * lcd.h
 *
 *  Created on: 10/05/2016
 *      Author: Smau
 */

#ifndef LCD_H_
#define LCD_H_

#include "lcd_primitives.h"

void lcd_init(lcd_t *lcd, SRAM_HandleTypeDef *hsram,
              uint32_t *lcd_reg, uint32_t *lcd_data,
              uint16_t lcd_x_size, uint16_t lcd_y_size);

gui_status_t lcd_print_area(lcd_t *lcd, area_t *area);
void lcd_set_brightness(lcd_t *lcd, uint8_t level);

#endif /* LCD_H_ */