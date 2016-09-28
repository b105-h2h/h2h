/*
 * lcd.h
 */

#ifndef LCD_H_
#define LCD_H_

#include "lcd_primitives.h"
#include "item.h"

void lcd_init(lcd_t *lcd, SRAM_HandleTypeDef *hsram,
              uint32_t *lcd_reg, uint32_t *lcd_data,
              uint16_t lcd_x_size, uint16_t lcd_y_size);

gui_status_t lcd_print_area(lcd_t *lcd, item_t *item);
gui_status_t lcd_print_graph(lcd_t *lcd, item_t *item);
gui_status_t lcd_update_graph(lcd_t *lcd, item_t *item);
gui_status_t lcd_set_config(lcd_t *lcd, item_t *item);

#endif /* LCD_H_ */