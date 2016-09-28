#include "state_h2h_start_connect.h"

/* Possible transition to the following states */
#include "state_main.h"
#include "state_h2h_ongoing.h"

/* Parent states */
#include "state_h2h.h"
#include "state_running.h"

/* State includes */
#include "cmsis_os.h"
#include "menu.h"
#include "bluetooth_internal.h"

#define TIMEOUT 3

/* Mutexes */
extern osMutexId mutex_menuHandle;

/* Queues */
extern osMailQId queue_input_menuHandle;
extern osMailQId queue_lcdHandle;

/* Objects */
extern menu_t current_menu;

static void h2h_start_connect_to_main(state_ptr state)
{
  /* Do transition actions */

  /* Change state */
  entry_to_main(state);
}

static void h2h_start_connect_to_h2h_ongoing(state_ptr state)
{
  /* Do transition actions */

  /* Change state */
  entry_to_h2h_ongoing(state);
}

/* State behaviour */
void behaviour_h2h_start_connect(state_ptr state)
{
  uint8_t i;
  /* Set events to react to */
  state->back = h2h_start_connect_to_main;
  state->h2h_start_connect = h2h_start_connect_to_h2h_ongoing;

  /* Do state actions */

  /*Clean devices names */
  for( i = 1; i < 5;i++)
  {
    item_area_set_text(&menu_h2h_devices.items[i].item.area," ");
    menu_h2h_devices.items[i].item.area.is_active = GUI_INACTIVE;
  }
  item_area_set_text(&menu_h2h_devices.items[6].item.area,"Searching for devices...");
  /* Set menu */
  osMutexWait(mutex_menuHandle, osWaitForever);
  menu_copy(&menu_h2h_devices, &current_menu);
  osMutexRelease(mutex_menuHandle);

  /* Display menu */
  for (i = 0; i < menu_h2h_devices.item_num; i++)
  {
    while (osMailPut(queue_lcdHandle, (void *) &menu_h2h_devices.items[i]) != osOK)
    {
      osDelay(1);
    }
  }

  /* Do state actions */
  /* We close and reopen the spp port in case of "zombie" connection*/
  CloseServer();
  OpenServer(); 
  Inquiry(TIMEOUT);
}

/* Entry point to the state */
void entry_to_h2h_start_connect(state_ptr state)
{
  /* Set state name */
  strcpy(state->name, "h2h_start_connect");

  /* - Initialize with default implementation
   * - Set event behaviour
   * - Set parent events behaviour (bottom-up)
   */
  default_implementation(state);
  behaviour_h2h_start_connect(state);
  behaviour_h2h(state);
  behaviour_running(state);
}