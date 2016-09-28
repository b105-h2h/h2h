#include "state_settings.h"

/* Possible transition to the following states */

/* Child states */
#include "state_settings_configtabs.h"
#include "state_settings_touchcalib.h"

/* State behaviour */
void behaviour_settings(state_ptr state)
{
  /* Set events to react to */

  /* Do state actions */
}

/* Entry point to the state */
void entry_to_settings(state_ptr state)
{
  /* Set state name */
  strcpy(state->name, "settings");
  
  /* Go to child default state */
  entry_to_settings_configtabs(state);
}