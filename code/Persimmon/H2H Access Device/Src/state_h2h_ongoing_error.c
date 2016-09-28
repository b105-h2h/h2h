#include "state_h2h_ongoing_error.h"

/* Possible transition to the following states */
#include "state_main.h"

/* Parent states */
#include "state_h2h_ongoing.h"
#include "state_h2h.h"
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

static void h2h_ongoing_error_to_main(state_ptr state)
{
  /* Do transition actions */

  /* Change state */
  entry_to_main(state);
}

/* State behaviour */
void behaviour_h2h_ongoing_error(state_ptr state)
{
  /* Set events to react to */
  state->back = h2h_ongoing_error_to_main;
  /* Do state actions */

  /* Set menu */
  osMutexWait(mutex_menuHandle, osWaitForever);
  menu_copy(&menu_h2h_ongoing_error, &current_menu);
  osMutexRelease(mutex_menuHandle);

  /* Display menu */
  uint32_t i;
  for (i = 0; i < menu_h2h_ongoing_error.item_num; i++)
  {
    while(osMailPut(queue_lcdHandle, (void *) &menu_h2h_ongoing_error.items[i]) != osOK)
    {
      osDelay(1);
    }
  }
}

/* Entry point to the state */
void entry_to_h2h_ongoing_error(state_ptr state)
{
  /* Set state name */
  strcpy(state->name, "h2h_ongoing_error");
  
  /* - Initialize with default implementation
   * - Set event behaviour
   * - Set parent events behaviour (bottom-up)
   */
  default_implementation(state);
  behaviour_h2h_ongoing_error(state);
  behaviour_h2h_ongoing(state);
  behaviour_h2h(state);
  behaviour_running(state);
}