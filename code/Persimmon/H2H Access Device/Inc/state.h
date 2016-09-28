/*
 *  state.h
 *
 *  Created on: 29/9/2015
 *      Author: Smau
 */

#ifndef STATE_H_
#define STATE_H_

#include <string.h>

/* State pointer */
typedef struct state_t* state_ptr;
/* Event pointer */
typedef void (*event_f)(state_ptr);

/* State */
typedef struct state_t
{
  char name[32];

  event_f back;

  event_f ecg;
  event_f h2h;
  event_f settings;

  event_f ecg_1_lead;
  event_f ecg_2_lead;
  event_f ecg_6_lead;

  event_f h2h_start_connect;
  event_f h2h_connect_1;
  event_f h2h_connect_2;
  event_f h2h_connect_3;
  event_f h2h_connect_4;
  event_f h2h_start_gen;
  event_f h2h_pass_ready;
  event_f h2h_ok;
  event_f h2h_error;

  event_f settings_screen;
  event_f settings_timedate;
  event_f settings_bt;
  event_f settings_about;
  event_f settings_touch;

  event_f settings_touch_calib;
  event_f click;
  
  event_f button_short;
  event_f button_long;
  
  event_f gui_tick;
}
state_t;

void default_implementation(state_ptr state);

#endif /* STATE_H_ */
