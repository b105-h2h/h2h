/*
 * lcd_config.h
 *
 *  Created on: 27/05/2016
 *      Author: slopez
 */

#ifndef LCD_CONFIG_H_
#define LCD_CONFIG_H_

#include <stdint.h>

typedef struct lcd_config_t
{
  uint8_t backlight_level;
}
lcd_config_t;

void lcd_config_init(lcd_config_t *config, uint8_t bl_level);

#endif /* LCD_CONFIG_H_ */