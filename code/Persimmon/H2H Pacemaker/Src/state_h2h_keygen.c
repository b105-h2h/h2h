#include "state_h2h_keygen.h"

/* Possible transition to the following states */
#include "state_h2h_waitkey.h"

/* Parent states */
#include "state_running.h"

/* State includes */
#include "cmsis_os.h"

extern osSemaphoreId sem_ecg_keygenHandle;

static void h2h_keygen_to_waitkey(state_ptr state)
{
  /* Do transition actions */

  /* Change state */
  entry_to_h2h_waitkey(state);
}

/* State behaviour */
void behaviour_h2h_keygen(state_ptr state)
{
  /* Set events to react to */
  state->h2h_pass_ready       = h2h_keygen_to_waitkey;
  
  /* Do state actions */
  osSemaphoreRelease(sem_ecg_keygenHandle);
}

/* Entry point to the state */
void entry_to_h2h_keygen(state_ptr state)
{
  /* Set state name */
  strcpy(state->name, "h2h_keygen");

  /* - Initialize with default implementation
   * - Set event behaviour
   * - Set parent events behaviour
   */
  default_implementation(state);
  behaviour_running(state);
  behaviour_h2h_keygen(state);
}