#include "state_h2h_wait.h"

/* Possible transition to the following states */
#include "state_h2h_connected.h"

/* Parent states */
#include "state_running.h"

/* State includes */
#include "cmsis_os.h"
#include "bluetooth_internal.h"
#include "authentication.h"
static void h2h_wait_to_start_connect(state_ptr state)
{
  /* Do transition actions */

  /* Change state */
  entry_to_h2h_connected(state);
}

/* State behaviour */
void behaviour_h2h_wait(state_ptr state)
{
  /* Set events to react to */
  state->h2h_start_connect = h2h_wait_to_start_connect;
  /* Do state actions */
  /* We close and reopen the spp port */
  CloseServer();
  OpenServer(); 
}

/* Entry point to the state */
void entry_to_h2h_wait(state_ptr state)
{
  /* Set state name */
  strcpy(state->name, "h2h_wait");

  /* - Initialize with default implementation
   * - Set event behaviour
   * - Set parent events behaviour
   */
  default_implementation(state);
  behaviour_running(state);
  behaviour_h2h_wait(state);
}