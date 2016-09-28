/*
 * item.h
 */

#ifndef ITEM_H_
#define ITEM_H_

#include "item_gui.h"

#include "item_area.h"
#include "item_graph.h"
#include "item_lcd_config.h"

#include "lcd_hal.h"

typedef union item_t
{
  item_area_t area;
  item_graph_t graph;
  item_lcd_config_t config;
}
item_t;

typedef gui_status_t (*lcd_print_function_f)(lcd_t *lcd, item_t *item);
typedef struct item_action_t
{
  lcd_print_function_f item_print_function;
  item_t item;
}
item_action_t;

#endif /* ITEMS_H_ */