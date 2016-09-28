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