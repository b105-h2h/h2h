#include "text_area.h"

gui_status_t text_area_init(text_area_t *text_area,
                            uint16_t width, uint16_t height,
                            const uint8_t *font, char *string,
                            align_h_t text_area_h_align, align_v_t text_area_v_align,
                            align_h_t text_h_align, align_v_t text_v_align)
{
  gui_status_t status;

  text_area_set_width(text_area, width);  
  text_area_set_height(text_area, height);
  text_area_set_font(text_area, font);
  
  status = text_area_set_string(text_area, string);
  
  text_area_set_text_area_h_align(text_area, text_area_h_align);
  text_area_set_text_area_v_align(text_area, text_area_v_align);
  text_area_set_text_h_align(text_area, text_h_align);
  text_area_set_text_v_align(text_area, text_v_align);
  
  return status;
}

/*
 *  Getters
 *
 */
uint16_t text_area_get_width(text_area_t *text_area)
{
  return text_area->width;
}

uint16_t text_area_get_height(text_area_t *text_area)
{
  return text_area->height;
}

align_h_t text_area_get_text_area_h_align(text_area_t *text_area)
{
  return text_area->text_area_h_align;
}

align_v_t text_area_get_text_area_v_align(text_area_t *text_area)
{
  return text_area->text_area_v_align;
}

align_h_t text_area_get_text_h_align(text_area_t *text_area)
{
  return text_area->text_h_align;
}

align_v_t text_area_get_text_v_align(text_area_t *text_area)
{
  return text_area->text_v_align;
}

void text_area_get_font(text_area_t *text_area, const uint8_t *font)
{
  font = text_area->font;
}

void text_area_get_string(text_area_t *text_area, char *string)
{
  string = text_area->string;
}


/*
 *  Setters
 *
 */
void text_area_set_width(text_area_t *text_area, uint16_t width)
{
  text_area->width = width;
}

void text_area_set_height(text_area_t *text_area, uint16_t height)
{
  text_area->height = height;
}

void text_area_set_text_area_h_align(text_area_t *text_area, align_h_t text_area_h_align)
{
  text_area->text_area_h_align = text_area_h_align;
}

void text_area_set_text_area_v_align(text_area_t *text_area, align_v_t text_area_v_align)
{
  text_area->text_area_v_align = text_area_v_align;
}

void text_area_set_text_h_align(text_area_t *text_area, align_h_t text_h_align)
{
  text_area->text_h_align = text_h_align;
}

void text_area_set_text_v_align(text_area_t *text_area, align_v_t text_v_align)
{
  text_area->text_v_align = text_v_align;
}

void text_area_set_font(text_area_t *text_area, const uint8_t *font)
{
  text_area->font = font;
}

gui_status_t text_area_set_string(text_area_t *text_area, char *string)
{
  uint8_t font_starting_char;           // Font array starting character (character number == 0)

  char character;
  uint32_t character_index = 0;         // String iteration
  uint8_t character_number;             // Character number in font array
  uint16_t character_width = 0;         // Width for every character

  uint16_t string_height = 0;
  uint16_t string_width = 0;
  
  /* Calculate string height */
  string_height = text_area->font[6];  
  /* Check text_area height */
  if (text_area->height < string_height)
  {
    return GUI_SIZE_ERROR;
  }
  
  /* Get starting character in font array */
  font_starting_char = text_area->font[2];
  /* Calculate string width */
  while ((character = string[character_index++]) != 0)
  {
    /* Get character number */
    character_number = character - font_starting_char;          

    /* Get character width */
    character_width = text_area->font[8 + 4*character_number];
    
    /* Add character width to the accumulated string width */
    string_width += character_width;
  }
  
  /* Check text_area width */
  if (text_area->width < string_width)
  {
    return GUI_SIZE_ERROR;
  }
    
  /* If height and width are OK, set string */
  character_index = 0;
  while ((character = string[character_index]) != 0)
  {
    text_area->string[character_index++] = character;
  }
  text_area->string[character_index] = 0;
  
  return GUI_OK;
}