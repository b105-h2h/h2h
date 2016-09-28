#include "state_settings_configtabs_bluetooth.h"

/* Possible transition to the following states */

/* Parent states */
#include "state_settings_configtabs.h"
#include "state_settings.h"
#include "state_running.h"

/* State includes */
#include "cmsis_os.h"
#include "menu.h"

/* Queues */
extern osMailQId queue_input_menuHandle;
extern osMailQId queue_lcdHandle;

/* State behaviour */
void behaviour_settings_configtabs_bluetooth(state_ptr state)
{
  /* Set events to react to */

  /* Do state actions */
  menu_settings_configtabs.items[1].item.area.is_active = GUI_ACTIVE;
  menu_settings_configtabs.items[1].item.area.text_color = (color_t) COLOR_WHITE;
  menu_settings_configtabs.items[1].item.area.bg_color = (color_t) COLOR_BLUE;
  
  menu_settings_configtabs.items[2].item.area.is_active = GUI_ACTIVE;
  menu_settings_configtabs.items[2].item.area.text_color = (color_t) COLOR_WHITE;
  menu_settings_configtabs.items[2].item.area.bg_color = (color_t) COLOR_BLUE;

  menu_settings_configtabs.items[3].item.area.is_active = GUI_INACTIVE;
  menu_settings_configtabs.items[3].item.area.text_color = (color_t) COLOR_BLUE;
  menu_settings_configtabs.items[3].item.area.bg_color = (color_t) COLOR_WHITE;
  
  menu_settings_configtabs.items[4].item.area.is_active = GUI_ACTIVE;
  menu_settings_configtabs.items[4].item.area.text_color = (color_t) COLOR_WHITE;
  menu_settings_configtabs.items[4].item.area.bg_color = (color_t) COLOR_BLUE;
  
  menu_settings_configtabs.items[5].item.area.is_active = GUI_ACTIVE;
  menu_settings_configtabs.items[5].item.area.text_color = (color_t) COLOR_WHITE;
  menu_settings_configtabs.items[5].item.area.bg_color = (color_t) COLOR_BLUE;
  
//  /* Set menu */
//  osMailPut(queue_input_menuHandle, (void *) &menu_settings_configtabs_bluetooth);
//
//  /* Display menu */
//  uint32_t i;
//  for (i = 0; i < menu_settings_configtabs_bluetooth.item_num; i++)
//  {
//    osMailPut(queue_lcdHandle, (void *) &menu_settings_configtabs_bluetooth.items[i]);
//  }
}

/* Entry point to the state */
void entry_to_settings_configtabs_bluetooth(state_ptr state)
{
  /* Set state name */
  strcpy(state->name, "settings_configtabs_bluetooth");
  
  /* - Initialize with default implementation
   * - Set event behaviour
   * - Set parent events behaviour (bottom-up)
   */
  default_implementation(state);
  behaviour_settings_configtabs_bluetooth(state);
  behaviour_settings_configtabs(state);
  behaviour_settings(state);
  behaviour_running(state);
}