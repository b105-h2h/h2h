/*
 * lcd_items.h
 *
 *  Created on: 27/05/2016
 *      Author: slopez
 */

#ifndef LCD_ITEMS_H_
#define LCD_ITEMS_H_

#include "lcd_items_utils.h"

#include "lcd_area.h"
#include "lcd_graph.h"
#include "lcd_config.h"

#include "lcd_hal.h"

typedef gui_status_t (*lcd_print_function_t)(lcd_t *lcd, void *item);

typedef struct lcd_item_t
{
  lcd_print_function_t item_print_function;
  void *item;
}
lcd_item_t;

void lcd_item_set(lcd_item_t *lcd_item, void *item, lcd_print_function_t function);

#endif /* ITEMS_H_ */