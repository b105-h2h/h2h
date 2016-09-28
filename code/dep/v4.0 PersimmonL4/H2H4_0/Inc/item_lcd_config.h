/*
 * item_lcd_config.h
 *
 *  Created on: 27/05/2016
 *      Author: slopez
 */

#ifndef ITEM_LCD_CONFIG_H_
#define ITEM_LCD_CONFIG_H_

#include <stdint.h>

typedef struct item_lcd_config_t
{
  uint8_t backlight_level;
}
item_lcd_config_t;

void item_lcd_config_init(item_lcd_config_t *config, uint8_t bl_level);

#endif /* ITEM_LCD_CONFIG_H_ */