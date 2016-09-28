#include "tasks_fsm.h"

/* Semaphores */

/* Queues */
osMailQId queue_fsm_eventsHandle;

/* Tasks */
osThreadId fsm_managerTaskHandle;
void Start_fsm_managerTask(void const * argument);

/* Objects */
fsm_client_t fsm;

void tasks_fsm_init()
{
  /* Semaphores */

  /* Queues */
  /* queue_fsm_events */
  osMailQDef(queue_fsm_events, 8, fsm_event_f);
  queue_fsm_eventsHandle = osMailCreate(osMailQ(queue_fsm_events), NULL);
}

void tasks_fsm_start()
{
  /* Tasks */
  /* fsm_managerTask */
  osThreadDef(fsm_managerTask, Start_fsm_managerTask, osPriorityLow, 0, 512);
  fsm_managerTaskHandle = osThreadCreate(osThread(fsm_managerTask), NULL);
}

void Start_fsm_managerTask(void const * argument)
{
  osEvent event;
  fsm_event_f fsm_event;

  /* Initialize FSM */
  fsm_client_init(&fsm);

  /* Infinite loop */
  for(;;)
  {
    /* Get fsm event */
    event = osMailGet(queue_fsm_eventsHandle, osWaitForever);
    if (event.status == osEventMail)
    {
      /* Retrieve event function */
      fsm_event = *((fsm_event_f *) event.value.v);

      /* Trigger event */
      fsm_event(&fsm);
    }
  }
}