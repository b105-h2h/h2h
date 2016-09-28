/*
 * buzzer.h
 *
 *  Created on: 29/9/2015
 *      Author: Smau
 */

#ifndef BUZZER_H_
#define BUZZER_H_

#include "gpio.h"
#include "tim.h"

#include "buzzer_constants.h"

typedef struct buzzer_note_t
{
  uint16_t note;
  uint16_t ms;
}
buzzer_note_t;

typedef struct buzzer_t
{
  TIM_HandleTypeDef *htim_note;
  TIM_HandleTypeDef *htim_duration;
}
buzzer_t;

void buzzer_init(buzzer_t *buzzer,
                 TIM_HandleTypeDef *htim_note, TIM_HandleTypeDef *htim_duration);

void buzzer_play(buzzer_t *buzzer, buzzer_note_t *note);
void buzzer_stop(buzzer_t *buzzer);

#endif /* BUZZER_H_ */
