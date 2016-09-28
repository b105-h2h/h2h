/*
 * ecg_data_circular_buffer.c
 *
 *  Created on: 15/12/2015
 *      Author: tvalno
 */
#ifndef FILTERS_H_
#define FILTERS_H_

#include <msp430.h>
#include <stdint.h>

#include "../utils.h"

int32_t filter_sample(int32_t value);

#endif /* FILTERS_H_ */


