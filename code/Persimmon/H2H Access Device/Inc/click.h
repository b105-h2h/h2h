/*
 * click.h
 *
 *  Created on: 24/05/2016
 *      Author: Smau
 */
#ifndef CLICK_H_
#define CLICK_H_

#include <stdint.h>

#include "lcd_position.h"

typedef enum {CLICK_DOWN, CLICK_HOLD, CLICK_UP} click_status_t;

typedef struct click_t
{
  lcd_pos_t pos;
  click_status_t click_type;
}
click_t;

#endif /* CLICK_H_ */
