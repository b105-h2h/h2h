#include "state_ecg_6_lead.h"

/* Possible transition to the following states */

/* Parent states */
#include "state_ecg.h"
#include "state_running.h"

/* State includes */
#include "menu.h"

/* State behaviour */
void behaviour_ecg_6_lead(state_ptr state)
{
  /* Set events to react to */

  /* Do state actions */
  menu_ecg.items[3].item.area.is_active = GUI_ACTIVE;
  menu_ecg.items[3].item.area.text_color = (color_t) COLOR_WHITE;
  menu_ecg.items[3].item.area.bg_color = (color_t) COLOR_BLUE;

  menu_ecg.items[4].item.area.is_active = GUI_ACTIVE;
  menu_ecg.items[4].item.area.text_color = (color_t) COLOR_WHITE;
  menu_ecg.items[4].item.area.bg_color = (color_t) COLOR_BLUE;
  
  menu_ecg.items[5].item.area.is_active = GUI_INACTIVE;
  menu_ecg.items[5].item.area.text_color = (color_t) COLOR_BLUE;
  menu_ecg.items[5].item.area.bg_color = (color_t) COLOR_WHITE;
}

/* Entry point to the state */
void entry_to_ecg_6_lead(state_ptr state)
{
  /* Set state name */
  strcpy(state->name, "ecg_6_lead");

  /* - Initialize with default implementation
   * - Set event behaviour
   * - Set parent events behaviour (bottom-up)
   */
  default_implementation(state);
  behaviour_ecg_6_lead(state);
  behaviour_ecg(state);
  behaviour_running(state);
}