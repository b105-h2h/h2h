/*
 * touch_hal.h
 *
 *  Created on: 27/05/2016
 *      Author: slopez
 */

#ifndef TOUCH_HAL_H_
#define TOUCH_HAL_H_

#include <stdint.h>
#include "spi.h"
#include "gpio.h"

#include "touch_constants.h"
#include "touch_position.h"

#include "touch_calib.h"

typedef enum {T_CLICKED, T_NOT_CLICKED} touch_clicked_t;

typedef struct touch_t
{
  /* Hardware/memory map location */
  SPI_HandleTypeDef *hspi;
  
  /* Physical size */
  uint16_t touch_x_size;
  uint16_t touch_y_size;
  
  /* Calibration data */
  touch_calib_t *calib;
  
  /* Touch data */
  touch_pos_t current_pos;
  touch_clicked_t is_clicked;
}
touch_t;

void touch_hal_init(touch_t *touch,
                    SPI_HandleTypeDef *hspi);

uint32_t touch_hal_read(touch_t *touch, uint8_t cntl_byte);

#endif /* TOUCH_HAL_H_ */