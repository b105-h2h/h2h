#include "state_settings_configtabs.h"

/* Possible transition to the following states */
#include "state_main.h"

/* Child states */
#include "state_settings_configtabs_screen.h"
#include "state_settings_configtabs_timedate.h"
#include "state_settings_configtabs_bluetooth.h"
#include "state_settings_configtabs_about.h"
#include "state_settings_configtabs_touch.h"

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

static void settings_configtabs_to_main(state_ptr state)
{
  /* Do transition actions */

  /* Change state */
  entry_to_main(state);
}

static void settings_configtabs_to_settings_configtabs_screen(state_ptr state)
{
  /* Do transition actions */

  /* Change state */
  entry_to_settings_configtabs_screen(state);
}

static void settings_configtabs_to_settings_configtabs_timedate(state_ptr state)
{
  /* Do transition actions */

  /* Change state */
  entry_to_settings_configtabs_timedate(state);
}

static void settings_configtabs_to_settings_configtabs_bluetooth(state_ptr state)
{
  /* Do transition actions */

  /* Change state */
  entry_to_settings_configtabs_bluetooth(state);
}

static void settings_configtabs_to_settings_configtabs_about(state_ptr state)
{
  /* Do transition actions */

  /* Change state */
  entry_to_settings_configtabs_about(state);
}

static void settings_configtabs_to_settings_configtabs_touch(state_ptr state)
{
  /* Do transition actions */

  /* Change state */
  entry_to_settings_configtabs_touch(state);
}

/* State behaviour */
void behaviour_settings_configtabs(state_ptr state)
{
  /* Set events to react to */
  state->back = settings_configtabs_to_main;
  state->settings_screen = settings_configtabs_to_settings_configtabs_screen;
  state->settings_timedate = settings_configtabs_to_settings_configtabs_timedate;
  state->settings_bt = settings_configtabs_to_settings_configtabs_bluetooth;
  state->settings_about = settings_configtabs_to_settings_configtabs_about;
  state->settings_touch = settings_configtabs_to_settings_configtabs_touch;

  /* Do state actions */

  /* Set menu */
  osMutexWait(mutex_menuHandle, osWaitForever);
  menu_copy(&menu_settings_configtabs, &current_menu);
  osMutexRelease(mutex_menuHandle);

  /* Display menu */
  uint32_t i;
  for (i = 0; i < menu_settings_configtabs.item_num; i++)
  {
    while (osMailPut(queue_lcdHandle, (void *) &menu_settings_configtabs.items[i]) != osOK)
    {
      osDelay(1);
    }
  }
}

/* Entry point to the state */
void entry_to_settings_configtabs(state_ptr state)
{
  /* Set state name */
  strcpy(state->name, "settings_configtabs");

  /* Go to child default state */
  entry_to_settings_configtabs_screen(state);
}