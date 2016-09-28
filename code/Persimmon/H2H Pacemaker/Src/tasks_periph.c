#include "tasks_periph.h"

/* Semaphores */
osSemaphoreId sem_periph_batteryHandle;
osSemaphoreId sem_periph_gauge_dma_rxHandle;
osSemaphoreId sem_periph_rtcHandle;

/* Queues */
osMailQId queue_periph_buzzerHandle;

/* Tasks */
osThreadId periph_buzzerTaskHandle;
osThreadId periph_batteryTaskHandle;
osThreadId periph_rtcTaskHandle;
void Start_periph_buzzerTask(void const * argument);
void Start_periph_batteryTask(void const * argument);
void Start_periph_rtcTask(void const * argument);

/* Objects */
extern buzzer_t buzzer;
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
  osMailQDef(queue_periph_buzzer, 8, buzzer_note_t);
  queue_periph_buzzerHandle = osMailCreate(osMailQ(queue_periph_buzzer), NULL);
}

void tasks_periph_start()
{
  /* Tasks */
  /* periph_buzzerTask */
  osThreadDef(periph_buzzerTask, Start_periph_buzzerTask, osPriorityNormal, 0, 64);
  periph_buzzerTaskHandle = osThreadCreate(osThread(periph_buzzerTask), NULL);

  /* periph_batteryTask */
  osThreadDef(periph_batteryTask, Start_periph_batteryTask, osPriorityNormal, 0, 100);
  periph_batteryTaskHandle = osThreadCreate(osThread(periph_batteryTask), NULL);

  /* periph_rtcTask */
  osThreadDef(periph_rtcTask, Start_periph_rtcTask, osPriorityNormal, 0, 100);
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

void Start_periph_batteryTask(void const * argument)
{
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

        /* Read charger values */
        if (HAL_GPIO_ReadPin(CHRG_CHG_GPIO_Port, CHRG_CHG_Pin))
        {
        }
        else
        {
        }
      }
    }
  }

}

void Start_periph_rtcTask(void const * argument)
{
  /* RTC time and date structures */
  RTC_TimeTypeDef sTime;
  RTC_DateTypeDef sDate;

  /* Infinite loop */
  for(;;)
  {
    /* When rtc alarm rings */
    if (osSemaphoreWait(sem_periph_rtcHandle, osWaitForever) == osOK)
    {
      /* Get time and date from rtc calendar */
      HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
      HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
    }
  }
}