#include "tasks_ecg.h"

#include "fsm_client.h"
#include "bluetooth_internal.h"
/* Mutexes */
osMutexId mutex_ecg_leadsHandle;

/* Semaphores */
osSemaphoreId sem_ecg_afe_drdyHandle;
osSemaphoreId sem_ecg_afe_dma_rxHandle;
osSemaphoreId sem_ecg_keygenHandle;

/* Queues */
osMessageQId queue_ecg_afe_ch_1Handle;
osMessageQId queue_ecg_afe_ch_2Handle;
osMessageQId queue_ecg_preprocessedHandle;
osMessageQId queue_ecg_lead_IHandle;
osMessageQId queue_ecg_lead_IIHandle;
osMessageQId queue_ecg_lead_IIIHandle;
osMessageQId queue_ecg_lead_aVRHandle;
osMessageQId queue_ecg_lead_aVLHandle;
osMessageQId queue_ecg_lead_aVFHandle;
osMessageQId queue_ecg_bpmHandle;
osMessageQId queue_ecg_bpm_screenHandle;
osMailQId queue_ecg_keyHandle;
osMailQId queue_ecg_keybtHandle;
extern osMailQId queue_fsm_eventsHandle;
extern osMailQId queue_bt_packet_sendHandle;

/* Tasks */
osThreadId ecg_afeTaskHandle;
osThreadId ecg_filterTaskHandle;
osThreadId ecg_bpmDetTaskHandle;
osThreadId ecg_keyGenTaskHandle;
osThreadId ecg_validationTaskHandle;

void Start_ecg_afeTask(void const * argument);
void Start_ecg_filterTask(void const * argument);
void Start_ecg_bpmDetTask(void const * argument);
void Start_ecg_keyGenTask(void const * argument);
void Start_ecg_validationTask(void const * argument);

/* Objects */
extern afe_t afe;
leads_t leads;

void tasks_ecg_init()
{
  /* Mutexes */
  osMutexDef(mutex_ecg_leads);
  mutex_ecg_leadsHandle = osMutexCreate(osMutex(mutex_ecg_leads));

  /* Semaphores */
  /* sem_ecg_afe_drdy */
  osSemaphoreDef(sem_ecg_afe_drdy);
  sem_ecg_afe_drdyHandle = osSemaphoreCreate(osSemaphore(sem_ecg_afe_drdy), 1);

  /* sem_ecg_afe_dma_rx */
  osSemaphoreDef(sem_ecg_afe_dma_rx);
  sem_ecg_afe_dma_rxHandle = osSemaphoreCreate(osSemaphore(sem_ecg_afe_dma_rx), 1);

  /* sem_ecg_afe_dma_rx */
  osSemaphoreDef(sem_ecg_keygen);
  sem_ecg_keygenHandle = osSemaphoreCreate(osSemaphore(sem_ecg_keygen), 1);

  /* Queues */
  /* queue_ecg_afe_ch_1 */
  osMessageQDef(queue_ecg_afe_ch_1, 2, int32_t);
  queue_ecg_afe_ch_1Handle = osMessageCreate(osMessageQ(queue_ecg_afe_ch_1), NULL);

  /* queue_ecg_afe_ch_2 */
  osMessageQDef(queue_ecg_afe_ch_2, 2, int32_t);
  queue_ecg_afe_ch_2Handle = osMessageCreate(osMessageQ(queue_ecg_afe_ch_2), NULL);

  /* queue_ecg_filter_ch_1 */
  osMessageQDef(queue_ecg_preprocessed, 2, int32_t);
  queue_ecg_preprocessedHandle = osMessageCreate(osMessageQ(queue_ecg_preprocessed), NULL);

  /* queue_ecg_lead_I */
  osMessageQDef(queue_ecg_lead_I, 2, int32_t);
  queue_ecg_lead_IHandle = osMessageCreate(osMessageQ(queue_ecg_lead_I), NULL);

  /* queue_ecg_lead_II */
  osMessageQDef(queue_ecg_lead_II, 2, int32_t);
  queue_ecg_lead_IIHandle = osMessageCreate(osMessageQ(queue_ecg_lead_II), NULL);

  /* queue_ecg_lead_III */
  osMessageQDef(queue_ecg_lead_III, 2, int32_t);
  queue_ecg_lead_IIIHandle = osMessageCreate(osMessageQ(queue_ecg_lead_III), NULL);

  /* queue_ecg_lead_aVR */
  osMessageQDef(queue_ecg_lead_aVR, 2, int32_t);
  queue_ecg_lead_aVRHandle = osMessageCreate(osMessageQ(queue_ecg_lead_aVR), NULL);

  /* queue_ecg_lead_aVL */
  osMessageQDef(queue_ecg_lead_aVL, 2, int32_t);
  queue_ecg_lead_aVLHandle = osMessageCreate(osMessageQ(queue_ecg_lead_aVL), NULL);

  /* queue_ecg_lead_aVF */
  osMessageQDef(queue_ecg_lead_aVF, 2, int32_t);
  queue_ecg_lead_aVFHandle = osMessageCreate(osMessageQ(queue_ecg_lead_aVF), NULL);

  /* queue_ecg_bpm */
  osMessageQDef(queue_ecg_bpm, 2, uint32_t);
  queue_ecg_bpmHandle = osMessageCreate(osMessageQ(queue_ecg_bpm), NULL);

  /* queue_ecg_bpm */
  osMessageQDef(queue_ecg_bpm_screen, 2, uint32_t);
  queue_ecg_bpm_screenHandle = osMessageCreate(osMessageQ(queue_ecg_bpm_screen), NULL);

  /* queue_ecg_key */
  osMailQDef(queue_ecg_key, 2, validation_key_t);
  queue_ecg_keyHandle = osMailCreate(osMailQ(queue_ecg_key), NULL);
  
  /* queue_ecg_key */
  osMailQDef(queue_ecg_btkey, 2, validation_key_t);
  queue_ecg_keybtHandle = osMailCreate(osMailQ(queue_ecg_btkey), NULL);
}

