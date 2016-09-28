#include "state_h2h.h"

/* Possible transition to the following states */

/* Child states */
#include "state_h2h_connect.h"
#include "state_h2h_ongoing.h"

/* State behaviour */
void behaviour_h2h(state_ptr state)
{
  /* Set events to react to */

  /* Do state actions */
}

/* Entry point to the state */
void entry_to_h2h(state_ptr state)
{
  /* Set state name */
  strcpy(state->name, "h2h");

  /* Go to child default state */
  entry_to_h2h_connect(state);
}