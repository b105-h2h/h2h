#include "tasks_ecg.h"

#define FS 1000

/* Semaphores */
osSemaphoreId sem_ecg_afe_drdyHandle;
osSemaphoreId sem_ecg_afe_dma_rxHandle;

/* Queues */
osMessageQId queue_ecg_afe_ch_1Handle;
osMessageQId queue_ecg_afe_ch_2Handle;
osMessageQId queue_ecg_filter_ch_1Handle;
osMessageQId queue_ecg_filter_ch_2Handle;
osMessageQId queue_ecg_lead_IHandle;
osMessageQId queue_ecg_lead_IIHandle;
osMessageQId queue_ecg_lead_IIIHandle;
osMessageQId queue_ecg_lead_aVRHandle;
osMessageQId queue_ecg_lead_aVLHandle;
osMessageQId queue_ecg_lead_aVFHandle;
osMessageQId queue_ecg_bpmHandle;

/* Tasks */
osThreadId ecg_afeTaskHandle;
osThreadId ecg_filterTaskHandle;
osThreadId ecg_leadGenTaskHandle;
osThreadId ecg_bpmDetTaskHandle;
osThreadId ecg_keyGenTaskHandle;
void Start_ecg_afeTask(void const * argument);
void Start_ecg_filterTask(void const * argument);
void Start_ecg_leadGenTask(void const * argument);
void Start_ecg_bpmDetTask(void const * argument);
void Start_ecg_keyGenTask(void const * argument);

/* Objects */
extern afe_t afe;

void tasks_ecg_init()
{
  /* Semaphores */
  /* sem_ecg_afe_drdy */
  osSemaphoreDef(sem_ecg_afe_drdy);
  sem_ecg_afe_drdyHandle = osSemaphoreCreate(osSemaphore(sem_ecg_afe_drdy), 1);

  /* sem_ecg_afe_dma_rx */
  osSemaphoreDef(sem_ecg_afe_dma_rx);
  sem_ecg_afe_dma_rxHandle = osSemaphoreCreate(osSemaphore(sem_ecg_afe_dma_rx), 1);

  /* Queues */
  /* queue_ecg_afe_ch_1 */
  osMessageQDef(queue_ecg_afe_ch_1, 4, int32_t);
  queue_ecg_afe_ch_1Handle = osMessageCreate(osMessageQ(queue_ecg_afe_ch_1), NULL);

  /* queue_ecg_afe_ch_2 */
  osMessageQDef(queue_ecg_afe_ch_2, 4, int32_t);
  queue_ecg_afe_ch_2Handle = osMessageCreate(osMessageQ(queue_ecg_afe_ch_2), NULL);

  /* queue_ecg_filter_ch_1 */
  osMessageQDef(queue_ecg_filter_ch_1, 4, int32_t);
  queue_ecg_filter_ch_1Handle = osMessageCreate(osMessageQ(queue_ecg_filter_ch_1), NULL);

  /* queue_ecg_filter_ch_2 */
  osMessageQDef(queue_ecg_filter_ch_2, 4, int32_t);
  queue_ecg_filter_ch_2Handle = osMessageCreate(osMessageQ(queue_ecg_filter_ch_2), NULL);

  /* queue_ecg_lead_I */
  osMessageQDef(queue_ecg_lead_I, 4, int32_t);
  queue_ecg_lead_IHandle = osMessageCreate(osMessageQ(queue_ecg_lead_I), NULL);

  /* queue_ecg_lead_II */
  osMessageQDef(queue_ecg_lead_II, 4, int32_t);
  queue_ecg_lead_IIHandle = osMessageCreate(osMessageQ(queue_ecg_lead_II), NULL);

  /* queue_ecg_lead_III */
  osMessageQDef(queue_ecg_lead_III, 4, int32_t);
  queue_ecg_lead_IIIHandle = osMessageCreate(osMessageQ(queue_ecg_lead_III), NULL);

  /* queue_ecg_lead_aVR */
  osMessageQDef(queue_ecg_lead_aVR, 4, int32_t);
  queue_ecg_lead_aVRHandle = osMessageCreate(osMessageQ(queue_ecg_lead_aVR), NULL);

  /* queue_ecg_lead_aVL */
  osMessageQDef(queue_ecg_lead_aVL, 4, int32_t);
  queue_ecg_lead_aVLHandle = osMessageCreate(osMessageQ(queue_ecg_lead_aVL), NULL);

  /* queue_ecg_lead_aVF */
  osMessageQDef(queue_ecg_lead_aVF, 4, int32_t);
  queue_ecg_lead_aVFHandle = osMessageCreate(osMessageQ(queue_ecg_lead_aVF), NULL);

  /* queue_ecg_bpm */
  osMessageQDef(queue_ecg_bpm, 4, uint32_t);
  queue_ecg_bpmHandle = osMessageCreate(osMessageQ(queue_ecg_bpm), NULL);
}

