/*
 * menu.h
 *
 *  Created on: 24/05/2016
 *      Author: Smau
 */
#ifndef MENU_H_
#define MENU_H_

#include "item.h"
#include "fonts.h"

#include "lcd.h"
#include "click.h"

#define MENU_MAX_ITEMS 7

typedef struct menu_t
{
  uint32_t item_num;
  item_action_t items[MENU_MAX_ITEMS];
}
menu_t;

int32_t menu_search_click(menu_t *menu, click_t *click, item_t *item);
int32_t menu_copy(menu_t const *orig_menu, menu_t *dest_menu);

/* Menus */
extern const menu_t menu_welcome;
extern const menu_t menu_goodbye;
extern menu_t menu_top_bar;

extern const menu_t menu_main;
extern menu_t menu_ecg;
extern menu_t graph_ecg_1_lead;
extern menu_t graph_ecg_2_lead;
extern menu_t graph_ecg_6_lead;
extern const menu_t tags_ecg_1_lead;
extern const menu_t tags_ecg_2_lead;
extern const menu_t tags_ecg_6_lead;

extern const menu_t menu_h2h_ongoing_connecting;
extern const menu_t menu_h2h_ongoing_error;
extern const menu_t menu_h2h_ongoing_action;
extern menu_t menu_h2h_devices;

extern const menu_t menu_settings_configtabs;
extern const menu_t menu_settings_configtabs_screen;
extern const menu_t menu_settings_configtabs_timedate;
extern const menu_t menu_settings_configtabs_bluetooth;
extern const menu_t menu_settings_configtabs_touch;
extern const menu_t menu_settings_configtabs_about;

#endif /* MENU_H_ */