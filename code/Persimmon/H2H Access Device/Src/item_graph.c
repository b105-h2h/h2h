#include "item_graph.h"

gui_status_t item_graph_init(item_graph_t *graph,
                             uint16_t width_legend, uint16_t width_graph, uint16_t height,
                             uint16_t x_pos, uint16_t y_pos,
                             gui_h_align_t legend_position,
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
gui_active_t item_graph_get_is_active(item_graph_t *graph)
{
  return graph->is_active;
}

/*
 * Setters
 */
void item_graph_set_is_active(item_graph_t *graph, gui_active_t is_active)
{
  graph->is_active = is_active;
}

/*
 * Handlers
 */
void item_graph_add_value(item_graph_t *graph, int32_t value)
{
  graph->value_index++;
  if (graph->value_index >= graph->width_graph)
  {
    graph->value_index = 0;
  }

  graph->second_to_last_value = graph->last_value;
  graph->last_value = value;
}

void item_graph_reset_value(item_graph_t *graph)
{
  graph->value_index = 0;
  graph->second_to_last_value = 0;
  graph->last_value = 0;
}
