/*
 * touch.h
 *
 *  Created on: 24/05/2016
 *      Author: Smau
 */
#ifndef TOUCH_TOUCH_H_
#define TOUCH_TOUCH_H_

#include "touch_hal.h"
#include "touch_calib.h"

#include "touch_constants.h"

#include "touch_position.h"
#include "lcd_position.h"

void touch_init(touch_t *touch, SPI_HandleTypeDef *hspi,
                uint16_t touch_x_size, uint16_t touch_y_size);

void      touch_read_position(touch_t *touch);
lcd_pos_t touch_get_lcd_pos(touch_t *touch);

#endif /* TOUCH_TOUCH_H_ */
