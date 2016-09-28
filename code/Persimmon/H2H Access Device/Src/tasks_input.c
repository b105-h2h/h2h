#include "tasks_input.h"

/* Mutexes */
osMutexId mutex_menuHandle;

/* Semaphores */
osSemaphoreId sem_input_touch_penHandle;
osSemaphoreId sem_input_button_short_pressHandle;
osSemaphoreId sem_input_button_long_pressHandle;
extern osSemaphoreId sem_ecg_keygenHandle;

/* Queues */
osMailQId queue_input_clickHandle;
osMailQId queue_input_menuHandle;
extern osMailQId queue_periph_buzzerHandle;
extern osMailQId queue_fsm_eventsHandle;

/* Tasks */
osThreadId input_touchTaskHandle;
osThreadId input_clickTaskHandle;
osThreadId input_buttonTaskHandle;
void Start_input_touchTask(void const * argument);
void Start_input_clickTask(void const * argument);
void Start_input_buttonTask(void const * argument);

/* Objects */
extern touch_t touch;
menu_t current_menu;

void tasks_input_init()
{
  /* Mutexes */
  osMutexDef(mutex_menu);
  mutex_menuHandle = osMutexCreate(osMutex(mutex_menu));

  /* Semaphores */
  /* sem_input_touch_pen */
  osSemaphoreDef(sem_input_touch_pen);
  sem_input_touch_penHandle = osSemaphoreCreate(osSemaphore(sem_input_touch_pen), 1);

  /* sem_input_button_short_press */
  osSemaphoreDef(sem_input_button_short_press);
  sem_input_button_short_pressHandle = osSemaphoreCreate(osSemaphore(sem_input_button_short_press), 1);

  /* sem_input_button_long_press */
  osSemaphoreDef(sem_input_button_long_press);
  sem_input_button_long_pressHandle = osSemaphoreCreate(osSemaphore(sem_input_button_long_press), 1);

  /* Queues */
  /* queue_input_click */
  osMailQDef(queue_input_click, 16, click_t);
  queue_input_clickHandle = osMailCreate(osMailQ(queue_input_click), NULL);

  /* queue_input_menu */
  osMailQDef(queue_input_menu, 4, menu_t *);
  queue_input_menuHandle = osMailCreate(osMailQ(queue_input_menu), NULL);
}

void tasks_input_start()
{
  /* Tasks */
  /* input_touchTask */
  osThreadDef(input_touchTask, Start_input_touchTask, osPriorityAboveNormal, 0, 64);
  input_touchTaskHandle = osThreadCreate(osThread(input_touchTask), NULL);

  /* input_clickTask */
  osThreadDef(input_clickTask, Start_input_clickTask, osPriorityAboveNormal, 0, 100);
  input_clickTaskHandle = osThreadCreate(osThread(input_clickTask), NULL);

  /* input_buttonTask */
  osThreadDef(input_buttonTask, Start_input_buttonTask, osPriorityAboveNormal, 0, 64);
  input_buttonTaskHandle = osThreadCreate(osThread(input_buttonTask), NULL);
}

void Start_input_touchTask(void const * argument)
{
  uint8_t is_first_click = 1;
  lcd_pos_t current_lcd_pos;
  click_t click;

  /* Infinite loop */
  for(;;)
  {
    if (osSemaphoreWait(sem_input_touch_penHandle, osWaitForever) == osOK)
    {
      /* Mask PEN interrupts */
      EXTI->IMR &= (~TP_PEN_Pin);

      /* A finger is touching the screen.
       * Keep reading values until finger is lifted
       */
      do
      {
        /* Read value */
        touch_read_position(&touch);
        current_lcd_pos = touch_get_lcd_pos(&touch);

        /* If we detect a touch with enough pressure */
        if (touch.is_clicked == T_CLICKED)
        {
          /* Prepare click position */
          click.pos.x_pos = current_lcd_pos.x_pos;
          click.pos.y_pos = current_lcd_pos.y_pos;

          /* If it is the first one (the first click) */
          if (is_first_click)
          {
            /* A finger just pressed the screen
             * with enough pressure!
             */
            click.click_type = CLICK_DOWN;
            while(osMailPut(queue_input_clickHandle, (void *) &click) != osOK)
            {
              osDelay(1);
            }

            /* No longer detecting the first click */
            is_first_click = 0;
          }
          /* If it is not the first one (it keeps pressing the screen) */
          else
          {
            /* Finger is still pressing the screen!
             */
            click.click_type = CLICK_HOLD;
            while(osMailPut(queue_input_clickHandle, (void *) &click) != osOK)
            {
              osDelay(1);
            }
          }
        }
        /* Finger detection sample interval */
        osDelay(50);
      }
      while (HAL_GPIO_ReadPin(TP_PEN_GPIO_Port, TP_PEN_Pin) == GPIO_PIN_RESET);
      /* Keep polling until PEN signal goes high */

      /* If we have detected at least one valid click */
      if (!is_first_click)
      {
        /* Finger is lifted from the screen!
         */
        click.click_type = CLICK_UP;
        while(osMailPut(queue_input_clickHandle, (void *) &click) != osOK)
        {
          osDelay(1);
        }
        /* Set last position as not clicked */
        touch.is_clicked = T_NOT_CLICKED;
        touch.current_pos.pressure = 0;
      }

      /* Reset first click status */
      is_first_click = 1;

      /* Unmask PEN interrupts */
      EXTI->IMR |= TP_PEN_Pin;
    }
  }
}

void Start_input_clickTask(void const * argument)
{
  osEvent event_click;
  item_t item;

  click_t *click;
  buzzer_note_t beep;

  /* Infinite loop */
  for(;;)
  {
    /* Get click */
    event_click = osMailGet(queue_input_clickHandle, osWaitForever);
    if (event_click.status == osEventMail)
    {
      /* Get click position */
      click = (click_t *) event_click.value.p;

      /* Do something based on click type */
      switch (click->click_type)
      {
        case CLICK_DOWN:
          break;
        case CLICK_HOLD:
          break;
        case CLICK_UP:
          /* Search item in menu on click position */
          osMutexWait(mutex_menuHandle, osWaitForever);
          if (menu_search_click(&current_menu, click, &item))
          {
            /* Send item event */
            while(osMailPut(queue_fsm_eventsHandle, (void *) &(item.area.event)) != osOK)
            {
              osDelay(1);
            }

            /* Beep */
            beep.note = A5;
            beep.ms = 50;
            while(osMailPut(queue_periph_buzzerHandle, (void *) &beep) != osOK)
            {
              osDelay(1);
            }
          }
          osMutexRelease(mutex_menuHandle);
          break;
        default:
          break;
      }
    }
  }
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

      // TEST
      osSemaphoreRelease(sem_ecg_keygenHandle);

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