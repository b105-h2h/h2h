#include "item_area.h"

void item_area_init(item_area_t *area,
                    uint16_t width, uint16_t height,
                    uint16_t x_pos, uint16_t y_pos,
                    gui_border_t border,
                    gui_active_t is_active,
                    wchar_t *string, const uint8_t *font,
                    gui_h_align_t string_h_align, gui_v_align_t string_v_align,
                    color_t text_color, color_t bg_color, color_t border_color,
                    fsm_event_f event)
{
  /* Set area size */
  area->width = width;
  area->height = height;

  /* Set area position */
  area->pos.x_pos = x_pos;
  area->pos.y_pos = y_pos;

  /* Set area border */
  area->border = border;

  /* Set area text */
  area->font = font;
  item_area_set_text_16(area, string);

  /* Set text alignment */
  area->string_h_align = string_h_align;
  area->string_v_align = string_v_align;

  /* Set behaviour default parameters */
  area->is_active = is_active;

  /* Set colors */
  area->text_color = text_color;
  area->bg_color = bg_color;
  area->border_color = border_color;

  /* Set triggered event */
  area->event = event;
}

void item_area_set_text(item_area_t *area, char *string)
{
  char character = 0;                   // Iterating char
  uint32_t character_index = 0;         // String iteration

  /* Calculate string width */
  while ((character = string[character_index]) != 0)
  {
    /* If string still fits into area
     * store character in area->string
     */
    if (character_index < MAX_STRING_LENGTH - 1)
    {
      /* If there is enough space to keep copying the string */
      area->string[character_index] = (wchar_t) character;
    }
    else
    {
        /* Close string */
        area->string[character_index] = 0;
        return;
    }
    /* Next character */
    character_index++;
  }

  /* Close string */
  area->string[character_index] = 0;
}

void item_area_set_text_16(item_area_t *area, wchar_t *string)
{
  wchar_t character = 0;                // Iterating char
  uint32_t character_index = 0;         // String iteration

  /* Calculate string width */
  while ((character = string[character_index]) != 0)
  {
    /* If string still fits into area
     * store character in area->string
     */
    if (character_index < MAX_STRING_LENGTH - 1)
    {
      /* If there is enough space to keep copying the string */
      area->string[character_index] = character;
    }
    else
    {
        /* Close string */
        area->string[character_index] = 0;
        return;
    }
    /* Next character */
    character_index++;
  }

  /* Close string */
  area->string[character_index] = 0;
}