void tasks_ecg_start()
{
  /* Tasks */
  /* ecg_afeTask */
  osThreadDef(ecg_afeTask, Start_ecg_afeTask, osPriorityRealtime, 0, 64);
  ecg_afeTaskHandle = osThreadCreate(osThread(ecg_afeTask), NULL);

//  /* ecg_filterTask */
//  osThreadDef(ecg_filterTask, Start_ecg_filterTask, osPriorityNormal, 0, 128);
//  ecg_filterTaskHandle = osThreadCreate(osThread(ecg_filterTask), NULL);
//
//  /* leadGenTask */
//  osThreadDef(ecg_leadGenTask, Start_ecg_leadGenTask, osPriorityNormal, 0, 128);
//  ecg_leadGenTaskHandle = osThreadCreate(osThread(ecg_leadGenTask), NULL);
//
//  /* ecg_bpmDetTask */
//  osThreadDef(ecg_bpmDetTask, Start_ecg_bpmDetTask, osPriorityNormal, 0, 128);
//  ecg_bpmDetTaskHandle = osThreadCreate(osThread(ecg_bpmDetTask), NULL);
//
//  /* ecg_keyGenTask */
//  osThreadDef(ecg_keyGenTask, Start_ecg_keyGenTask, osPriorityNormal, 0, 64);
//  ecg_keyGenTaskHandle = osThreadCreate(osThread(ecg_keyGenTask), NULL);
}

void Start_ecg_afeTask(void const * argument)
{
  /* Reset semaphores */
  osSemaphoreWait(sem_ecg_afe_drdyHandle, osWaitForever);
  osSemaphoreWait(sem_ecg_afe_dma_rxHandle, osWaitForever);

  /* Infinite loop */
  for(;;)
  {
    /* Wait for AFE to inform when data is ready */
    if (osSemaphoreWait(sem_ecg_afe_drdyHandle, osWaitForever) == osOK)
    {
      /* Start reading data from AFE */
      afe_start_read(&afe);

      /* Wait for DMA to complete transaction */
      if (osSemaphoreWait(sem_ecg_afe_dma_rxHandle, osWaitForever) == osOK)
      {
        /* Format received bytes into usable data */
        afe_format_data(&afe);

        /* Output data to queues */
        osMessagePut(queue_ecg_afe_ch_1Handle, afe.last_data.ch1_data, 0);
        osMessagePut(queue_ecg_afe_ch_2Handle, afe.last_data.ch2_data, 0);
      }
    }
  }
}

void Start_ecg_filterTask(void const * argument)
{
  osEvent event_ch1, event_ch2;

  int32_t ch1_data = 0, ch2_data = 0;
  int32_t filtered_ch1_data = 0, filtered_ch2_data = 0, bpm_preprocessed = 0;

  /* Infinite loop */
  for(;;)
  {
    /* Get channel data */
    event_ch1 = osMessageGet(queue_ecg_afe_ch_1Handle, osWaitForever);
    event_ch2 = osMessageGet(queue_ecg_afe_ch_2Handle, osWaitForever);
    if ((event_ch1.status == osEventMessage) && (event_ch2.status == osEventMessage))
    {
      /* Retrieve values */
      ch1_data = (int32_t) event_ch1.value.v;
      ch2_data = (int32_t) event_ch2.value.v;

      /* Filter channel 1 */
      filtered_ch1_data = show_filter(ch1_data);
      /* Filter channel 2 */
      filtered_ch2_data = show_filter(ch2_data);
      /* Filter channel 1 for bpm detection */
      bpm_preprocessed = bpm_preprocessing(ch1_data);

      /* Output data to queues */
      osMessagePut(queue_ecg_filter_ch_1Handle, filtered_ch1_data, 0);
      osMessagePut(queue_ecg_filter_ch_2Handle, filtered_ch2_data, 0);
      osMessagePut(queue_ecg_bpmHandle, bpm_preprocessed, 0);
    }
  }
}

