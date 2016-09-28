/*
 * lcd_area.h
 *
 *  Created on: 27/05/2016
 *      Author: slopez
 */

#ifndef LCD_AREA_H_
#define LCD_AREA_H_

#include "lcd_items_utils.h"

#include "lcd_position.h"
#include "color.h"

typedef struct lcd_area_t
{
  uint16_t width;
  uint16_t height;

  lcd_pos_t pos;

  gui_border_t border;
  gui_active_t is_active;
  gui_clicked_t is_clicked;

  char string[50];
  const uint8_t *font;

  uint16_t string_width;
  uint16_t string_height;

  h_align_t string_h_align;
  v_align_t string_v_align;

  color_t text_color;
  color_t bg_color;
  color_t border_color;
}
lcd_area_t;

gui_status_t lcd_area_init(lcd_area_t *area,
                           uint16_t width, uint16_t height,
                           uint16_t x_pos, uint16_t y_pos,
                           gui_border_t border,
                           gui_active_t is_active,
                           char *string, const uint8_t *font,
                           h_align_t string_h_align, v_align_t string_v_align,
                           color_t text_color, color_t bg_color, color_t border_color);

/*
 * Getters
 */
gui_active_t  lcd_area_get_is_active(lcd_area_t *area);
gui_clicked_t lcd_area_get_is_clicked(lcd_area_t *area);

/*
 * Setters
 */
void          lcd_area_set_is_active(lcd_area_t *area, gui_active_t is_active);
void          lcd_area_set_is_clicked(lcd_area_t *area, gui_clicked_t is_clicked);

gui_status_t  lcd_area_set_text(lcd_area_t *area, char *string);

#endif /* LCD_AREA_H_ */