/*
 * item_graph.h
 *
 *  Created on: 27/05/2016
 *      Author: slopez
 */

#ifndef ITEM_GRAPH_H_
#define ITEM_GRAPH_H_

#include "item_gui.h"

#include "lcd_position.h"
#include "color.h"

typedef struct item_graph_t
{
  uint16_t width_legend;
  uint16_t width_graph;
  uint16_t height;

  lcd_pos_t pos;

  gui_h_align_t legend_position;

  gui_border_t border;
  gui_active_t is_active;

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
item_graph_t;

gui_status_t item_graph_init(item_graph_t *graph,
                            uint16_t width_legend, uint16_t width_graph, uint16_t height,
                            uint16_t x_pos, uint16_t y_pos,
                            gui_h_align_t legend_position,
                            gui_border_t border,
                            gui_active_t is_active,
                            uint32_t x_axis_full_scale, uint32_t y_axis_full_scale,
                            color_t text_color, color_t bg_legend_color, color_t bg_graph_color,
                            color_t line_color, color_t border_color);

/*
 * Getters
 */
gui_active_t item_graph_get_is_active(item_graph_t *graph);

/*
 * Setters
 */
void item_graph_set_is_active(item_graph_t *graph, gui_active_t is_active);

/*
 * Handlers
 */
void item_graph_add_value(item_graph_t *graph, int32_t value);

#endif /* ITEM_GRAPH_H_ */