/*
 * touch_position.h
 *
 *  Created on: 24/05/2016
 *      Author: Smau
 */
#ifndef TOUCH_POSITION_H_
#define TOUCH_POSITION_H_

#include <stdint.h>

typedef struct touch_pos_t
{
  uint32_t x_pos;
  uint32_t y_pos;
  uint32_t pressure;
}
touch_pos_t;

#endif /* TOUCH_POSITION_H_ */
