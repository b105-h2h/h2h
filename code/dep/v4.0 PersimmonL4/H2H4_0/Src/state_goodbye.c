#include "state_goodbye.h"

/* Possible transition to the following states */

/* State includes */
#include "cmsis_os.h"
#include "menu.h"

/* Queues */
extern osMailQId queue_input_menuHandle;
extern osMailQId queue_lcdHandle;

/* Objects */
extern RTC_HandleTypeDef hrtc;

/* State behaviour */
void behaviour_goodbye(state_ptr state)
{
  /* Set events to react to */

  /* Do state actions */

  /* Set menu */
  osMailPut(queue_input_menuHandle, (void *) &menu_goodbye);

  /* Display menu */
  uint32_t i;
  
  for (i = 0; i < menu_goodbye.item_num; i++)
  {
    osMailPut(queue_lcdHandle, (void *) &menu_goodbye.items[i]);
  }

  /* Do state actions */
  osDelay(2000);

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
   * - Set parent events behaviour (bottom-up)
   */
  default_implementation(state);
  behaviour_goodbye(state);
}