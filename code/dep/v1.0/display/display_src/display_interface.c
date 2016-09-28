/*
 * display_interface.c
 *
 *  Created on: 29/9/2015
 *      Author: slopez
 */

#include "display_interface.h"

/*
 * Window drawing functions
 */
static void display_interface_draw_window(color_t color, uint16_t line_origin, uint16_t line_end)
{
	//Set origin line
	display_hal_set_GRAM_position(0, line_origin);

	uint32_t i;
	uint32_t last_pixel = DISPLAY_COLUMNS * ((uint32_t)(line_end - line_origin));

	for (i = last_pixel; i > 0; i--)
	{
		display_hal_write_GRAM(color);
	}
}

static void display_interface_windows_setup(display_interface_t* display_interface, color_t signal_window_bg_color,
																					color_t menubar_window_bg_color)
{
	//Store window colors
		display_interface->signal_window_bg_color = signal_window_bg_color;
		display_interface->menubar_window_bg_color = menubar_window_bg_color;

	//Store window limits
		//Signal window
		display_interface->signal_window_line_origin = 0;
		display_interface->signal_window_line_end = SIGNAL_LINES;

		//Menubar window
		display_interface->menubar_window_line_origin = SIGNAL_LINES;
		display_interface->menubar_window_line_end = DISPLAY_LINES;

	//Write them to GRAM
		display_interface_draw_window(signal_window_bg_color, 0, SIGNAL_LINES);					//Set signal window
		display_interface_draw_window(menubar_window_bg_color, SIGNAL_LINES, DISPLAY_LINES);	//Set menubar window
}

//--------------------------------------------------

/*
 * ECG signal index handlers
 */
static uint16_t display_interface_get_index(display_interface_t* display_interface, uint16_t offset)
{
	uint16_t index;

	index = (display_interface->signal_index + offset) % SIGNAL_COLUMNS;
	return index;
}

static void display_interface_set_index(display_interface_t* display_interface, uint16_t index)
{
	display_interface->signal_index = display_interface->signal_index + index;
	display_interface->signal_index = display_interface->signal_index % SIGNAL_COLUMNS;
}

static void display_interface_inc_index(display_interface_t* display_interface)
{
	display_interface->signal_index++;
	display_interface->signal_index = display_interface->signal_index % SIGNAL_COLUMNS;
}

//--------------------------------------------------

/*
 * Display interface functions
 */
void display_interface_init(display_interface_t* display_interface, color_t signal_window_bg_color,
																	 color_t menubar_window_bg_color)
{
	//Set signal index to 0
		display_interface_set_index(display_interface, 0);
	//Set windows
		display_interface_windows_setup(display_interface, signal_window_bg_color, menubar_window_bg_color);
}

void display_interface_write_signal(display_interface_t* display_interface, ecg_data_t* signal_data, color_t color)
{
		static uint16_t last_signal_y_point = SIGNAL_OFFSET;
		uint16_t current_index, clear_index, signal_y_point;
		int16_t signal_value;
		int i;

	//Clear screen
		current_index = display_interface_get_index(display_interface, 0);
		clear_index = display_interface_get_index(display_interface, CLEAR_DISTANCE);

		for (i = SIGNAL_LINES; i > 0; i--)
		{
			//Clear current column
				display_functions_write_pixel(display_interface->signal_window_bg_color, current_index, SIGNAL_LINES - i);
			//Clear column CLEAR_DISTANCE pixels ahead signal
				display_functions_write_pixel(display_interface->signal_window_bg_color, clear_index, SIGNAL_LINES - i);
		}

	//Cook ecg data
		signal_value = signal_data->data >> 4;

	//Limit ecg data values
		if (signal_value > (SIGNAL_LINES >> 1))			//If value goes below screen bottom limit
		{
			signal_y_point = 0;
		}
		else if (signal_value < (- SIGNAL_LINES >> 1))	//If value goes above screen top limit
		{
			signal_y_point = SIGNAL_LINES - 1;
		}
		else
		{
			signal_y_point = (SIGNAL_LINES >> 1) - signal_value;
		}

	//Print data
		display_functions_write_line(color, current_index - 1, last_signal_y_point,
						  					current_index, (uint16_t)signal_y_point);

	//Increment index
		display_interface_inc_index(display_interface);
		last_signal_y_point = (uint16_t)signal_y_point;
}
