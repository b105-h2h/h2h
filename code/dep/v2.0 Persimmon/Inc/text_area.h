/*
 * text_area.h
 *
 *  Created on: 27/05/2016
 *      Author: slopez
 */

#ifndef TEXT_AREA_H_
#define TEXT_AREA_H_

#include "color.h"

typedef enum {LEFT, CENTER, RIGHT} align_h_t;
typedef enum {TOP, MID, BOTTOM} align_v_t;

typedef enum {GUI_OK, GUI_SIZE_ERROR} gui_status_t;

typedef struct text_area_t
{
  uint16_t width;
  uint16_t height;

  const uint8_t *font;
  char string[50];

  align_h_t text_area_h_align;
  align_v_t text_area_v_align;
  
  align_h_t text_h_align;
  align_v_t text_v_align;
}
text_area_t;

gui_status_t text_area_init(text_area_t *text_area,
                            uint16_t width, uint16_t height,
                            const uint8_t *font, char *string,
                            align_h_t text_area_h_align, align_v_t text_area_v_align,
                            align_h_t text_h_align, align_v_t text_v_align);

uint16_t     text_area_get_width(text_area_t *text_area);
uint16_t     text_area_get_height(text_area_t *text_area);
align_h_t    text_area_get_text_area_h_align(text_area_t *text_area);
align_v_t    text_area_get_text_area_v_align(text_area_t *text_area);
align_h_t    text_area_get_text_h_align(text_area_t *text_area);
align_v_t    text_area_get_text_v_align(text_area_t *text_area);
void         text_area_get_font(text_area_t *text_area, const uint8_t *font);
void         text_area_get_string(text_area_t *text_area, char *string);

void         text_area_set_width(text_area_t *text_area, uint16_t width);
void         text_area_set_height(text_area_t *text_area, uint16_t height);
void         text_area_set_text_area_h_align(text_area_t *text_area, align_h_t text_area_h_align);
void         text_area_set_text_area_v_align(text_area_t *text_area, align_v_t text_area_v_align);
void         text_area_set_text_h_align(text_area_t *text_area, align_h_t text_h_align);
void         text_area_set_text_v_align(text_area_t *text_area, align_v_t text_v_align);
void         text_area_set_font(text_area_t *text_area, const uint8_t *font);
gui_status_t text_area_set_string(text_area_t *text_area, char *string);

#endif /* TEXT_AREA_H_ */