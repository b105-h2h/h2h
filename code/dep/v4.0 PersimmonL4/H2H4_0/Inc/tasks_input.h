#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

#include "buzzer.h"
#include "touch.h"

#include "click.h"
#include "menu.h"

void tasks_input_init();
void tasks_input_start();