/*
 * lcd_hal.h
 *
 *  Created on: 27/05/2016
 *      Author: slopez
 */

#ifndef LCD_HAL_H_
#define LCD_HAL_H_

#include <stdint.h>
#include "fsmc.h"

#include "lcd_constants.h"
#include "lcd_position.h"

#include "color.h"

typedef struct lcd_t
{
  /* Hardware/memory map location */
  SRAM_HandleTypeDef *hsram;
  uint32_t *lcd_reg;
  uint32_t *lcd_data;

  /* Physical size */
  uint16_t lcd_x_size;
  uint16_t lcd_y_size;

  /* Config */
  uint8_t backlight_level;
}
lcd_t;

void lcd_hal_init(lcd_t *lcd,
                  SRAM_HandleTypeDef *hsram, uint32_t *lcd_reg, uint32_t *lcd_data);

void lcd_hal_write_command(lcd_t *lcd, uint16_t command);
void lcd_hal_write_data(lcd_t *lcd, uint16_t data);
void lcd_hal_read_reg(lcd_t *lcd,
                      uint16_t reg,
                      uint16_t *param_buffer, uint32_t param_buffer_size);
void lcd_hal_write_reg(lcd_t *lcd,
                       uint16_t reg,
                       uint16_t *param_buffer, uint32_t param_buffer_size);
void lcd_hal_set_drawing_address(lcd_t *lcd,
                                 uint16_t start_col, uint16_t end_col,
                                 uint16_t start_row, uint16_t end_row);

#endif /* LCD_HAL_H_ */