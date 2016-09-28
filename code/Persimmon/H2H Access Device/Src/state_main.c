#include "state_main.h"

/* Possible transition to the following states */
#include "state_ecg.h"
#include "state_h2h.h"
#include "state_settings.h"

/* Parent states */
#include "state_running.h"

/* State includes */
#include "cmsis_os.h"
#include "menu.h"

/* Mutexes */
extern osMutexId mutex_menuHandle;

/* Queues */
extern osMailQId queue_input_menuHandle;
extern osMailQId queue_lcdHandle;

/* Objects */
extern menu_t current_menu;

static void main_to_ecg(state_ptr state)
{
  /* Do transition actions */

  /* Change state */
  entry_to_ecg(state);
}

static void main_to_h2h(state_ptr state)
{
  /* Do transition actions */

  /* Change state */
  entry_to_h2h(state);
}

static void main_to_settings(state_ptr state)
{
  /* Do transition actions */

  /* Change state */
  entry_to_settings(state);
}

/* State behaviour */
void behaviour_main(state_ptr state)
{
  /* Set events to react to */
  state->ecg = main_to_ecg;
  state->h2h = main_to_h2h;
  state->settings = main_to_settings;

  /* Do state actions */

  /* Set menu */
  osMutexWait(mutex_menuHandle, osWaitForever);
  menu_copy(&menu_main, &current_menu);
  osMutexRelease(mutex_menuHandle);

  /* Display menu */
  uint32_t i;
  for (i = 0; i < menu_main.item_num; i++)
  {
    while (osMailPut(queue_lcdHandle, (void *) &menu_main.items[i]) != osOK)
    {
      osDelay(1);
    }
  }
}

/* Entry point to the state */
void entry_to_main(state_ptr state)
{
  /* Set state name */
  strcpy(state->name, "main");

  /* - Initialize with default implementation
   * - Set event behaviour
   * - Set parent events behaviour (bottom-up)
   */
  default_implementation(state);
  behaviour_main(state);
  behaviour_running(state);
}