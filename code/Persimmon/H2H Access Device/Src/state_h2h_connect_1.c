#include "state_h2h_connect_1.h"

/* Possible transition to the following states */
#include "state_main.h"
#include "state_h2h_ongoing_error.h"
#include "state_h2h_wait_key.h"

/* Parent states */
#include "state_h2h.h"
#include "state_running.h"

/* State includes */
#include "cmsis_os.h"
#include "menu.h"
#include "bluetooth_internal.h"

/* Queues */
extern osMailQId queue_input_menuHandle;
extern osMailQId queue_lcdHandle;

static void h2h_connect_1_to_main(state_ptr state)
{
  /* Do transition actions */

  /* Change state */
  entry_to_main(state);
}

static void h2h_connect_1_to_wait_key(state_ptr state)
{
  /* Do transition actions */

  /* Change state */
  entry_to_h2h_wait_key(state);
}

static void h2h_connect_1_to_ongoing_error(state_ptr state)
{
  /* Do transition actions */

  /* Change state */
  entry_to_h2h_ongoing_error(state);
}

/* State behaviour */
void behaviour_h2h_connect_1(state_ptr state)
{
  device_info_t inquired_bt_devices[MAX_INQUIRY_RESULTS];
  /* Set events to react to */
  state->back = h2h_connect_1_to_main;
  state->h2h_start_gen = h2h_connect_1_to_wait_key;
  state->h2h_error = h2h_connect_1_to_ongoing_error;
  
  /* Do state actions */
  bt_get_remote_devices(inquired_bt_devices);
  SPPServiceDiscovery(inquired_bt_devices[0].physical_address);
}

/* Entry point to the state */
void entry_to_h2h_connect_1(state_ptr state)
{
  /* Set state name */
  strcpy(state->name, "h2h_connect_1");
  
  /* - Initialize with default implementation
   * - Set event behaviour
   * - Set parent events behaviour (bottom-up)
   */
  default_implementation(state);
  behaviour_h2h_connect_1(state);
  behaviour_h2h(state);
  behaviour_running(state);
}