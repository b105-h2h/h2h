/*
 * font.h
 *
 *  Created on: 29/9/2015
 *      Author: slopez
 */

#ifndef DISPLAY_DISPLAY_SRC_FONT_H_
#define DISPLAY_DISPLAY_SRC_FONT_H_

#include <msp430.h>
#include <stdint.h>

//Font constants
#define FONT_HEIGHT			16
#define FONT_WIDTH			16
#define STARTING_CHAR		0x20
#define CHAR_NUM			95

#define BYTES_PER_CHAR		FONT_HEIGHT * (FONT_WIDTH/8)
#define FONT_ARRAY_SIZE		BYTES_PER_CHAR * CHAR_NUM

/*
 * Generic font type
 */
typedef const struct font_t
{
	const uint8_t font_height;
	const uint8_t font_width;
	const uint8_t starting_character;
	const uint8_t character_num;

	const uint8_t bytes_per_character;
	const uint8_t font_array[FONT_ARRAY_SIZE];
}
font_t;

uint8_t*	font_get_char(const font_t* current_font, char character);

#endif /* DISPLAY_DISPLAY_SRC_FONT_H_ */
