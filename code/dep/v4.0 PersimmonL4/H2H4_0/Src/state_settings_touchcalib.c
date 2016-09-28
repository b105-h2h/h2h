#include "state_settings_touchcalib.h"

/* Possible transition to the following states */
#include "state_settings_configtabs_touch.h"

/* Child states */
#include "state_settings_touchcalib_point_1.h"
#include "state_settings_touchcalib_point_2.h"
#include "state_settings_touchcalib_point_3.h"

static void settings_touchcalib_to_settings_configtabs_touch(state_ptr state)
{
  /* Do transition actions */

  /* Change state */
  entry_to_settings_configtabs_touch(state);
}

/* State behaviour */
void behaviour_settings_touchcalib(state_ptr state)
{
  /* Set events to react to */
  state->back = settings_touchcalib_to_settings_configtabs_touch;

  /* Do state actions */
}

/* Entry point to the state */
void entry_to_settings_touchcalib(state_ptr state)
{
  /* Set state name */
  strcpy(state->name, "settings_touchcalib");
  
  /* Go to child default state */
  entry_to_settings_touchcalib_point_1(state);
}