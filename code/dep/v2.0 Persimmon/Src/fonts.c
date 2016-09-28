/*
 * font.c
 *
 *  Created on: 29/9/2015
 *      Author: slopez
 */

#include "fonts.h"

uint16_t font_get_char(font_t* current_font, char character)
{
  uint8_t  character_number;
  uint16_t character_position;

  /* Get character number (substract ASCII offset) */
  character_number   = ((uint8_t) character) - current_font->starting_character;
  /* Get character starting position in font array */
  character_position = ((uint16_t) character_number) * current_font->bytes_per_character;
  
  /* Check if requested char doesn't exist in array */
  if (character_number > current_font->character_num)
  {
    /* If character exists, return its position in the array */
    character_position = 31*current_font->bytes_per_character;
  }

  return character_position;
}