void tasks_ecg_start()
{
  /* Tasks */
  /* ecg_afeTask */
  osThreadDef(ecg_afeTask, Start_ecg_afeTask, osPriorityRealtime, 0, 64);
  ecg_afeTaskHandle = osThreadCreate(osThread(ecg_afeTask), NULL);

  /* ecg_filterTask */
  osThreadDef(ecg_filterTask, Start_ecg_filterTask, osPriorityNormal, 0, 100);
  ecg_filterTaskHandle = osThreadCreate(osThread(ecg_filterTask), NULL);

  /* ecg_bpmDetTask */
  osThreadDef(ecg_bpmDetTask, Start_ecg_bpmDetTask, osPriorityNormal, 0, 80);
  ecg_bpmDetTaskHandle = osThreadCreate(osThread(ecg_bpmDetTask), NULL);

  /* ecg_keyGenTask */
  osThreadDef(ecg_keyGenTask, Start_ecg_keyGenTask, osPriorityNormal, 0, 80);
  ecg_keyGenTaskHandle = osThreadCreate(osThread(ecg_keyGenTask), NULL);

  /* ecg_validationTask */
  osThreadDef(ecg_validationTask, Start_ecg_validationTask, osPriorityNormal, 0, 128);
  ecg_validationTaskHandle = osThreadCreate(osThread(ecg_validationTask), NULL);
}

size_t sizerino;

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

    /* Check RAM */
    sizerino = xPortGetMinimumEverFreeHeapSize();
  }
}

void Start_ecg_filterTask(void const * argument)
{
  osEvent event_ch1, event_ch2;

  int32_t ch1_data = 0, ch2_data = 0;
  int32_t lead_I = 0, lead_II = 0, lead_III = 0;
  int32_t bpm_preprocessed = 0;

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
      lead_I = show_filter(ch1_data);
      /* Filter channel 2 */
      lead_II = show_filter(ch2_data);
      /* Generate lead III */
      lead_III = lead_II - lead_I;
      /* Filter channel 1 for bpm detection */
      bpm_preprocessed = bpm_preprocessing(ch1_data);

      /* Generate leads */
      osMutexWait(mutex_ecg_leadsHandle, osWaitForever);
      leads.lead_I   = lead_I;
      leads.lead_II  = lead_II;
      leads.lead_III = lead_III;
      leads.lead_aVR = (lead_I + lead_II)   >> 1;
      leads.lead_aVL = (lead_I - lead_III)  >> 1;
      leads.lead_aVF = (lead_II + lead_III) >> 1;
      osMutexRelease(mutex_ecg_leadsHandle);

      /* Output data to queues */
      osMessagePut(queue_ecg_preprocessedHandle, bpm_preprocessed, 0);
    }
  }
}

