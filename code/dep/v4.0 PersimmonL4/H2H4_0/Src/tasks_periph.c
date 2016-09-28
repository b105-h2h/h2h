#include "tasks_periph.h"

/* Semaphores */
osSemaphoreId sem_periph_batteryHandle;
osSemaphoreId sem_periph_gauge_dma_rxHandle;
osSemaphoreId sem_periph_rtcHandle;

/* Queues */
osMailQId queue_periph_buzzerHandle;
osMailQId queue_lcdHandle;

/* Tasks */
osThreadId periph_buzzerTaskHandle;
osThreadId periph_screenTaskHandle;
osThreadId periph_batteryTaskHandle;
osThreadId periph_rtcTaskHandle;
void Start_periph_buzzerTask(void const * argument);
void Start_periph_screenTask(void const * argument);
void Start_periph_batteryTask(void const * argument);
void Start_periph_rtcTask(void const * argument);

/* Objects */
extern buzzer_t buzzer;
extern lcd_t lcd;
extern gauge_t gauge;
extern RTC_HandleTypeDef hrtc;

void tasks_periph_init()
{
  /* Semaphores */
  /* sem_periph_battery */
  osSemaphoreDef(sem_periph_battery);
  sem_periph_batteryHandle = osSemaphoreCreate(osSemaphore(sem_periph_battery), 1);

  /* sem_periph_gauge_dma_rx */
  osSemaphoreDef(sem_periph_gauge_dma_rx);
  sem_periph_gauge_dma_rxHandle = osSemaphoreCreate(osSemaphore(sem_periph_gauge_dma_rx), 1);

  /* sem_periph_rtc */
  osSemaphoreDef(sem_periph_rtc);
  sem_periph_rtcHandle = osSemaphoreCreate(osSemaphore(sem_periph_rtc), 1);

  /* Queues */
  /* queue_periph_buzzer */
  osMailQDef(queue_periph_buzzer, 4, buzzer_note_t);
  queue_periph_buzzerHandle = osMailCreate(osMailQ(queue_periph_buzzer), NULL);

  /* queue_lcd */
  osMailQDef(queue_lcd, 12, item_action_t);
  queue_lcdHandle = osMailCreate(osMailQ(queue_lcd), NULL);
}

void tasks_periph_start()
{
  /* Tasks */
  /* periph_buzzerTask */
  osThreadDef(periph_buzzerTask, Start_periph_buzzerTask, osPriorityNormal, 0, 64);
  periph_buzzerTaskHandle = osThreadCreate(osThread(periph_buzzerTask), NULL);

  /* periph_screenTask */
  osThreadDef(periph_screenTask, Start_periph_screenTask, osPriorityNormal, 0, 256);
  periph_screenTaskHandle = osThreadCreate(osThread(periph_screenTask), NULL);

  /* periph_batteryTask */
  osThreadDef(periph_batteryTask, Start_periph_batteryTask, osPriorityLow, 0, 128);
  periph_batteryTaskHandle = osThreadCreate(osThread(periph_batteryTask), NULL);

  /* periph_rtcTask */
  osThreadDef(periph_rtcTask, Start_periph_rtcTask, osPriorityLow, 0, 128);
  periph_rtcTaskHandle = osThreadCreate(osThread(periph_rtcTask), NULL);
}

void Start_periph_buzzerTask(void const * argument)
{
  osEvent event;
  buzzer_note_t *current_note;

  /* HOW TO beep beep
   *
   *  buzzer_note_t beep;
   *  beep.note = A5;
   *  beep.ms = 50;
   *  osMailPut(queue_periph_buzzerHandle, (void *) &beep);
   *
   * from any task
   */
  for(;;)
  {
    event = osMailGet(queue_periph_buzzerHandle, osWaitForever);
    if (event.status == osEventMail)
    {
      /* Get note */
      current_note = (buzzer_note_t *) event.value.p;
      /* Play note */
      buzzer_play(&buzzer, current_note);
    }
  }
}

