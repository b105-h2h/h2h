/*
 * utils.h
 *
 *  Created on: 1/9/2015
 *      Author: slopez
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <msp430.h>
#include <stdint.h>

void 	itoa(uint16_t number, char* string);
void	delay_ms(int ms);

#endif /* UTILS_H_ */
