/*
 * item_area.h
 */

#ifndef ITEM_AREA_H_
#define ITEM_AREA_H_

#include <stdlib.h>

#include "item_gui.h"

#include "lcd_position.h"
#include "color.h"

#include "fsm_client.h"

#define MAX_STRING_LENGTH 50

typedef struct item_area_t
{
  uint16_t width;
  uint16_t height;

  lcd_pos_t pos;

  gui_border_t border;
  gui_active_t is_active;

  wchar_t string[MAX_STRING_LENGTH];
  const uint8_t *font;

  gui_h_align_t string_h_align;
  gui_v_align_t string_v_align;

  color_t text_color;
  color_t bg_color;
  color_t border_color;

  fsm_event_f event;
}
item_area_t;

void item_area_init(item_area_t *area,
                    uint16_t width, uint16_t height,
                    uint16_t x_pos, uint16_t y_pos,
                    gui_border_t border,
                    gui_active_t is_active,
                    wchar_t *string, const uint8_t *font,
                    gui_h_align_t string_h_align, gui_v_align_t string_v_align,
                    color_t text_color, color_t bg_color, color_t border_color,
                    fsm_event_f event);

void item_area_set_text(item_area_t *area, char *string);
void item_area_set_text_16(item_area_t *area, wchar_t *string);

#endif /* ITEM_AREA_H_ */