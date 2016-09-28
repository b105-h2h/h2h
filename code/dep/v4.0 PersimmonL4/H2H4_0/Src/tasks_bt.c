#include "tasks_bt.h"

/* This will be probably moved to hacitrans */
/* Semaphores */
osSemaphoreId sem_bt_conectedHandle;

/* Queues */
osMailQId queue_bt_packet_recievedHandle;
osMailQId queue_bt_packet_sendHandle;

/* Tasks */
osThreadId bt_txTaskHandle;
osThreadId bt_rxTaskHandle;
void Start_bt_txTask(void const * argument);
void Start_bt_rxTask(void const * argument);

/* Objects */

void tasks_bt_init()
{
  /* Semaphores */
  osSemaphoreDef(sem_bt_conected);
  sem_bt_conectedHandle = osSemaphoreCreate(osSemaphore(sem_bt_conected), 1);

  /* Queues */
  osMailQDef(queue_bt_packet_recieve, 4, bt_packet_t);
  queue_bt_packet_recievedHandle = osMailCreate(osMailQ(queue_bt_packet_recieve), NULL);

  osMailQDef(queue_bt_packet_send, 4, bt_packet_t);
  queue_bt_packet_sendHandle = osMailCreate(osMailQ(queue_bt_packet_send), NULL);
}

void tasks_bt_start()
{
  /* Tasks */
  /* bt_txTask */
  osThreadDef(bt_txTask, Start_bt_txTask, osPriorityHigh, 0, 256);
  bt_txTaskHandle = osThreadCreate(osThread(bt_txTask), NULL);

  /* bt_rxTask */
  osThreadDef(bt_rxTask, Start_bt_rxTask, osPriorityHigh, 0, 256);
  bt_rxTaskHandle = osThreadCreate(osThread(bt_rxTask), NULL);
}

void Start_bt_txTask(void const * argument)
{
  osEvent event_pk_to_send;
  bt_packet_t* send_packet;

  /* Infinite loop */
  for(;;)
  {
    event_pk_to_send = osMailGet(queue_bt_packet_sendHandle, osWaitForever);
    if (event_pk_to_send.status == osEventMail)
    {
      send_packet = (bt_packet_t *) event_pk_to_send.value.p;
      SendData(PACKET_SIZE, send_packet->packet_content);
      osDelay(1);
    }
  }
}

void Start_bt_rxTask(void const * argument)
{
  osEvent event_pk_rec;
  bt_packet_t rec_packet;

  /* Infinite loop */
  for(;;)
  {
    event_pk_rec = osMailGet(queue_bt_packet_recievedHandle, osWaitForever);
    if (event_pk_rec.status == osEventMail)
    {
      rec_packet = *((bt_packet_t *) event_pk_rec.value.p);
      osMailPut(queue_bt_packet_sendHandle, (void *) &rec_packet);
      osDelay(1);
    }
  }
}

