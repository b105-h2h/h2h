#include "state_h2h_autentication.h"

/* Possible transition to the following states */
#include "state_h2h_validation_ok.h"
#include "state_h2h_validation_error.h"

/* Parent states */
#include "state_h2h.h"
#include "state_running.h"

/* State includes */
#include "cmsis_os.h"
#include "menu.h"
#include "bluetooth_internal.h"
#include "authentication.h"

/* Semaphores */
extern osSemaphoreId sem_ecg_keygenHandle;
/* Queues */
extern osMailQId queue_bt_packet_sendHandle;
extern osMailQId queue_lcdHandle;
extern osMailQId queue_ecg_keyfsmHandle;
extern osMailQId queue_ecg_keyHandle;

static void h2h_autentication_to_validation_ok(state_ptr state)
{
  /* Do transition actions */

  /* Change state */
  entry_to_h2h_validation_ok(state);
}

static void h2h_autentication_to_validation_error(state_ptr state)
{
  /* Do transition actions */

  /* Change state */
  entry_to_h2h_validation_error(state);
}

/* State behaviour */
void behaviour_h2h_autentication(state_ptr state)
{
  osEvent event;
  validation_key_t bt_key;
  /* Set events to react to */
  state->h2h_ok                 = h2h_autentication_to_validation_ok;
  state->h2h_error              = h2h_autentication_to_validation_error;
  /* Do state actions */
  
  item_area_set_text(&menu_h2h_devices.items[6].item.area,"Validating Key...");
  while (osMailPut(queue_lcdHandle, (void *) &menu_h2h_devices.items[6]) != osOK)
  {
    osDelay(1);
  }  
  
  event = osMailGet(queue_ecg_keyfsmHandle, osWaitForever);
  if (event.status == osEventMail)
  {
    bt_key = *((validation_key_t*) event.value.v);
    osMailPut(queue_ecg_keyHandle, (void *) &bt_key);  
  }
}

/* Entry point to the state */
void entry_to_h2h_autentication(state_ptr state)
{
  /* Set state name */
  strcpy(state->name, "h2h_autentication");
  
  /* - Initialize with default implementation
   * - Set event behaviour
   * - Set parent events behaviour (bottom-up)
   */
  default_implementation(state);
  behaviour_running(state);
  behaviour_h2h(state);
  behaviour_h2h_autentication(state);
}