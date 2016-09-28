/*
 * afe_serial.h
 *
 *  Created on: 29/9/2015
 *      Author: Smau
 */

#ifndef AFE_AFE_SRC_AFE_SERIAL_H_
#define AFE_AFE_SRC_AFE_SERIAL_H_

#include <msp430.h>
#include <stdint.h>

#include "ADS1291_constants.h"

uint8_t	afe_serial_send(uint8_t data);
uint8_t	afe_serial_read(uint8_t data);

uint8_t afe_serial_read_register(uint8_t address);
void 	afe_serial_write_register(uint8_t address, uint8_t value);

#endif /* AFE_AFE_SRC_AFE_SERIAL_H_ */

