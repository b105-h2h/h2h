#include "state_goodbye.h"

/* Possible transition to the following states */

/* State includes */
#include "cmsis_os.h"
#include "gpio.h"
#include "rtc.h"
#include "buzzer.h"

/* Queues */
extern osMailQId queue_periph_buzzerHandle;

/* Objects */
extern RTC_HandleTypeDef hrtc;

/* State behaviour */
void behaviour_goodbye(state_ptr state)
{
  uint32_t i;
  buzzer_note_t beep;

  /* Set events to react to */

  /* Do state actions */

  /* Blink green pin before shutting down */
  HAL_GPIO_WritePin(GPIOC,UI_LED_R_Pin|UI_LED_B_Pin|UI_LED_G_Pin,GPIO_PIN_RESET);
  for (i = 0; i < 6; i++)
  {
    HAL_GPIO_TogglePin(GPIOC,UI_LED_G_Pin);
    osDelay(200);
  }

  /* Beep */
  beep.note = A4;
  beep.ms = 100;
  while(osMailPut(queue_periph_buzzerHandle, (void *) &beep) != osOK)
  {
    osDelay(1);
  }
  osDelay(105);

  beep.note = D4;
  beep.ms = 80;
  while(osMailPut(queue_periph_buzzerHandle, (void *) &beep) != osOK)
  {
    osDelay(1);
  }
  osDelay(80);

  // Sleep well little prince
  /* Turn off LED */
  HAL_GPIO_WritePin(GPIOC,UI_LED_R_Pin|UI_LED_B_Pin|UI_LED_G_Pin,GPIO_PIN_RESET);

  /* Disable RTC alarms to avoid waking the system up */
  __HAL_RTC_WRITEPROTECTION_DISABLE(&hrtc);
  __HAL_RTC_ALARMA_DISABLE(&hrtc);

  /* Enable wakeup pin and go to sleep */
  HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);
  __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
  HAL_PWR_EnterSTANDBYMode();
}

/* Entry point to the state */
void entry_to_goodbye(state_ptr state)
{
  /* Set state name */
  strcpy(state->name, "goodbye");

  /* - Initialize with default implementation
   * - Set event behaviour
   * - Set parent events behaviour
   */
  default_implementation(state);
  behaviour_goodbye(state);
}