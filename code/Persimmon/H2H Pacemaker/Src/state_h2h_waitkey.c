#include "state_h2h_waitkey.h"

/* Possible transition to the following states */
#include "state_h2h_wait.h"
#include "state_h2h_auth.h"

/* Parent states */
#include "state_running.h"

/* State includes */
#include "cmsis_os.h"
#include "bluetooth_internal.h"
#include "authentication.h"

extern osMailQId queue_bt_packet_sendHandle;
extern osMailQId queue_ecg_keybtHandle;

static void h2h_waitkey_to_wait(state_ptr state)
{
  /* Do transition actions */

  /* Change state */
  entry_to_h2h_wait(state);
}

static void h2h_waitkey_to_aut(state_ptr state)
{
  /* Do transition actions */

  /* Change state */
  entry_to_h2h_auth(state);
}

/* State behaviour */
void behaviour_h2h_waitkey(state_ptr state)
{
  bt_packet_t fsm_send_packet_1 = {.packet_content = {0}}, fsm_send_packet_2 = {.packet_content = {0}};
  osEvent event;
  validation_key_t inter_key;
  
  /* Set events to react to */
  state->h2h_ok       = h2h_waitkey_to_aut;
  state->h2h_error    = h2h_waitkey_to_wait;
  /* Do state actions */
  
  sprintf(&fsm_send_packet_1.packet_content[8],"%s",key_ready);
   /* Send Key ready */ 
  osMailPut(queue_bt_packet_sendHandle, (void *) &fsm_send_packet_1);
  
  event = osMailGet(queue_ecg_keybtHandle, osWaitForever);
  if (event.status == osEventMail)
  {
    inter_key = *((validation_key_t*) event.value.v);
    sprintf(&fsm_send_packet_2.packet_content[8],"%s",(char *)&(inter_key.token));
    fsm_send_packet_2.packet_content[0] = 1;
    /* Send Key */ 
    osMailPut(queue_bt_packet_sendHandle, (void *) &fsm_send_packet_2);
  }
}

/* Entry point to the state */
void entry_to_h2h_waitkey(state_ptr state)
{
  /* Set state name */
  strcpy(state->name, "h2h_waitkey");

  /* - Initialize with default implementation
   * - Set event behaviour
   * - Set parent events behaviour
   */
  default_implementation(state);
  behaviour_running(state);
  behaviour_h2h_waitkey(state);
}