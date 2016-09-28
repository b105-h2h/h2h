#include "area.h"

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
                       color_t* text_color, color_t* bg_color, color_t* border_color)
{
  gui_status_t status;
  lcd_pos_t pos;
  text_area_t text;
  
  /* Set area size */
  area_set_width(area, width);
  area_set_height(area, height);
  
  /* Set area position */
  pos.x_pos = x_pos;
  pos.y_pos = y_pos;
  area_set_pos(area, &pos);
  
  /* Set area alignment */
  area_set_align_h(area, h_align);
  area_set_align_v(area, v_align);
  
  /* Set area border */
  area_set_border(area, border);
  
  /* Set area text */
  text_area_init(&text, text_area_width, text_area_height,
                 font, string,
                 text_area_h_align, text_area_v_align,
                 text_h_align, text_v_align);
  status = area_set_text(area, &text);
  
  /* Set behaviour default parameters */
  area_set_is_active(area, is_active);
  area_set_is_clicked(area, NOT_CLICKED);
  
  /* Set colors */
  area_set_text_color(area, text_color);
  area_set_bg_color(area, bg_color);
  area_set_border_color(area, border_color);
  
  return status;
}

/*
 *  Getters
 *
 */
uint16_t area_get_width(area_t *area)
{
  return area->width;
}

uint16_t area_get_height(area_t *area)
{
  return area->height;
}

void area_get_pos(area_t *area, lcd_pos_t *pos)
{
  pos = &(area->pos);
}

void area_get_text(area_t *area, text_area_t *text)
{
  text = area->text;
}

align_h_t area_get_align_h(area_t *area)
{
  return area->h_align;
}

align_v_t area_get_align_v(area_t *area)
{
  return area->v_align;
}

area_active_t area_get_is_active(area_t *area)
{
  return area->is_active;
}

area_clicked_t area_get_is_clicked(area_t *area)
{
  return area->is_clicked;
}

area_border_t area_get_border(area_t *area)
{
  return area->border;
}

color_t* area_get_text_color(area_t *area)
{
  return area->text_color;
}

color_t* area_get_bg_color(area_t *area)
{
  return area->bg_color;
}

color_t* area_get_border_color(area_t *area)
{
  return area->border_color;
}

void area_get_string(area_t *area, char *string)
{
  string = area->text->string;
}


/*
 *  Setters
 *
 */
void area_set_width(area_t *area, uint16_t width)
{
  area->width = width;
}

void area_set_height(area_t *area, uint16_t height)
{
  area->height = height;
}

void area_set_pos(area_t *area, lcd_pos_t *pos)
{
  area->pos = *pos;
}

gui_status_t area_set_text(area_t *area, text_area_t *text)
{
  /* Check if text_area fits into area */
  if (text->height > area->height)
  {
    return GUI_SIZE_ERROR;
  }
  if (text->width > area->width)
  {
    return GUI_SIZE_ERROR;
  }
  
  /* Assign text to area */
  area->text = text;
  return GUI_OK;
}

void area_set_align_h(area_t *area, align_h_t h_align)
{
  area->h_align = h_align;
}

void area_set_align_v(area_t *area, align_v_t v_align)
{
  area->v_align = v_align;
}

void area_set_is_active(area_t *area, area_active_t is_active)
{
  area->is_active = is_active;
}

void area_set_is_clicked(area_t *area, area_clicked_t is_clicked)
{
  area->is_clicked = is_clicked;
}

void area_set_border(area_t *area, area_border_t border)
{
  area->border = border;
}

void area_set_text_color(area_t *area, color_t* text_color)
{
  area->text_color = text_color;
}

void area_set_bg_color(area_t *area, color_t* bg_color)
{
  area->bg_color = bg_color;
}

void area_set_border_color(area_t *area, color_t* border_color)
{
  area->border_color = border_color;
}
