#include "lcd_graph.h"

gui_status_t lcd_graph_init(lcd_graph_t *graph,
                            uint16_t width_legend, uint16_t width_graph, uint16_t height,
                            uint16_t x_pos, uint16_t y_pos,
                            h_align_t legend_position,
                            gui_border_t border,
                            gui_active_t is_active,
                            uint32_t x_axis_full_scale, uint32_t y_axis_full_scale,
                            color_t text_color, color_t bg_legend_color, color_t bg_graph_color,
                            color_t line_color, color_t border_color)
{
  /* Set graph size */
  graph->width_legend = width_legend;
  graph->width_graph = width_graph;
  graph->height = height;

  /* Set graph position */
  graph->pos.x_pos = x_pos;
  graph->pos.y_pos = y_pos;

  /* Set legend position */
  graph->legend_position = legend_position;

  /* Set area border */
  graph->border = border;

  /* Set behaviour default parameters */
  graph->is_active = is_active;
  graph->is_clicked = GUI_NOT_CLICKED;

  /* Set axis scales */
  graph->x_axis_full_scale = x_axis_full_scale;
  graph->y_axis_full_scale = y_axis_full_scale;

  /* Reset values */
  graph->value_index = 0;
  graph->second_to_last_value = 0;
  graph->last_value = 0;

  /* Set colors */
  graph->text_color = text_color;
  graph->bg_legend_color = bg_legend_color;
  graph->bg_graph_color = bg_graph_color;
  graph->line_color = line_color;
  graph->border_color = border_color;

  return GUI_OK;
}

/*
 * Getters
 */
gui_active_t lcd_graph_get_is_active(lcd_graph_t *graph)
{
  return graph->is_active;
}

gui_clicked_t lcd_graph_get_is_clicked(lcd_graph_t *graph)
{
  return graph->is_clicked;
}

/*
 * Setters
 */
void lcd_graph_set_is_active(lcd_graph_t *graph, gui_active_t is_active)
{
  graph->is_active = is_active;
}

void lcd_graph_set_is_clicked(lcd_graph_t *graph, gui_clicked_t is_clicked)
{
  graph->is_clicked = is_clicked;
}

/*
 * Handlers
 */
void lcd_graph_add_value(lcd_graph_t *graph, int32_t value)
{
  graph->second_to_last_value = graph->last_value;
  graph->last_value = value;
}
