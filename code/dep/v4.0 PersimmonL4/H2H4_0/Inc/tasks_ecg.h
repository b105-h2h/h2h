#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

#include "afe.h"
#include "dsp.h"

#define FS 1000

void tasks_ecg_init();
void tasks_ecg_start();