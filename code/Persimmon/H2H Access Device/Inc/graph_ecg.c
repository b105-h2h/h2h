#include "menu.h"

menu_t graph_ecg_1_lead =
{
  .item_num = 1,
  .items =
  {
    {
      /* 1 lead */
      .item_print_function = lcd_print_graph,
      .item.graph =
      {
        .width_legend         = 90,
        .width_graph          = 630,
        .height               = 290,
        .pos                  = {.x_pos = 40, .y_pos = 60},
        .legend_position      = LEFT,
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_INACTIVE,
        .x_axis_full_scale    = 2000,
        .y_axis_full_scale    = 2000,
        .value_index          = 0,
        .second_to_last_value = 0,
        .last_value           = 0,
        .text_color           = COLOR_WHITE,
        .bg_legend_color      = COLOR_BLUE,
        .bg_graph_color       = COLOR_CYAN,
        .line_color           = COLOR_BLACK,
        .border_color         = COLOR_WHITE,
      }
    }
  }
};

menu_t graph_ecg_2_lead =
{
  .item_num = 2,
  .items =
  {
    /* 2 leads */
    {
      .item_print_function = lcd_print_graph,
      .item.graph =
      {
        .width_legend         = 80,
        .width_graph          = 640,
        .height               = 140,
        .pos                  = {.x_pos = 40, .y_pos = 60},
        .legend_position      = LEFT,
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_INACTIVE,
        .x_axis_full_scale    = 2000,
        .y_axis_full_scale    = 2000,
        .value_index          = 0,
        .second_to_last_value = 0,
        .last_value           = 0,
        .text_color           = COLOR_WHITE,
        .bg_legend_color      = COLOR_BLUE,
        .bg_graph_color       = COLOR_CYAN,
        .line_color           = COLOR_BLACK,
        .border_color         = COLOR_WHITE,
      }
    },
    {
      .item_print_function = lcd_print_graph,
      .item.graph =
      {
        .width_legend         = 80,
        .width_graph          = 640,
        .height               = 140,
        .pos                  = {.x_pos = 40, .y_pos = 210},
        .legend_position      = LEFT,
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_INACTIVE,
        .x_axis_full_scale    = 2000,
        .y_axis_full_scale    = 2000,
        .value_index          = 0,
        .second_to_last_value = 0,
        .last_value           = 0,
        .text_color           = COLOR_WHITE,
        .bg_legend_color      = COLOR_BLUE,
        .bg_graph_color       = COLOR_CYAN,
        .line_color           = COLOR_BLACK,
        .border_color         = COLOR_WHITE,
      }
    }
  }
};

menu_t graph_ecg_6_lead =
{
  .item_num = 6,
  .items =
  {
    /* 6 leads */
    {
      .item_print_function = lcd_print_graph,
      .item.graph =
      {
        .width_legend         = 70,
        .width_graph          = 285,
        .height               = 90,
        .pos                  = {.x_pos = 40, .y_pos = 60},
        .legend_position      = LEFT,
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_INACTIVE,
        .x_axis_full_scale    = 2000,
        .y_axis_full_scale    = 2000,
        .value_index          = 0,
        .second_to_last_value = 0,
        .last_value           = 0,
        .text_color           = COLOR_WHITE,
        .bg_legend_color      = COLOR_BLUE,
        .bg_graph_color       = COLOR_CYAN,
        .line_color           = COLOR_BLACK,
        .border_color         = COLOR_WHITE,
      }
    },
    {
      .item_print_function = lcd_print_graph,
      .item.graph =
      {
        .width_legend         = 70,
        .width_graph          = 285,
        .height               = 90,
        .pos                  = {.x_pos = 40, .y_pos = 160},
        .legend_position      = LEFT,
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_INACTIVE,
        .x_axis_full_scale    = 2000,
        .y_axis_full_scale    = 2000,
        .value_index          = 0,
        .second_to_last_value = 0,
        .last_value           = 0,
        .text_color           = COLOR_WHITE,
        .bg_legend_color      = COLOR_BLUE,
        .bg_graph_color       = COLOR_CYAN,
        .line_color           = COLOR_BLACK,
        .border_color         = COLOR_WHITE,
      }
    },
    {
      .item_print_function = lcd_print_graph,
      .item.graph =
      {
        .width_legend         = 70,
        .width_graph          = 285,
        .height               = 90,
        .pos                  = {.x_pos = 40, .y_pos = 260},
        .legend_position      = LEFT,
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_INACTIVE,
        .x_axis_full_scale    = 2000,
        .y_axis_full_scale    = 2000,
        .value_index          = 0,
        .second_to_last_value = 0,
        .last_value           = 0,
        .text_color           = COLOR_WHITE,
        .bg_legend_color      = COLOR_BLUE,
        .bg_graph_color       = COLOR_CYAN,
        .line_color           = COLOR_BLACK,
        .border_color         = COLOR_WHITE,
      }
    },
    {
      .item_print_function = lcd_print_graph,
      .item.graph =
      {
        .width_legend         = 70,
        .width_graph          = 285,
        .height               = 90,
        .pos                  = {.x_pos = 405, .y_pos = 60},
        .legend_position      = LEFT,
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_INACTIVE,
        .x_axis_full_scale    = 2000,
        .y_axis_full_scale    = 2000,
        .value_index          = 0,
        .second_to_last_value = 0,
        .last_value           = 0,
        .text_color           = COLOR_WHITE,
        .bg_legend_color      = COLOR_BLUE,
        .bg_graph_color       = COLOR_CYAN,
        .line_color           = COLOR_BLACK,
        .border_color         = COLOR_WHITE,
      }
    },
    {
      .item_print_function = lcd_print_graph,
      .item.graph =
      {
        .width_legend         = 70,
        .width_graph          = 285,
        .height               = 90,
        .pos                  = {.x_pos = 405, .y_pos = 160},
        .legend_position      = LEFT,
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_INACTIVE,
        .x_axis_full_scale    = 2000,
        .y_axis_full_scale    = 2000,
        .value_index          = 0,
        .second_to_last_value = 0,
        .last_value           = 0,
        .text_color           = COLOR_WHITE,
        .bg_legend_color      = COLOR_BLUE,
        .bg_graph_color       = COLOR_CYAN,
        .line_color           = COLOR_BLACK,
        .border_color         = COLOR_WHITE,
      }
    },
    {
      .item_print_function = lcd_print_graph,
      .item.graph =
      {
        .width_legend         = 70,
        .width_graph          = 285,
        .height               = 90,
        .pos                  = {.x_pos = 405, .y_pos = 260},
        .legend_position      = LEFT,
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_INACTIVE,
        .x_axis_full_scale    = 2000,
        .y_axis_full_scale    = 2000,
        .value_index          = 0,
        .second_to_last_value = 0,
        .last_value           = 0,
        .text_color           = COLOR_WHITE,
        .bg_legend_color      = COLOR_BLUE,
        .bg_graph_color       = COLOR_CYAN,
        .line_color           = COLOR_BLACK,
        .border_color         = COLOR_WHITE,
      }
    }
  }
};