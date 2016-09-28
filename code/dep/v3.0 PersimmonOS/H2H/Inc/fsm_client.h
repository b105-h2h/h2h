/*
 * fsm_client.h
 *
 *  Created on: 29/9/2015
 *      Author: Smau
 */

#ifndef FSM_CLIENT_H_
#define FSM_CLIENT_H_

/* State interface */
#include "state.h"
#include "state_welcome.h"

/* FSM client */
typedef struct fsm_client_t* fsm_client_ptr;
typedef struct fsm_client_t
{
  state_t state;
}
fsm_client_t;

void fsm_client_init(fsm_client_ptr fsm);

/* Event function pointer */
typedef void (*fsm_event_f)(fsm_client_ptr);

/* Events */
void fsm_back(fsm_client_ptr fsm);

void fsm_ecg(fsm_client_ptr fsm);
void fsm_h2h(fsm_client_ptr fsm);
void fsm_settings(fsm_client_ptr fsm);

void fsm_ecg_1_lead(fsm_client_ptr fsm);
void fsm_ecg_2_lead(fsm_client_ptr fsm);
void fsm_ecg_6_lead(fsm_client_ptr fsm);

void fsm_h2h_connect(fsm_client_ptr fsm);
void fsm_h2h_ok(fsm_client_ptr fsm);
void fsm_h2h_error(fsm_client_ptr fsm);

void fsm_settings_screen(fsm_client_ptr fsm);
void fsm_settings_timedate(fsm_client_ptr fsm);
void fsm_settings_bt(fsm_client_ptr fsm);
void fsm_settings_about(fsm_client_ptr fsm);
void fsm_settings_touch(fsm_client_ptr fsm);

void fsm_settings_touch_calib(fsm_client_ptr fsm);
void fsm_click(fsm_client_ptr fsm);

void fsm_button_short(fsm_client_ptr fsm);
void fsm_button_long(fsm_client_ptr fsm);

void fsm_gui_tick(fsm_client_ptr fsm);

void fsm_no_event(fsm_client_ptr fsm);

#endif /* FSM_CLIENT_H_ */