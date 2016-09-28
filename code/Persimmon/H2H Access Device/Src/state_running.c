#include "state_running.h"

/* Possible transition to the following states */
#include "state_goodbye.h"

/* Child states */
#include "state_main.h"
#include "state_ecg.h"
#include "state_h2h.h"
#include "state_settings.h"

/* State includes */
#include "cmsis_os.h"
#include "menu.h"
#include "buzzer.h"

/* Mutexes */
extern osMutexId mutex_menuHandle;

/* Queues */
extern osMailQId queue_periph_buzzerHandle;

static void running_action_button_short(state_ptr state)
{
  buzzer_note_t beep;

  /* Do transition actions */

  /* Beep */
  beep.note = A6;
  beep.ms = 100;
  while(osMailPut(queue_periph_buzzerHandle, (void *) &beep) != osOK)
  {
    osDelay(1);
  }

  //TO-DO: Disable clicks and screen

//  /* If lcd is on */
//  if (is_lcd_on)
//  {
//    /* Turn it off */
//    lcd_config.item.config.backlight_level = 0;
//    osMailPut(queue_lcdHandle, (void *) &lcd_config);
//
//    is_lcd_on = 0;
//  }
//  /* If it is off */
//  else
//  {
//    /* Turn it on, turn it on again */
//    lcd_config.item.config.backlight_level = lcd.backlight_level;
//    osMailPut(queue_lcdHandle, (void *) &lcd_config);
//
//    is_lcd_on = 1;
//  }
}

static void running_to_goodbye(state_ptr state)
{
  /* Do transition actions */

  /* Change state */
  entry_to_goodbye(state);
}

/* State behaviour */
void behaviour_running(state_ptr state)
{
  /* Set events to react to */
  state->button_short = running_action_button_short;
  state->button_long = running_to_goodbye;

  /* Do state actions */
}

/* Entry point to the state */
void entry_to_running(state_ptr state)
{
  /* Set state name */
  strcpy(state->name, "running");

  /* Go to child default state */
  entry_to_main(state);
}