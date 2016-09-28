/*
 * gauge.h
 */

#ifndef GAUGE_H_
#define GAUGE_H_

#include "gauge_hal.h"

void gauge_init(gauge_t *gauge, I2C_HandleTypeDef *hi2c);

void gauge_start_read(gauge_t *gauge);
void gauge_format_data(gauge_t *gauge);

#endif /* GAUGE_H_ */
