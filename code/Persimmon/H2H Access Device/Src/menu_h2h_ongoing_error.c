#include "menu.h"

const menu_t menu_h2h_ongoing_error =
{
  .item_num = 3,
  .items =
  {
    {
      .item_print_function = lcd_print_area,
      .item.area =
      {
        .width                = 410,
        .height               = 210,
        .pos                  = {.x_pos = 195, .y_pos = 135},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_INACTIVE,
        .string               = L"",
        .font                 = myriad_pro_semibold_16,
        .string_h_align       = CENTER,
        .string_v_align       = MID,
        .text_color           = COLOR_BLACK,
        .bg_color             = COLOR_BLACK,
        .border_color         = COLOR_BLACK,
        .event                = fsm_no_event
      }
    },
    {
      .item_print_function = lcd_print_area,
      .item.area =
      {
        .width                = 400,
        .height               = 200,
        .pos                  = {.x_pos = 200, .y_pos = 140},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_ACTIVE,
        .string               = L"ERROR",
        .font                 = myriad_pro_semibold_16,
        .string_h_align       = CENTER,
        .string_v_align       = MID,
        .text_color           = COLOR_WHITE,
        .bg_color             = COLOR_RED,
        .border_color         = COLOR_BLACK,
        .event                = fsm_no_event
      }
    },
    {
      .item_print_function = lcd_print_area,
      .item.area =
      {
        .width                = 150,
        .height               = 100,
        .pos                  = {.x_pos = 650, .y_pos = 380},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_ACTIVE,
        .string               = L"\xF064",
        .font                 = fontawesome_back,
        .string_h_align       = CENTER,
        .string_v_align       = MID,
        .text_color           = COLOR_WHITE,
        .bg_color             = COLOR_BLUE,
        .border_color         = COLOR_BLACK,
        .event                = fsm_back
      }
    }
  }
};