#include "state_welcome.h"

/* Possible transition to the following states */
#include "state_running.h"

/* State includes */
#include "cmsis_os.h"
#include "menu.h"

/* Queues */
extern osMailQId queue_input_menuHandle;
extern osMailQId queue_lcdHandle;

/* State behaviour */
void behaviour_welcome(state_ptr state)
{
  /* Set events to react to */

  /* Do state actions */

  /* Set menu */
  osMailPut(queue_input_menuHandle, (void *) &menu_welcome);

  /* Display menu */
  uint32_t i;
  for (i = 0; i < menu_welcome.item_num; i++)
  {
    osMailPut(queue_lcdHandle, (void *) &menu_welcome.items[i]);
  }

  /* Do state actions */
  osDelay(2500);
  entry_to_running(state);
}

/* Entry point to the state */
void entry_to_welcome(state_ptr state)
{
  /* Set state name */
  strcpy(state->name, "welcome");
  
  /* - Initialize with default implementation
   * - Set event behaviour
   * - Set parent events behaviour (bottom-up)
   */
  default_implementation(state);
  behaviour_welcome(state);
}