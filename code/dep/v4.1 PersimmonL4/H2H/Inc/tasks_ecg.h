/*
 * tasks_ecg.h
 */

#ifndef TASKS_ECG_H_
#define TASKS_ECG_H_

#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

#include "afe.h"
#include "dsp.h"

void tasks_ecg_init();
void tasks_ecg_start();

#endif /* TASKS_ECG_H_ */