void Start_ecg_bpmDetTask(void const * argument)
{
  osEvent event;
  int32_t ecg_lead = 0,bpm = 0;
  int32_t threshold_high = 1, threshold_low = 1, maxerino = 0;
  int sample_counter = 0;
  uint8_t flag_qrs_zone = 0;

  /* Infinite loop */
  for(;;)
  {
    /* Get lead I data (or any other lead) */
    event = osMessageGet(queue_ecg_preprocessedHandle, osWaitForever);
    if (event.status == osEventMessage)
    {
      /* Retrieve value */
      ecg_lead = (int32_t) event.value.v;
    }
    
    /* If signal crosses certain threshold signal visually*/
    if (ecg_lead > threshold_high)
    {
      HAL_GPIO_WritePin(GPIOC, UI_LED_G_Pin, GPIO_PIN_SET);
    }
    else
    {
      HAL_GPIO_WritePin(GPIOC, UI_LED_G_Pin, GPIO_PIN_RESET);
    }
    
    /* BPM detection in progress */
    if((ecg_lead > threshold_high)&&(flag_qrs_zone == 0))
    {
      flag_qrs_zone = 1;
    }
    else if(flag_qrs_zone == 1)
    {
      if (ecg_lead <= 0)
      {
        threshold_high =   ((threshold_high + maxerino*3) >> 2);
        threshold_low =    ((threshold_low + maxerino*3) * 10)>>6;

        if( sample_counter > 100)
        {
          /* 60 * 1024 * FS / SC */
          bpm = ((61440 * FS ) / sample_counter);
          osMessagePut(queue_ecg_bpmHandle, (uint32_t) sample_counter, 0);
          osMessagePut(queue_ecg_bpm_screenHandle, (uint32_t) bpm, 0);
          sample_counter = -1;
          maxerino = 0;
          flag_qrs_zone = 0;
        }
      }
    }
    else if (threshold_high > threshold_low)
    {
      threshold_high = threshold_high *255/256;
    }
    if (ecg_lead > maxerino)
    {
      maxerino = ecg_lead;
    }
    sample_counter++;
  }
}

void Start_ecg_keyGenTask(void const * argument)
{
  osEvent event;
  validation_key_t key;
  uint32_t bpm = 0;
  fsm_event_f key_event;
  init_key(&key,INTERN);
  osSemaphoreWait(sem_ecg_keygenHandle, osWaitForever);
  /* Infinite loop */
  for(;;)
  {
    /* Get bpm data */
    event = osMessageGet(queue_ecg_bpmHandle, osWaitForever);
    if (event.status == osEventMessage)
    {
      /* Retrieve value */
      bpm = (uint32_t) event.value.v;
      if (osSemaphoreWait(sem_ecg_keygenHandle, 0) == osOK)
      {
        if(write_key(bpm, &key) != READY)
        {
          /* Activates itselft untill the key is ready */
          osSemaphoreRelease(sem_ecg_keygenHandle);
        }
        else
        {
          while (osMailPut(queue_ecg_keyHandle, (void *) &key) != osOK)
          {
            osDelay(1);
          }       
          while (osMailPut(queue_ecg_keybtHandle, (void *) &key) != osOK)
          {
            osDelay(1);
          }    
          key_event = fsm_h2h_pass_ready;
          while(osMailPut(queue_fsm_eventsHandle, (void *) &key_event) != osOK)
          {
            osDelay(1);
          }
          key.index = 0;
        }
      }
    }
  }
}

void Start_ecg_validationTask(void const * argument)
{
  osEvent event;
  validation_key_t inter_key, extern_key, incomming_key;
  autentitication_t auth_reponse;
  bt_packet_t fsm_send_packet = {.packet_content = {0}};
  fsm_event_f valid_event;
    
  init_key(&inter_key,INTERN);
  init_key(&extern_key,EXTERN);
  init_key(&incomming_key,INTERN);

  /* Infinite loop */
  for(;;)
  {
    event = osMailGet(queue_ecg_keyHandle, osWaitForever);
    if (event.status == osEventMail)
    {
      incomming_key = *((validation_key_t*) event.value.v);
      if(incomming_key.origin == INTERN)
      {
        inter_key = incomming_key;
      }
      else
      {
        extern_key = incomming_key;
      }

      if((inter_key.state == READY)&&(extern_key.state == READY))
      {
        auth_reponse = validate(&inter_key,&extern_key);
        if ( auth_reponse == ACCEPTED)
        {
          sprintf(&fsm_send_packet.packet_content[8],"%s",access_ok);
        }
        else
        {
          sprintf(&fsm_send_packet.packet_content[8],"%s",access_denied);
        }
        osMailPut(queue_bt_packet_sendHandle, (void *) &fsm_send_packet);
        erase_key(&inter_key);
        erase_key(&extern_key);
        valid_event = fsm_h2h_ok;
        while(osMailPut(queue_fsm_eventsHandle, (void *) &valid_event) != osOK)
        {
          osDelay(1);
        }
      }
    }
  }
}