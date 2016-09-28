#include "state_menu.h"

/* Possible transition to the following states */
#include "state_ecg.h"
#include "state_h2h.h"
#include "state_settings.h"

static void menu_to_ecg(state_ptr state)
{
  /* Do transition actions */

  /* Change state */
  entry_to_ecg(state);
}

static void menu_to_h2h(state_ptr state)
{
  /* Do transition actions */

  /* Change state */
  entry_to_h2h(state);
}

static void menu_to_settings(state_ptr state)
{
  /* Do transition actions */

  /* Change state */
//  entry_to_settings(state);
}

/* State behaviour */
void behaviour_menu(state_ptr state)
{
  /* Set events to react to */
  state->ecg = menu_to_ecg;
  state->h2h = menu_to_h2h;
  state->settings = menu_to_settings;

  /* Do state actions */
}

/* Entry point to the state */
void entry_to_menu(state_ptr state)
{
  /* - Initialize with default implementation
   * - Set event behaviour
   * - Set parent events behaviour (bottom-up)
   */
  default_implementation(state);
  behaviour_menu(state);

  /* Set state name */
  strcpy(state->name, "menu");
}