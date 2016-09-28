/*
 *  touch_calib.h
 *
 *  Created on: 27/05/2016
 *      Author: slopez
 */

#ifndef TOUCH_CALIB_H_
#define TOUCH_CALIB_H_

#include <stdint.h>

#include "touch_position.h"
#include "lcd_position.h"

typedef struct touch_calib_t
{
  /* Calibration coefficients
   *
   * Q20.12 fixed point format
   */
  int32_t alpha_x;
  int32_t beta_x;
  int32_t delta_x;

  int32_t alpha_y;
  int32_t beta_y;
  int32_t delta_y;
}
touch_calib_t;

void touch_calib_3_point_calib(touch_calib_t *calib,
                               lcd_pos_t *l1, lcd_pos_t *l2, lcd_pos_t *l3,
                               touch_pos_t *t1, touch_pos_t *t2, touch_pos_t *t3);

#endif /* TOUCH_CALIB_H_ */