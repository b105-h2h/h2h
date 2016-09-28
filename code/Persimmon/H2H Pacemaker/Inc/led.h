/*
 * led.h
 *
 *  Created on: 23/6/2016
 *      Author: Smau
 */

#ifndef LED_H_
#define LED_H_

#include "gpio.h"

#define LED_RED         UI_LED_R_Pin
#define LED_GREEN       UI_LED_G_Pin
#define LED_BLUE        UI_LED_B_Pin

void led_toggle(uint16_t led);
void led_on(uint16_t led);
void led_off(uint16_t led);

#endif /* LED_H_ */
