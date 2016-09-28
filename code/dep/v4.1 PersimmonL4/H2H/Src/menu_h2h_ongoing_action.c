#include "menu.h"

menu_t menu_h2h_ongoing_action =
{
  .item_num = 4,
  .items =
  {
    {
      .item_print_function = lcd_print_area,
      .item.area =
      {
        .width                = 800,
        .height               = 450,
        .pos                  = {.x_pos = 0, .y_pos = 30},
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
        .width                = 150,
        .height               = 100,
        .pos                  = {.x_pos = 650, .y_pos = 380},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_ACTIVE,
        .string               = L"Back",
        .font                 = myriad_pro_semibold_16,
        .string_h_align       = CENTER,
        .string_v_align       = MID,
        .text_color           = COLOR_WHITE,
        .bg_color             = COLOR_BLUE,
        .border_color         = COLOR_BLACK,
        .event                = fsm_back
      }
    },
    {
      .item_print_function = lcd_print_area,
      .item.area =
      {
        .width                = 610,
        .height               = 60,
        .pos                  = {.x_pos = 20, .y_pos = 90},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_ACTIVE,
        .string               = L"Device: Pacemaker 266178",
        .font                 = myriad_pro_semibold_16,
        .string_h_align       = CENTER,
        .string_v_align       = MID,
        .text_color           = COLOR_WHITE,
        .bg_color             = COLOR_BLUE,
        .border_color         = COLOR_BLACK,
        .event                = fsm_h2h_connect
      }
    },
    {
      .item_print_function = lcd_print_area,
      .item.area =
      {
        .width                = 610,
        .height               = 320,
        .pos                  = {.x_pos = 20, .y_pos = 120},
        .border               = GUI_RECTANGLE,
        .is_active            = GUI_ACTIVE,
        .string               = L"",
        .font                 = myriad_pro_semibold_16,
        .string_h_align       = CENTER,
        .string_v_align       = MID,
        .text_color           = COLOR_WHITE,
        .bg_color             = COLOR_BLACK,
        .border_color         = COLOR_WHITE,
        .event                = fsm_no_event
      }
    },
  }
};