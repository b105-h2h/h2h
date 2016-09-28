/*
 * area.h
 *
 *  Created on: 27/05/2016
 *      Author: slopez
 */

#ifndef AREA_H_
#define AREA_H_

#include "text_area.h"

#include "lcd_position.h"

typedef enum {ACTIVE, INACTIVE}     area_active_t;
typedef enum {CLICKED, NOT_CLICKED} area_clicked_t;
typedef enum {NO_BORDER, RECTANGLE} area_border_t;

typedef struct area_t
{
  uint16_t width;
  uint16_t height;

  lcd_pos_t pos;
  
  text_area_t *text;
  
  align_h_t h_align;
  align_v_t v_align;
  
  area_active_t is_active;
  area_clicked_t is_clicked;
  area_border_t border;
  
  color_t *text_color;
  color_t *bg_color;
  color_t *border_color;
}
area_t;

gui_status_t area_init(area_t *area,
                       uint16_t width, uint16_t height,
                       uint16_t x_pos, uint16_t y_pos,
                       align_h_t h_align, align_h_t v_align,
                       area_border_t border,
                       //Text area
                       uint16_t text_area_height, uint16_t text_area_width,
                       const uint8_t *font, char *string,
                       align_h_t text_area_h_align, align_v_t text_area_v_align,
                       align_h_t text_h_align, align_v_t text_v_align,
                       //---------
                       area_active_t is_active,
                       color_t* text_color, color_t* bg_color, color_t* border_color);

uint16_t       area_get_width(area_t *area);
uint16_t       area_get_height(area_t *area);
void           area_get_pos(area_t *area, lcd_pos_t *pos);
void           area_get_text(area_t *area, text_area_t *text);
align_h_t      area_get_align_h(area_t *area);
align_v_t      area_get_align_v(area_t *area);
area_active_t  area_get_is_active(area_t *area);
area_clicked_t area_get_is_clicked(area_t *area);
area_border_t  area_get_border(area_t *area);
color_t*       area_get_text_color(area_t *area);
color_t*       area_get_bg_color(area_t *area);
color_t*       area_get_border_color(area_t *area);
void           area_get_string(area_t *area, char *string);

void           area_set_width(area_t *area, uint16_t width);
void           area_set_height(area_t *area, uint16_t height);
void           area_set_pos(area_t *area, lcd_pos_t *pos);
gui_status_t   area_set_text(area_t *area, text_area_t *text);
void           area_set_align_h(area_t *area, align_h_t h_align);
void           area_set_align_v(area_t *area, align_v_t v_align);
void           area_set_is_active(area_t *area, area_active_t is_active);
void           area_set_is_clicked(area_t *area, area_clicked_t is_clicked);
void           area_set_border(area_t *area, area_border_t border);
void           area_set_text_color(area_t *area, color_t* text_color);
void           area_set_bg_color(area_t *area, color_t* bg_color);
void           area_set_border_color(area_t *area, color_t* border_color);

#endif /* AREA_H_ */