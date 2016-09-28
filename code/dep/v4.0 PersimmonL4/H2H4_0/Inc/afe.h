/*
 * afe.h
 *
 *  Created on: 29/9/2015
 *      Author: Smau
 */

#ifndef AFE_H_
#define AFE_H_

#include "gpio.h"
#include "spi.h"

#include "afe_hal.h"

void afe_init(afe_t *afe, SPI_HandleTypeDef *hspi);

void afe_start_read(afe_t *afe);
void afe_format_data(afe_t *afe);

void afe_calibrate(afe_t *afe);

void afe_test_signal_on(afe_t *afe);
void afe_test_signal_off(afe_t *afe);

void afe_set_gain(afe_t *afe, uint8_t channel, uint8_t gain);

#endif /* AFE_H_ */
