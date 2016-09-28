/*
 * touch_coordinate.h
 *
 *  Created on: 29/9/2015
 *      Author: Smau
 */

#ifndef TOUCH_TOUCH_SRC_TOUCH_COORDINATE_H_
#define TOUCH_TOUCH_SRC_TOUCH_COORDINATE_H_

#include <msp430.h>
#include <stdint.h>

#include "../../utils.h"

#define DISPLAY_LINES	240
#define DISPLAY_COLUMNS	320

typedef struct touch_coordinate_t
{
	uint16_t xPos;
	uint16_t yPos;
}
touch_coordinate_t;

void	touch_coordinate_set(touch_coordinate_t* tCoord, uint16_t touch_xValue, uint16_t touch_yValue);

#endif /* TOUCH_TOUCH_SRC_TOUCH_COORDINATE_H_ */
