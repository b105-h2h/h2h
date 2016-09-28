#include "state_h2h_ongoing_connecting.h"

/* Possible transition to the following states */
#include "state_h2h_ongoing_error.h"
#include "state_h2h_ongoing_action.h"

/* Parent states */
#include "state_h2h_ongoing.h"
#include "state_h2h.h"
#include "state_running.h"

/* State includes */
#include "cmsis_os.h"
#include "menu.h"
//#include "bluetooth_internal.h"

/* Queues */
extern osMailQId queue_input_menuHandle;
extern osMailQId queue_lcdHandle;

static void h2h_ongoing_connecting_to_h2h_ongoing_error(state_ptr state)
{
  /* Do transition actions */

  /* Change state */
  entry_to_h2h_ongoing_error(state);
}

static void h2h_ongoing_connecting_to_h2h_ongoing_action(state_ptr state)
{
  /* Do transition actions */

  /* Change state */
  entry_to_h2h_ongoing_action(state);
}

/* State behaviour */
void behaviour_h2h_ongoing_connecting(state_ptr state)
{
//  BD_ADDR_t inquired_bt_addr[MAX_INQUIRY_RESULTS];
  uint32_t timeout = 5, number_of_btaddr = 0, i;

  /* Set events to react to */
  state->h2h_error = h2h_ongoing_connecting_to_h2h_ongoing_error;
  state->h2h_ok = h2h_ongoing_connecting_to_h2h_ongoing_action;

  /* Do state actions */
//  Inquiry(timeout);
//  osDelay((timeout)<<10);
//  number_of_btaddr = bt_get_ADDR(inquired_bt_addr);

  /* Set menu */
  osMailPut(queue_input_menuHandle, (void *) &menu_h2h_ongoing_connecting);

  /* Display menu */
  for (i = 0; i < menu_h2h_ongoing_connecting.item_num; i++)
  {
    osMailPut(queue_lcdHandle, (void *) &menu_h2h_ongoing_connecting.items[i]);
  }
}

/* Entry point to the state */
void entry_to_h2h_ongoing_connecting(state_ptr state)
{
  /* Set state name */
  strcpy(state->name, "h2h_ongoing_connecting");

  /* - Initialize with default implementation
   * - Set event behaviour
   * - Set parent events behaviour (bottom-up)
   */
  default_implementation(state);
  behaviour_h2h_ongoing_connecting(state);
  behaviour_h2h_ongoing(state);
  behaviour_h2h(state);
  behaviour_running(state);
}