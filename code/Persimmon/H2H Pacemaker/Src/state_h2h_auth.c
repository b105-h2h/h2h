#include "state_h2h_auth.h"

/* Possible transition to the following states */
#include "state_h2h_wait.h"

/* Parent states */
#include "state_running.h"

/* State includes */
#include "cmsis_os.h"

static void h2h_auth_to_wait(state_ptr state)
{
  /* Do transition actions */

  /* Change state */
  entry_to_h2h_wait(state);
}

/* State behaviour */
void behaviour_h2h_auth(state_ptr state)
{
  /* Set events to react to */
  state->h2h_ok       = h2h_auth_to_wait;
  /* Do state actions */
}

/* Entry point to the state */
void entry_to_h2h_auth(state_ptr state)
{
  /* Set state name */
  strcpy(state->name, "h2h_auth");

  /* - Initialize with default implementation
   * - Set event behaviour
   * - Set parent events behaviour
   */
  default_implementation(state);
  behaviour_running(state);
  behaviour_h2h_auth(state);
}