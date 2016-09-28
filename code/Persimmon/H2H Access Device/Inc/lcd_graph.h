/*
 * lcd_graph.h
 *
 *  Created on: 27/05/2016
 *      Author: slopez
 */

#ifndef LCD_GRAPH_H_
#define LCD_GRAPH_H_

#include "lcd_items_utils.h"

#include "lcd_position.h"
#include "color.h"

typedef struct lcd_graph_t
{
  uint16_t width_legend;
  uint16_t width_graph;
  uint16_t height;

  lcd_pos_t pos;

  h_align_t legend_position;

  gui_border_t border;
  gui_active_t is_active;
  gui_clicked_t is_clicked;

  uint32_t x_axis_full_scale;        // In miliseconds
  uint32_t y_axis_full_scale;        // In nanovolts

  uint32_t value_index;
  int32_t second_to_last_value;
  int32_t last_value;

  color_t text_color;
  color_t bg_legend_color;
  color_t bg_graph_color;
  color_t line_color;
  color_t border_color;
}
lcd_graph_t;

gui_status_t lcd_graph_init(lcd_graph_t *graph,
                            uint16_t width_legend, uint16_t width_graph, uint16_t height,
                            uint16_t x_pos, uint16_t y_pos,
                            h_align_t legend_position,
                            gui_border_t border,
                            gui_active_t is_active,
                            uint32_t x_axis_full_scale, uint32_t y_axis_full_scale,
                            color_t text_color, color_t bg_legend_color, color_t bg_graph_color,
                            color_t line_color, color_t border_color);

/*
 * Getters
 */
gui_active_t  lcd_graph_get_is_active(lcd_graph_t *graph);
gui_clicked_t lcd_graph_get_is_clicked(lcd_graph_t *graph);

/*
 * Setters
 */
void          lcd_graph_set_is_active(lcd_graph_t *graph, gui_active_t is_active);
void          lcd_graph_set_is_clicked(lcd_graph_t *graph, gui_clicked_t is_clicked);

/*
 * Handlers
 */
void          lcd_graph_add_value(lcd_graph_t *graph, int32_t value);

#endif /* LCD_GRAPH_H_ */