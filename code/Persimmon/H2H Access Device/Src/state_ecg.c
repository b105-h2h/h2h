#include "state_ecg.h"

/* Possible transition to the following states */
#include "state_main.h"

/* Child states */
#include "state_ecg_1_lead.h"
#include "state_ecg_2_lead.h"
#include "state_ecg_6_lead.h"

/* State includes */
#include "cmsis_os.h"
#include "menu.h"

/* Mutexes */
extern osMutexId mutex_menuHandle;

/* Queues */
extern osMailQId queue_input_menuHandle;
extern osMailQId queue_lcdHandle;

/* Objects */
extern menu_t current_menu;

static void ecg_to_main(state_ptr state)
{
  /* Do transition actions */

  /* Change state */
  entry_to_main(state);
}

static void ecg_to_ecg_1_lead(state_ptr state)
{
  /* Do transition actions */

  /* Change state */
  entry_to_ecg_1_lead(state);
}

static void ecg_to_ecg_2_lead(state_ptr state)
{
  /* Do transition actions */

  /* Change state */
  entry_to_ecg_2_lead(state);
}

static void ecg_to_ecg_6_lead(state_ptr state)
{
  /* Do transition actions */

  /* Change state */
  entry_to_ecg_6_lead(state);
}

/* State behaviour */
void behaviour_ecg(state_ptr state)
{
  uint32_t i;

  /* Set events to react to */
  state->back = ecg_to_main;
  state->ecg_1_lead = ecg_to_ecg_1_lead;
  state->ecg_2_lead = ecg_to_ecg_2_lead;
  state->ecg_6_lead = ecg_to_ecg_6_lead;

  /* Do state actions */

  /* Set menu */
  osMutexWait(mutex_menuHandle, osWaitForever);
  menu_copy(&menu_ecg, &current_menu);
  osMutexRelease(mutex_menuHandle);

  /* Display menu */
  for (i = 0; i < menu_ecg.item_num; i++)
  {
    while (osMailPut(queue_lcdHandle, (void *) &menu_ecg.items[i]) != osOK)
    {
      osDelay(1);
    }
  }
}

/* Entry point to the state */
void entry_to_ecg(state_ptr state)
{
  /* Set state name */
  strcpy(state->name, "ecg");

  /* Go to child default state */
  entry_to_ecg_1_lead(state);
}