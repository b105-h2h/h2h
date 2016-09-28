/*
 * font.h
 *
 *  Created on: 29/9/2015
 *      Author: slopez
 */

#ifndef LCD_FONT_H_
#define LCD_FONT_H_

#include <stdint.h>

/*
 * Generic font type
 */
typedef struct font_t
{
  uint8_t font_width;
  uint8_t font_height;
  uint8_t starting_character;
  uint8_t character_num;

  uint8_t bytes_per_character;
  uint8_t font[];
}
font_t;

uint16_t font_get_char(font_t* current_font, char character);

#endif /* LCD_FONT_H_ */