void Start_ecg_leadGenTask(void const * argument)
{
  osEvent event_ch1, event_ch2;

  int32_t filtered_ch1_data = 0, filtered_ch2_data = 0;
  int32_t lead_I = 0, lead_II = 0, lead_III = 0;
  int32_t lead_aVR = 0, lead_aVL = 0, lead_aVF = 0;

  /* Infinite loop */
  for(;;)
  {
    /* Get filtered channel data */
    event_ch1 = osMessageGet(queue_ecg_filter_ch_1Handle, osWaitForever);
    event_ch2 = osMessageGet(queue_ecg_filter_ch_2Handle, osWaitForever);
    if ((event_ch1.status == osEventMessage) && (event_ch2.status == osEventMessage))
    {
      /* Retrieve values */
      filtered_ch1_data = (int32_t) event_ch1.value.v;
      filtered_ch2_data = (int32_t) event_ch2.value.v;
    }

    /* Generate lead I */
    lead_I = filtered_ch1_data;
    /* Generate lead I */
    lead_II = filtered_ch2_data;
    /* Generate lead III */
    lead_III = lead_II - lead_I;
    /* Generate lead aVR */
    lead_aVR = (lead_I + lead_II)   >> 1;
    /* Generate lead aVL */
    lead_aVL = (lead_I - lead_III)  >> 1;
    /* Generate lead aVF */
    lead_aVF = (lead_II + lead_III) >> 1;

    /* Output data to queues */
    osMessagePut(queue_ecg_lead_IHandle,   lead_I,   0);
    osMessagePut(queue_ecg_lead_IIHandle,  lead_II,  0);
    osMessagePut(queue_ecg_lead_IIIHandle, lead_III, 0);
    osMessagePut(queue_ecg_lead_aVRHandle, lead_aVR, 0);
    osMessagePut(queue_ecg_lead_aVLHandle, lead_aVL, 0);
    osMessagePut(queue_ecg_lead_aVFHandle, lead_aVF, 0);
  }
}

void Start_ecg_bpmDetTask(void const * argument)
{
  osEvent event;
  int32_t ecg_lead = 0;

  static int32_t threshold = 100, maxerino = 0, prev_value = 0;
  static uint32_t maxerino_pos = 0, sample_counter = 0;
  float bpm = 0;

  /* Infinite loop */
  for(;;)
  {
    /* Get lead I data (or any other lead) */
    event = osMessageGet(queue_ecg_bpmHandle, osWaitForever);
    if (event.status == osEventMessage)
    {
      /* Retrieve value */
      ecg_lead = (int32_t) event.value.v;
    }

    /* If signal crosses certain threshold */
    if (ecg_lead > threshold)
    {
      HAL_GPIO_WritePin(GPIOC, UI_LED_G_Pin, GPIO_PIN_SET);
    }
    else
    {
      HAL_GPIO_WritePin(GPIOC, UI_LED_G_Pin, GPIO_PIN_RESET);
    }

    /* BPM detection in progress */
    if(ecg_lead > threshold)
    {
      if (ecg_lead > maxerino)
      {
        maxerino =         ecg_lead;
        maxerino_pos =     sample_counter;
      }
    }
    else if (prev_value > threshold)
    {
      if( maxerino_pos > (FS >> 2)) /* this limits bpm <= 240*/
      {
        bpm =              ((60 * FS) << 10 ) / maxerino_pos;
        sample_counter =   sample_counter - maxerino_pos - 1;
        threshold =        (threshold + maxerino) >> 1;
        /* Output data to queue */
        osMessagePut(queue_ecg_bpmHandle, (uint32_t) bpm, 0);
      }
      maxerino =         0;
    }
    else if (threshold > 100) /* Minimum threshold */
    {
      threshold = ((threshold*511)>>9);
    }

    sample_counter++;
    prev_value = ecg_lead;
  }
}

void Start_ecg_keyGenTask(void const * argument)
{
  osEvent event;

  uint32_t bpm = 0;
  /* Infinite loop */
  for(;;)
  {
    /* Get bpm data */
    event = osMessageGet(queue_ecg_bpmHandle, osWaitForever);
    if (event.status == osEventMessage)
    {
      /* Retrieve value */
      bpm = (uint32_t) event.value.v;
      // Esto lo pongo para que el compilador deje de joder
      // se puede quitar a cholon
      UNUSED(bpm);
    }

    /* Security magic */

  }
}