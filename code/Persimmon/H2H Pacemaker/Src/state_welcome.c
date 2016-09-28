#include "state_welcome.h"

/* Possible transition to the following states */
#include "state_running.h"

/* State includes */
#include "cmsis_os.h"
#include "gpio.h"
#include "buzzer.h"
#include "bluetooth_internal.h"

/* Queues */
extern osMailQId queue_periph_buzzerHandle;

/* State behaviour */
void behaviour_welcome(state_ptr state)
{
  uint32_t i;
  buzzer_note_t beep;

  /* Set events to react to */

  /* Do state actions */

  /* Blink green LED */
  HAL_GPIO_WritePin(GPIOC,UI_LED_R_Pin|UI_LED_B_Pin|UI_LED_G_Pin,GPIO_PIN_RESET);
  for (i = 0; i < 4; i++)
  {
    HAL_GPIO_TogglePin(GPIOC,UI_LED_G_Pin);
    osDelay(200);
  }

  /* Beep */
  beep.note = D4;
  beep.ms = 100;
  while(osMailPut(queue_periph_buzzerHandle, (void *) &beep) != osOK)
  {
    osDelay(1);
  }
  osDelay(105);

  beep.note = A4;
  beep.ms = 80;
  while(osMailPut(queue_periph_buzzerHandle, (void *) &beep) != osOK)
  {
    osDelay(1);
  }
  
  bluetooth_init();
  
  entry_to_running(state);
}

/* Entry point to the state */
void entry_to_welcome(state_ptr state)
{
  /* Set state name */
  strcpy(state->name, "welcome");

  /* - Initialize with default implementation
   * - Set event behaviour
   * - Set parent events behaviour
   */
  default_implementation(state);
  behaviour_welcome(state);
}