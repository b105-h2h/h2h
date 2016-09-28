#include "menu.h"

menu_t menu_settings_configtabs =
{
  .item_num = 7,
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
        .width                = 156,
        .height               = 60,
        .pos                  = {.x_pos = 0, .y_pos = 35},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_ACTIVE,
        .string               = L"Screen",
        .font                 = myriad_pro_semibold_16,
        .string_h_align       = CENTER,
        .string_v_align       = MID,
        .text_color           = COLOR_WHITE,
        .bg_color             = COLOR_BLUE,
        .border_color         = COLOR_BLACK,
        .event                = fsm_settings_screen
      }
    },
    {
      .item_print_function = lcd_print_area,
      .item.area =
      {
        .width                = 156,
        .height               = 60,
        .pos                  = {.x_pos = 161, .y_pos = 35},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_ACTIVE,
        .string               = L"Time/Date",
        .font                 = myriad_pro_semibold_16,
        .string_h_align       = CENTER,
        .string_v_align       = MID,
        .text_color           = COLOR_WHITE,
        .bg_color             = COLOR_BLUE,
        .border_color         = COLOR_BLACK,
        .event                = fsm_settings_timedate
      }
    },
    {
      .item_print_function = lcd_print_area,
      .item.area =
      {
        .width                = 156,
        .height               = 60,
        .pos                  = {.x_pos = 322, .y_pos = 35},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_ACTIVE,
        .string               = L"Bluetooth",
        .font                 = myriad_pro_semibold_16,
        .string_h_align       = CENTER,
        .string_v_align       = MID,
        .text_color           = COLOR_WHITE,
        .bg_color             = COLOR_BLUE,
        .border_color         = COLOR_BLACK,
        .event                = fsm_settings_bt
      }
    },
    {
      .item_print_function = lcd_print_area,
      .item.area =
      {
        .width                = 156,
        .height               = 60,
        .pos                  = {.x_pos = 483, .y_pos = 35},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_ACTIVE,
        .string               = L"Touch",
        .font                 = myriad_pro_semibold_16,
        .string_h_align       = CENTER,
        .string_v_align       = MID,
        .text_color           = COLOR_WHITE,
        .bg_color             = COLOR_BLUE,
        .border_color         = COLOR_BLACK,
        .event                = fsm_settings_touch
      }
    },
    {
      .item_print_function = lcd_print_area,
      .item.area =
      {
        .width                = 156,
        .height               = 60,
        .pos                  = {.x_pos = 644, .y_pos = 35},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_ACTIVE,
        .string               = L"About",
        .font                 = myriad_pro_semibold_16,
        .string_h_align       = CENTER,
        .string_v_align       = MID,
        .text_color           = COLOR_WHITE,
        .bg_color             = COLOR_BLUE,
        .border_color         = COLOR_BLACK,
        .event                = fsm_settings_about
      }
    },
    {
      .item_print_function = lcd_print_area,
      .item.area =
      {
        .width                = 156,
        .height               = 100,
        .pos                  = {.x_pos = 644, .y_pos = 380},
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
    }
  }
};