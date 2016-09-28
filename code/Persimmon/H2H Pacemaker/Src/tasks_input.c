#include "tasks_input.h"

/* Semaphores */
osSemaphoreId sem_input_button_short_pressHandle;
osSemaphoreId sem_input_button_long_pressHandle;

/* Queues */
extern osMailQId queue_fsm_eventsHandle;

/* Tasks */
osThreadId input_buttonTaskHandle;
void Start_input_buttonTask(void const * argument);

void tasks_input_init()
{
  /* Semaphores */
  /* sem_input_button_short_press */
  osSemaphoreDef(sem_input_button_short_press);
  sem_input_button_short_pressHandle = osSemaphoreCreate(osSemaphore(sem_input_button_short_press), 1);

  /* sem_input_button_long_press */
  osSemaphoreDef(sem_input_button_long_press);
  sem_input_button_long_pressHandle = osSemaphoreCreate(osSemaphore(sem_input_button_long_press), 1);
}

void tasks_input_start()
{
  /* Tasks */
  /* input_buttonTask */
  osThreadDef(input_buttonTask, Start_input_buttonTask, osPriorityAboveNormal, 0, 64);
  input_buttonTaskHandle = osThreadCreate(osThread(input_buttonTask), NULL);
}

void Start_input_buttonTask(void const * argument)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  fsm_event_f button_fsm_event;

  /* Take both semaphores for the first time */
  osSemaphoreWait(sem_input_button_short_pressHandle, osWaitForever);
  osSemaphoreWait(sem_input_button_long_pressHandle, osWaitForever);

  /* Infinite loop */
  for(;;)
  {
    /* If I/O button is pressed */
    if (osSemaphoreWait(sem_input_button_short_pressHandle, osWaitForever) == osOK)
    {
      /* Make falling edge sensitive */
      GPIO_InitStruct.Pin = WAKEUP_Pin;
      GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      HAL_GPIO_Init(WAKEUP_GPIO_Port, &GPIO_InitStruct);

      /* Wait for falling edge */
      if (osSemaphoreWait(sem_input_button_long_pressHandle, 2000) == osErrorOS)
      {
        /* If falling edge is NOT detected before timeout
         * we have a long press
         */
        button_fsm_event = fsm_button_long;
        while(osMailPut(queue_fsm_eventsHandle, (void *) &button_fsm_event) != osOK)
        {
          osDelay(1);
        }
      }
      else
      {
        /* Make rising edge sensitive again */
        GPIO_InitStruct.Pin = WAKEUP_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(WAKEUP_GPIO_Port, &GPIO_InitStruct);

        /* If falling edge is detected before timeout
         * we have a short press
         */
        button_fsm_event = fsm_button_short;
        while(osMailPut(queue_fsm_eventsHandle, (void *) &button_fsm_event) != osOK)
        {
          osDelay(1);
        }
      }
    }
  }
}