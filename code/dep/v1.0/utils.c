/*
 * utils.c
 *
 *  Created on: 1/9/2015
 *      Author: slopez
 */

#include "utils.h"

void itoa(uint16_t number, char* string)
{
	uint16_t units, tens, hundreds, thousands;

	units = number % 10;
	tens = number % 100 - units;
	hundreds = number % 1000 - units - tens;
	thousands = number % 10000 - units - tens - hundreds;

	string[0] = thousands/1000 + '0';
	string[1] = hundreds/100 + '0';
	string[2] = tens/10 + '0';
	string[3] = units + '0';
	string[4] = 0x00;
}

void delay_ms(int ms)
{
	int i = 0;
	for (i = ms; i > 0; i--) {
		__delay_cycles(16000);
	}

	//TODO
	//Future work: make delay with timerino
}
