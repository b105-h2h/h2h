/*
 * tasks_periph.h
 */

#ifndef TASKS_PERIPH_H_
#define TASKS_PERIPH_H_

#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

#include "buzzer.h"
#include "gauge.h"
#include "lcd.h"

#include "item.h"
#include "menu.h"

void tasks_periph_init();
void tasks_periph_start();

#endif /* TASKS_PERIPH_H_ */