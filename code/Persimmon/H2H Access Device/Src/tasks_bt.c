#include "tasks_bt.h"

#include "fsm_client.h"
#include "authentication.h"
#include "menu.h"

/* Queues */
osMailQId queue_bt_packet_recievedHandle;
osMailQId queue_bt_packet_sendHandle;
extern osMailQId queue_ecg_keyHandle;
extern osMailQId queue_fsm_eventsHandle;
extern osMailQId queue_lcdHandle;
extern osMailQId queue_ecg_keyfsmHandle;
/* Tasks */
osThreadId bt_txTaskHandle;
osThreadId bt_rxTaskHandle;
void Start_bt_txTask(void const * argument);
void Start_bt_rxTask(void const * argument);

/* Objects */

void tasks_bt_init()
{
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
  osThreadDef(bt_txTask, Start_bt_txTask, osPriorityHigh, 0, 240);
  bt_txTaskHandle = osThreadCreate(osThread(bt_txTask), NULL);

  /* bt_rxTask */
  osThreadDef(bt_rxTask, Start_bt_rxTask, osPriorityLow, 0, 240);
  bt_rxTaskHandle = osThreadCreate(osThread(bt_rxTask), NULL);
}

void Start_bt_txTask(void const * argument)
{
  osEvent event_pk_to_send;
  bt_packet_t send_packet = {.packet_content = {0}};;

  /* Infinite loop */
  for(;;)
  {
    event_pk_to_send = osMailGet(queue_bt_packet_sendHandle, osWaitForever);
    if (event_pk_to_send.status == osEventMail)
    {
      send_packet = *((bt_packet_t *) event_pk_to_send.value.p);
      SendData(PACKET_SIZE, send_packet.packet_content);
    }
  }
}

void Start_bt_rxTask(void const * argument)
{
  osEvent event_pk_rec;
  bt_packet_t rec_packet;
  char command_str[24] = "";
  fsm_event_f bt_event;
  validation_key_t rec_key;
  
  init_key(&rec_key,EXTERN);
  
  /* Infinite loop */
  for(;;)
  {
    event_pk_rec = osMailGet(queue_bt_packet_recievedHandle, osWaitForever);
    if (event_pk_rec.status == osEventMail)
    {
      rec_packet = *((bt_packet_t *) event_pk_rec.value.p);
      /* Command */
      bt_event = fsm_no_event;
      if(!rec_packet.packet_content[0])
      {
        sprintf(command_str, "%s", &rec_packet.packet_content[8]);
        if(!strcmp(command_str,gen_init))
        {
          bt_event = fsm_h2h_start_gen;
          while(osMailPut(queue_fsm_eventsHandle, (void *) &bt_event) != osOK)
          {
            osDelay(1);
          }
        } 
        else if(!strcmp(command_str,access_ok))
        {
          item_area_set_text(&menu_h2h_devices.items[6].item.area,"Access granted");
          while (osMailPut(queue_lcdHandle, (void *) &menu_h2h_devices.items[6]) != osOK)
          {
            osDelay(1);
          }  
        }
        else if(!strcmp(command_str,access_denied))
        {
          item_area_set_text(&menu_h2h_devices.items[6].item.area,"Access denied");
          while (osMailPut(queue_lcdHandle, (void *) &menu_h2h_devices.items[6]) != osOK)
          {
            osDelay(1);
          }  
        }
      }
      else if(rec_packet.packet_content[0]) /* Key */
      {
        sprintf((char *)&(rec_key.token),"%s",&rec_packet.packet_content[8]);
        rec_key.state = READY;
        while (osMailPut(queue_ecg_keyfsmHandle, (void *) &rec_key) != osOK)
        {
          osDelay(1);
        }
        /* signal key have been recieved*/
        bt_event = fsm_h2h_ok;
        while(osMailPut(queue_fsm_eventsHandle, (void *) &bt_event) != osOK)
        {
          osDelay(1);
        }
      }
      
    }
  }
}