void Start_periph_screenTask(void const * argument)
{
  osEvent event;

  item_action_t *current_item;

  for(;;)
  {
    event = osMailGet(queue_lcdHandle, osWaitForever);
    if (event.status == osEventMail)
    {
      /* Get item information */
      current_item = (item_action_t *) event.value.p;
      /* Do item function */
      (*(current_item->item_print_function))(&lcd, &current_item->item);
    }
  }
}

void Start_periph_batteryTask(void const * argument)
{
  char batt_soc_string[5];

  /* Reset semaphore */
  osSemaphoreWait(sem_periph_gauge_dma_rxHandle, osWaitForever);

  /* Infinite loop */
  for(;;)
  {
    /* Wait for gauge or charger to inform of changes in the battery */
    if (osSemaphoreWait(sem_periph_batteryHandle, osWaitForever) == osOK)
    {
      /* Read gauge values */
      gauge_start_read(&gauge);

      /* Wait for DMA to complete transaction */
      if (osSemaphoreWait(sem_periph_gauge_dma_rxHandle, osWaitForever) == osOK)
      {
        /* Format received bytes into usable data */
        gauge_format_data(&gauge);
      }

      /* Read charger values */

      /* Display battery status */
        /* Create batt soc string */
        sprintf(batt_soc_string, "%u%%", gauge.last_data.soc);
        item_area_set_text(&menu_top_bar.items[2].item.area, batt_soc_string);

        /* Create batt icon */
        if (gauge.last_data.soc < 5)
        {
          item_area_set_text_16(&menu_top_bar.items[1].item.area, L"\xF244");
        }
        else if (gauge.last_data.soc < 25)
        {
          item_area_set_text_16(&menu_top_bar.items[1].item.area, L"\xF243");
        }
        else if (gauge.last_data.soc < 50)
        {
          item_area_set_text_16(&menu_top_bar.items[1].item.area, L"\xF242");
        }
        else if (gauge.last_data.soc < 75)
        {
          item_area_set_text_16(&menu_top_bar.items[1].item.area, L"\xF241");
        }
        else if (gauge.last_data.soc <= 100)
        {
          item_area_set_text_16(&menu_top_bar.items[1].item.area, L"\xF240");
        }

        /* Display screen items */
        osMailPut(queue_lcdHandle, (void *) &menu_top_bar.items[1]);
        osMailPut(queue_lcdHandle, (void *) &menu_top_bar.items[2]);
    }
  }
}

void Start_periph_rtcTask(void const * argument)
{
  /* RTC time and date structures */
  RTC_TimeTypeDef sTime;
  RTC_DateTypeDef sDate;

  /* Time and date strings */
  char time_string[6];
  char date_string[11];

  /* Infinite loop */
  for(;;)
  {
    /* When rtc alarm rings */
    if (osSemaphoreWait(sem_periph_rtcHandle, osWaitForever) == osOK)
    {
      /* Get time and date from rtc calendar */
      HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
      HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

      /* Create time and date strings */
      if (sTime.Hours < 10)
      {
        if (sTime.Minutes < 10)
        {
          sprintf(time_string, "0%u:0%u", sTime.Hours, sTime.Minutes);
        }
        else
        {
          sprintf(time_string, "0%u:%u", sTime.Hours, sTime.Minutes);
        }
      }
      else
      {
        if (sTime.Minutes < 10)
        {
          sprintf(time_string, "%u:0%u", sTime.Hours, sTime.Minutes);
        }
        else
        {
          sprintf(time_string, "%u:%u", sTime.Hours, sTime.Minutes);
        }
      }
      sprintf(date_string, "%u/%u/20%u", sDate.Date, sDate.Month, sDate.Year);

      /* Update screen items */
      item_area_set_text(&menu_top_bar.items[4].item.area, time_string);
      item_area_set_text(&menu_top_bar.items[3].item.area, date_string);

      /* Draw time and date on screen */
      osMailPut(queue_lcdHandle, (void *) &menu_top_bar.items[4]);
      osMailPut(queue_lcdHandle, (void *) &menu_top_bar.items[3]);
    }
  }
}