#include "state.h"

static void default_h2h_start_connect(state_ptr state)
{
  /* Run if this state isn't supported in the state we are in */
}

static void default_h2h_pass_ready(state_ptr state)
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

static void default_button_short(state_ptr state)
{
  /* Run if this state isn't supported in the state we are in */
}

static void default_button_long(state_ptr state)
{
  /* Run if this state isn't supported in the state we are in */
}

void default_implementation(state_ptr state)
{
  state->h2h_start_connect    = default_h2h_start_connect;
  state->h2h_pass_ready       = default_h2h_pass_ready;
  state->h2h_ok               = default_h2h_ok;
  state->h2h_error            = default_h2h_error;

  state->button_short         = default_button_short;
  state->button_long          = default_button_long;
}