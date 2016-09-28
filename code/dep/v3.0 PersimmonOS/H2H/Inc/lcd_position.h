/*
 * lcd_position.h
 *
 *  Created on: 24/05/2016
 *      Author: Smau
 */
#ifndef LCD_POSITION_H_
#define LCD_POSITION_H_

#include <stdint.h>

typedef struct lcd_pos_t
{
  uint32_t x_pos;
  uint32_t y_pos;
}
lcd_pos_t;

#endif /* LCD_POSITION_H_ */