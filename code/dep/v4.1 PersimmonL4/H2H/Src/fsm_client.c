#include "fsm_client.h"

void fsm_client_init(fsm_client_ptr fsm)
{
  /* Specify the initial state */
  entry_to_welcome(&fsm->state);
}

/*
 * Event functions
 */
void fsm_back(fsm_client_ptr fsm)
{
  /* Trigger event function */
  fsm->state.back(&fsm->state);
}

void fsm_ecg(fsm_client_ptr fsm)
{
  /* Trigger event function */
  fsm->state.ecg(&fsm->state);
}

void fsm_h2h(fsm_client_ptr fsm)
{
  /* Trigger event function */
  fsm->state.h2h(&fsm->state);
}

void fsm_settings(fsm_client_ptr fsm)
{
  /* Trigger event function */
  fsm->state.settings(&fsm->state);
}

void fsm_ecg_1_lead(fsm_client_ptr fsm)
{
  /* Trigger event function */
  fsm->state.ecg_1_lead(&fsm->state);
}

void fsm_ecg_2_lead(fsm_client_ptr fsm)
{
  /* Trigger event function */
  fsm->state.ecg_2_lead(&fsm->state);
}

void fsm_ecg_6_lead(fsm_client_ptr fsm)
{
  /* Trigger event function */
  fsm->state.ecg_6_lead(&fsm->state);
}

void fsm_h2h_connect(fsm_client_ptr fsm)
{
  /* Trigger event function */
  fsm->state.h2h_connect(&fsm->state);
}

void fsm_h2h_ok(fsm_client_ptr fsm)
{
  /* Trigger event function */
  fsm->state.h2h_ok(&fsm->state);
}

void fsm_h2h_error(fsm_client_ptr fsm)
{
  /* Trigger event function */
  fsm->state.h2h_error(&fsm->state);
}

void fsm_settings_screen(fsm_client_ptr fsm)
{
  /* Trigger event function */
  fsm->state.settings_screen(&fsm->state);
}

void fsm_settings_timedate(fsm_client_ptr fsm)
{
  /* Trigger event function */
  fsm->state.settings_timedate(&fsm->state);
}

void fsm_settings_bt(fsm_client_ptr fsm)
{
  /* Trigger event function */
  fsm->state.settings_bt(&fsm->state);
}

void fsm_settings_about(fsm_client_ptr fsm)
{
  /* Trigger event function */
  fsm->state.settings_about(&fsm->state);
}

void fsm_settings_touch(fsm_client_ptr fsm)
{
  /* Trigger event function */
  fsm->state.settings_touch(&fsm->state);
}

void fsm_settings_touch_calib(fsm_client_ptr fsm)
{
  /* Trigger event function */
  fsm->state.settings_touch_calib(&fsm->state);
}

void fsm_click(fsm_client_ptr fsm)
{
  /* Trigger event function */
  fsm->state.click(&fsm->state);
}

void fsm_button_short(fsm_client_ptr fsm)
{
  /* Trigger event function */
  fsm->state.button_short(&fsm->state);
}

void fsm_button_long(fsm_client_ptr fsm)
{
  /* Trigger event function */
  fsm->state.button_long(&fsm->state);
}

void fsm_gui_tick(fsm_client_ptr fsm)
{
  /* Trigger event function */
  fsm->state.gui_tick(&fsm->state);  
}

void fsm_no_event(fsm_client_ptr fsm)
{
}