/*
 * display_timer.h
 *
 *  Created on: 29/9/2015
 *      Author: Smau
 */

#ifndef DISPLAY_DISPLAY_TIMER_H_
#define DISPLAY_DISPLAY_TIMER_H_

#include <msp430.h>
#include <stdint.h>

#include "display.h"
#include "display_src/display_interface.h"

#include "../data/ecg_data.h"

#define SWEEP_TIME_10000	10000	//Miliseconds
#define SWEEP_TIME_5000		5000
#define	SWEEP_TIME_2000		2000
#define SWEEP_TIME_1000		1000
#define SWEEP_TIME_500		500
#define	SWEEP_TIME_200		200

void	display_timer_setup();
void	display_timer_start();

#endif /* DISPLAY_DISPLAY_TIMER_H_ */
