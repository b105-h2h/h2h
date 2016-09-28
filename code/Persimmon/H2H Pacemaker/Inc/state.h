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

  event_f h2h_start_connect;
  event_f h2h_pass_ready;
  event_f h2h_ok;
  event_f h2h_error;

  event_f button_short;
  event_f button_long;
}
state_t;

void default_implementation(state_ptr state);

#endif /* STATE_H_ */
