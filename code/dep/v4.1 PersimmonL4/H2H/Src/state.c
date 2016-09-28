#include "state.h"

static void default_back(state_ptr state)
{
  /* Run if this state isn't supported in the state we are in */
}

static void default_ecg(state_ptr state)
{
  /* Run if this state isn't supported in the state we are in */
}

static void default_h2h(state_ptr state)
{
  /* Run if this state isn't supported in the state we are in */
}

static void default_settings(state_ptr state)
{
  /* Run if this state isn't supported in the state we are in */
}

static void default_ecg_1_lead(state_ptr state)
{
  /* Run if this state isn't supported in the state we are in */
}

static void default_ecg_2_lead(state_ptr state)
{
  /* Run if this state isn't supported in the state we are in */
}

static void default_ecg_6_lead(state_ptr state)
{
  /* Run if this state isn't supported in the state we are in */
}

static void default_h2h_connect(state_ptr state)
{
  /* Run if this state isn't supported in the state we are in */
}

static void default_h2h_ok(state_ptr state)
{
  /* Run if this state isn't supported in the state we are in */
}

static void default_h2h_error(state_ptr state)
{
  /* Run if this state isn't supported in the state we are in */
}

static void default_settings_screen(state_ptr state)
{
  /* Run if this state isn't supported in the state we are in */
}

static void default_settings_timedate(state_ptr state)
{
  /* Run if this state isn't supported in the state we are in */
}

static void default_settings_bt(state_ptr state)
{
  /* Run if this state isn't supported in the state we are in */
}

static void default_settings_about(state_ptr state)
{
  /* Run if this state isn't supported in the state we are in */
}

static void default_settings_touch(state_ptr state)
{
  /* Run if this state isn't supported in the state we are in */
}

static void default_settings_touch_calib(state_ptr state)
{
  /* Run if this state isn't supported in the state we are in */
}

static void default_click(state_ptr state)
{
  /* Run if this state isn't supported in the state we are in */
}

static void default_button_short(state_ptr state)
{
  /* Run if this state isn't supported in the state we are in */
}

static void default_button_long(state_ptr state)
{
  /* Run if this state isn't supported in the state we are in */
}

static void default_gui_tick(state_ptr state)
{
  /* Run if this state isn't supported in the state we are in */
}

void default_implementation(state_ptr state)
{
  state->back                 = default_back;
  state->ecg                  = default_ecg;
  state->h2h                  = default_h2h;
  state->settings             = default_settings;
  state->ecg_1_lead           = default_ecg_1_lead;
  state->ecg_2_lead           = default_ecg_2_lead;
  state->ecg_6_lead           = default_ecg_6_lead;
  state->h2h_connect          = default_h2h_connect;
  state->h2h_ok               = default_h2h_ok;
  state->h2h_error            = default_h2h_error;
  state->settings_screen      = default_settings_screen;
  state->settings_timedate    = default_settings_timedate;
  state->settings_bt          = default_settings_bt;
  state->settings_about       = default_settings_about;
  state->settings_touch       = default_settings_touch;
  state->settings_touch_calib = default_settings_touch_calib;
  state->click                = default_click;
  state->button_short         = default_button_short;
  state->button_long          = default_button_long;
  state->gui_tick             = default_gui_tick;
}