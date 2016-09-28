#include "state_ecg_2_lead.h"

/* Possible transition to the following states */

/* Parent states */
#include "state_ecg.h"
#include "state_running.h"

/* State includes */
#include "cmsis_os.h"
#include "menu.h"
#include "leads.h"

/* Mutexes */
extern osMutexId mutex_ecg_leadsHandle;

/* Queues */
extern osMailQId queue_lcdHandle;
extern osMessageQId queue_ecg_bpm_screenHandle;
extern osMessageQId queue_ecg_lead_IHandle;

/* Objects */
extern leads_t leads;

static void ecg_2_lead_gui_tick(state_ptr state)
{
  uint32_t i = 0;
  osEvent event;

  uint32_t bpm = 0;
  char str_bpm[4] = "-?-";

  int32_t lead_I = 0, lead_II = 0;

  /* Do transition actions */
  /* Get bpm data */
  event = osMessageGet(queue_ecg_bpm_screenHandle, 0);
  if (event.status == osEventMessage)
  {
    /* Retrieve value */
    bpm = (((uint32_t) event.value.v) >> 10);
    sprintf(str_bpm, "%u", bpm);

    item_area_set_text(&menu_ecg.items[1].item.area, str_bpm);
    while (osMailPut(queue_lcdHandle, (void *) &menu_ecg.items[1]) != osOK)
    {
      osDelay(1);
    }
  }

  /* Get data and update graphs */
  osMutexWait(mutex_ecg_leadsHandle, osWaitForever);
  lead_I  = leads.lead_I;
  lead_II = leads.lead_II;
  osMutexRelease(mutex_ecg_leadsHandle);

  item_graph_add_value(&graph_ecg_2_lead.items[0].item.graph, lead_I);
  item_graph_add_value(&graph_ecg_2_lead.items[1].item.graph, lead_II);

  for (i = 0; i < graph_ecg_2_lead.item_num; i++)
  {
    graph_ecg_2_lead.items[i].item_print_function = lcd_update_graph;
    while (osMailPut(queue_lcdHandle, (void *) &graph_ecg_2_lead.items[i]) != osOK)
    {
      osDelay(1);
    }
  }
}

/* State behaviour */
void behaviour_ecg_2_lead(state_ptr state)
{
  uint32_t i = 0;

  /* Set events to react to */
  state->gui_tick = ecg_2_lead_gui_tick;

  /* Do state actions */

  /* Print graphs */
  for (i = 0; i < graph_ecg_2_lead.item_num; i++)
  {
    item_graph_reset_value(&graph_ecg_2_lead.items[i].item.graph);
    graph_ecg_2_lead.items[i].item_print_function = lcd_print_graph;
    while (osMailPut(queue_lcdHandle, (void *) &graph_ecg_2_lead.items[i]) != osOK)
    {
      osDelay(1);
    }
  }

  /* Print tags */
  for (i = 0; i < tags_ecg_2_lead.item_num; i++)
  {
    while (osMailPut(queue_lcdHandle, (void *) &tags_ecg_2_lead.items[i]) != osOK)
    {
      osDelay(1);
    }
  }
}

/* Entry point to the state */
void entry_to_ecg_2_lead(state_ptr state)
{
  /* Set state name */
  strcpy(state->name, "ecg_2_lead");

  /* - Initialize with default implementation
   * - Set event behaviour
   * - Set parent events behaviour (bottom-up)
   */
  default_implementation(state);
  behaviour_running(state);
  behaviour_ecg(state);
  behaviour_ecg_2_lead(state);
}