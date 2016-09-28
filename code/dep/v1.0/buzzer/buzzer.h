/*
 * buzzer_module.h
 *
 *  Created on: 29/7/2015
 *      Author: slopez
 */

#ifndef BUZZER_BUZZER_H_
#define BUZZER_BUZZER_H_

#include <msp430.h>
#include <stdint.h>

#include "buzzer_src/buzzer_notes.h"
#include "../utils.h"

typedef struct buzzer_t
{

}
buzzer_t;

void	buzzer_setup(buzzer_t* buzzer);
void	buzzer_play(buzzer_t* buzzer, int note, int ms);

#endif /* BUZZER_BUZZER_H_ */
