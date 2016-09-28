#include "state_settings_configtabs_bluetooth.h"

/* Possible transition to the following states */

/* Parent states */
#include "state_settings_configtabs.h"
#include "state_settings.h"
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

/* State behaviour */
void behaviour_settings_configtabs_bluetooth(state_ptr state)
{
  /* Set events to react to */

  /* Do state actions */

  /* Set menu */
  osMutexWait(mutex_menuHandle, osWaitForever);
  menu_copy(&menu_settings_configtabs_bluetooth, &current_menu);
  osMutexRelease(mutex_menuHandle);

  /* Display menu */
  uint32_t i;
  for (i = 0; i < menu_settings_configtabs_about.item_num; i++)
  {
    while (osMailPut(queue_lcdHandle, (void *) &menu_settings_configtabs_bluetooth.items[i]) != osOK)
    {
      osDelay(1);
    }
  }
}

/* Entry point to the state */
void entry_to_settings_configtabs_bluetooth(state_ptr state)
{
  /* Set state name */
  strcpy(state->name, "settings_configtabs_bluetooth");

  /* - Initialize with default implementation
   * - Set event behaviour
   * - Set parent events behaviour (bottom-up)
   */
  default_implementation(state);
  behaviour_settings_configtabs_bluetooth(state);
  behaviour_settings_configtabs(state);
  behaviour_settings(state);
  behaviour_running(state);
}