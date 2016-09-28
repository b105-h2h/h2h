#include "state_h2h_connected.h"

/* Possible transition to the following states */
#include "state_h2h_keygen.h"

/* Parent states */
#include "state_running.h"

/* State includes */
#include "cmsis_os.h"
#include "bluetooth_internal.h"

/* Semaphores */
extern osSemaphoreId sem_ecg_keygenHandle;
extern osMailQId queue_bt_packet_sendHandle;
/* Queues */


static void h2h_connect_to_keygen(state_ptr state)
{
  /* Do transition actions */

  /* Change state */
  entry_to_h2h_keygen(state);
}

/* State behaviour */
void behaviour_h2h_connected(state_ptr state)
{
  bt_packet_t fsm_send_packet = {.packet_content = {0}};
  /* Set events to react to */
  state->h2h_ok       = h2h_connect_to_keygen;
  /* Do state actions */
  
  sprintf(&fsm_send_packet.packet_content[8],"%s",gen_init);
  
  osMailPut(queue_bt_packet_sendHandle, (void *) &fsm_send_packet);
}

/* Entry point to the state */
void entry_to_h2h_connected(state_ptr state)
{
  /* Set state name */
  strcpy(state->name, "h2h_connected");

  /* - Initialize with default implementation
   * - Set event behaviour
   * - Set parent events behaviour
   */
  default_implementation(state);
  behaviour_running(state);
  behaviour_h2h_connected(state);
}