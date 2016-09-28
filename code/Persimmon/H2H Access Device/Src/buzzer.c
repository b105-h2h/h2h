/*
 * buzzer.c
 *
 *  Created on: 22/6/2015
 *      Author: Smau
 */
#include "buzzer.h"

void buzzer_init(buzzer_t *buzzer,
                 TIM_HandleTypeDef *htim_note, TIM_HandleTypeDef *htim_duration)
{
  buzzer->htim_note = htim_note;
  buzzer->htim_duration = htim_duration;
}

void buzzer_play(buzzer_t *buzzer, buzzer_note_t *buzzer_note)
{
  TIM_OC_InitTypeDef sConfigOC;

  /* Set note*/
  buzzer->htim_note->Init.Period = buzzer_note->note;
  HAL_TIM_Base_Init(buzzer->htim_note);

  /* Set duty cycle to 50% */
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = buzzer_note->note >> 1;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_ENABLE;
  HAL_TIM_PWM_ConfigChannel(buzzer->htim_note, &sConfigOC, TIM_CHANNEL_3);

  /* Set duration */
  buzzer->htim_duration->Init.Period = buzzer_note->ms;
  HAL_TIM_Base_Init(buzzer->htim_duration);

  /* Reset counters */
  __HAL_TIM_SET_COUNTER(buzzer->htim_note, 0);
  __HAL_TIM_SET_COUNTER(buzzer->htim_duration, 0);

  /* Start playing note */
  HAL_TIM_PWM_Start(buzzer->htim_note, TIM_CHANNEL_3);
  HAL_TIM_OC_Start_IT(buzzer->htim_duration, TIM_CHANNEL_1);
}

void buzzer_stop(buzzer_t *buzzer)
{
  /* Stop playing note */
  HAL_TIM_PWM_Stop(buzzer->htim_note, TIM_CHANNEL_3);
  HAL_TIM_OC_Stop_IT(buzzer->htim_duration, TIM_CHANNEL_1);
}