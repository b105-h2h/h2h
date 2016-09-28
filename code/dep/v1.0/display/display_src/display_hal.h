/*
 * display_hal.h
 *
 *  Created on: 29/9/2015
 *      Author: slopez
 */

#ifndef DISPLAY_DISPLAY_SRC_DISPLAY_HAL_H_
#define DISPLAY_DISPLAY_SRC_DISPLAY_HAL_H_

#include <msp430.h>
#include <stdint.h>

#include "color.h"

void		display_hal_input();
void		display_hal_output();

void		display_hal_screen_on();
void		display_hal_screen_off();

uint16_t	display_hal_read_reg(uint8_t reg);
void		display_hal_write_reg(uint8_t reg, uint8_t data_hi, uint8_t data_lo);
void		display_hal_write_GRAM(color_t color);
void		display_hal_set_GRAM_position(uint16_t x, uint16_t y);

#endif /* DISPLAY_DISPLAY_SRC_DISPLAY_HAL_H_ */
