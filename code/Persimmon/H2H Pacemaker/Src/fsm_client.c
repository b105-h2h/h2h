#include "fsm_client.h"

void fsm_client_init(fsm_client_ptr fsm)
{
  /* Specify the initial state */
  entry_to_welcome(&fsm->state);
}

/*
 * Event functions
 */
void fsm_h2h_start_connect(fsm_client_ptr fsm)
{
  /* Trigger event function */
  fsm->state.h2h_start_connect(&fsm->state);
}

void fsm_h2h_pass_ready(fsm_client_ptr fsm)
{
  /* Trigger event function */
  fsm->state.h2h_pass_ready(&fsm->state);
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

void fsm_no_event(fsm_client_ptr fsm)
{
}