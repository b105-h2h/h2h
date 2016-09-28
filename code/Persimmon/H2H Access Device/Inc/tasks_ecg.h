#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

#include "afe.h"
#include "dsp.h"
#include "authentication.h"

#include "leads.h"

#define FS 500

void tasks_ecg_init();
void tasks_ecg_start();