#include "lcd_items.h"

void lcd_item_set(lcd_item_t *lcd_item, void *item, lcd_print_function_t function)
{
  lcd_item->item = item;
  lcd_item->item_print_function = function;
}