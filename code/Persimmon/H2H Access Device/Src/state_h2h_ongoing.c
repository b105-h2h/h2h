#include "state_h2h_ongoing.h"

/* Possible transition to the following states */
#include "state_h2h_start_connect.h"

/* Child states */
#include "state_h2h_ongoing_connecting.h"
#include "state_h2h_ongoing_error.h"
#include "state_h2h_ongoing_action.h"

/* State includes */
#include "cmsis_os.h"
#include "menu.h"
#include "bluetooth_internal.h"

static void h2h_ongoing_to_h2h_connect(state_ptr state)
{
  /* Do transition actions */

  /* Change state */
  entry_to_h2h_start_connect(state);
}

/* State behaviour */
void behaviour_h2h_ongoing(state_ptr state)
{

  /* Set events to react to */
  state->back = h2h_ongoing_to_h2h_connect;

  /* Do state actions */

}

/* Entry point to the state */
void entry_to_h2h_ongoing(state_ptr state)
{
  /* Set state name */
  strcpy(state->name, "h2h_ongoing");

  /* Go to child default state */
  entry_to_h2h_ongoing_connecting(state);
}