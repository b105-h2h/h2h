/*
 * display_interface.h
 *
 *  Created on: 29/9/2015
 *      Author: slopez
 */

#ifndef DISPLAY_DISPLAY_SRC_DISPLAY_INTERFACE_H_
#define DISPLAY_DISPLAY_SRC_DISPLAY_INTERFACE_H_

#include <msp430.h>
#include <stdint.h>

#include "display_hal.h"
#include "display_functions.h"
#include "color.h"
#include "../../utils.h"

#include "../../data/ecg_data.h"

#define SIGNAL_OFFSET	0x0057

#define DISPLAY_LINES	240
#define DISPLAY_COLUMNS	320

#define SIGNAL_LINES	176
#define SIGNAL_COLUMNS	320

#define CLEAR_DISTANCE  25

typedef struct display_interface_t
{
	uint16_t signal_index;	//ECG signal sweep index

	//Windows
		//Signal window
		uint16_t signal_window_line_origin;
		uint16_t signal_window_line_end;

		//Menubar window
		uint16_t menubar_window_line_origin;
		uint16_t menubar_window_line_end;

		//Background colors
		color_t signal_window_bg_color;
		color_t menubar_window_bg_color;
}
display_interface_t;

void	display_interface_init(display_interface_t* display_interface, color_t signal_window_bg_color,
																	   color_t menubar_window_bg_color);
void	display_interface_write_signal(display_interface_t* display_interface, ecg_data_t* signal_data,
																			   color_t color);

#endif /* DISPLAY_DISPLAY_SRC_DISPLAY_INTERFACE_H_ */
