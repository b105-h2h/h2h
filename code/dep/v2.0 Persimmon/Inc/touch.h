/*
 * touch.h
 *
 *  Created on: 24/05/2016
 *      Author: Smau
 */
#ifndef TOUCH_TOUCH_H_
#define TOUCH_TOUCH_H_

#include "touch_hal.h"
#include "lcd_hal.h"

#include "touch_constants.h"
#include "touch_position.h"

void touch_init(touch_t *touch, SPI_HandleTypeDef *hspi,
                lcd_t *lcd,
                uint16_t touch_x_size, uint16_t touch_y_size);

void touch_read_position(touch_t *touch);

#endif /* TOUCH_TOUCH_H_ */
