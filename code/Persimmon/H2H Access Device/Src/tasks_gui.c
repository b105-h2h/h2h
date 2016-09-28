#include "tasks_gui.h"

/* Semaphores */
osSemaphoreId sem_gui_tickHandle;

/* Queues */
extern osMailQId queue_fsm_eventsHandle;

/* Tasks */
osThreadId gui_tickTaskHandle;
void Start_gui_tickTask(void const * argument);

/* Objects */

void tasks_gui_init()
{
  /* Semaphores */
  /* sem_gui */
  osSemaphoreDef(sem_gui_tick);
  sem_gui_tickHandle = osSemaphoreCreate(osSemaphore(sem_gui_tick), 1);

  /* Queues */
}

void tasks_gui_start()
{
  /* Tasks */
  /* guiTask */
  osThreadDef(gui_tickTask, Start_gui_tickTask, osPriorityLow, 0, 80);
  gui_tickTaskHandle = osThreadCreate(osThread(gui_tickTask), NULL);
}

#define GUI_TICK_INTERVAL 10

void Start_gui_tickTask(void const * argument)
{
  fsm_event_f gui_fsm_event;

  /* Infinite loop */
  for(;;)
  {
    if (osSemaphoreWait(sem_gui_tickHandle, GUI_TICK_INTERVAL) == osErrorOS)
    {
      /* GUI tick */
      gui_fsm_event = fsm_gui_tick;
      while(osMailPut(queue_fsm_eventsHandle, (void *) &gui_fsm_event) != osOK)
      {
        osDelay(1);
      }
    }
  }
}