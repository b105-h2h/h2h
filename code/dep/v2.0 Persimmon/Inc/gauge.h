/*
 * gauge.h
 *
 *  Created on: 10/05/2016
 *      Author: Smau
 */
#include "gauge_constants.h"
#include "i2c.h"

#include <stdint.h>

#ifndef GAUGE_GAUGE_H_
#define GAUGE_GAUGE_H_

void fg_init();

uint8_t fg_read_reg8(uint8_t reg);
uint16_t fg_read_reg16(uint8_t reg);

void fg_write_reg8(uint8_t reg, uint8_t param);
void fg_write_reg16(uint8_t reg, uint16_t param);

#endif /* GAUGE_GAUGE_H_ */
