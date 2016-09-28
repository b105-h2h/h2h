#include "state_running.h"

/* Possible transition to the following states */
#include "state_goodbye.h"

/* Child states */
#include "state_h2h_wait.h"
#include "state_h2h_connected.h"
#include "state_h2h_keygen.h"
#include "state_h2h_waitkey.h"
#include "state_h2h_auth.h"

/* State includes */
#include "cmsis_os.h"
#include "buzzer.h"
#include "gauge.h"

/* Queues */
extern osMailQId queue_periph_buzzerHandle;

/* Objects */
extern gauge_t gauge;

static void running_action_button_short(state_ptr state)
{
  uint32_t i, blink_num = 0;
  buzzer_note_t beep;

  /* Do transition actions */

  /* Check batt */
  if (gauge.last_data.soc < 10)
  {
    blink_num = 1;
  }
  else if (gauge.last_data.soc < 30)
  {
    blink_num = 2;
  }
  else if (gauge.last_data.soc < 70)
  {
    blink_num = 3;
  }
  else if (gauge.last_data.soc <= 100)
  {
    blink_num = 4;
  }

  /* Blink green and red LED */
  HAL_GPIO_WritePin(GPIOC,UI_LED_R_Pin|UI_LED_B_Pin|UI_LED_G_Pin,GPIO_PIN_RESET);
  for (i = 0; i < 2*blink_num; i++)
  {
    HAL_GPIO_TogglePin(GPIOC,UI_LED_R_Pin|UI_LED_G_Pin);
    osDelay(200);
  }

  /* Beep */
  beep.note = A4;
  beep.ms = 50;
  while(osMailPut(queue_periph_buzzerHandle, (void *) &beep) != osOK)
  {
    osDelay(1);
  }
}

static void running_to_goodbye(state_ptr state)
{
  /* Do transition actions */

  /* Change state */
  entry_to_goodbye(state);
}

/* State behaviour */
void behaviour_running(state_ptr state)
{
  /* Set events to react to */
  state->button_short = running_action_button_short;
  state->button_long = running_to_goodbye;

  /* Do state actions */
}

/* Entry point to the state */
void entry_to_running(state_ptr state)
{
  /* Set state name */
  strcpy(state->name, "running");

  /* Go to child default state */
  entry_to_h2h_wait(state);
}