#include "menu.h"

const menu_t menu_main =
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
        .font                 = myriad_pro_semibold_24,
        .string_h_align       = CENTER,
        .string_v_align       = MID,
        .text_color           = COLOR_BLACK,
        .bg_color             = COLOR_BLACK,
        .border_color         = COLOR_BLACK,
        .event                = fsm_no_event
      }
    },
    /* ECG */
    {
      .item_print_function = lcd_print_area,
      .item.area =
      {
        .width                = 145,
        .height               = 145,
        .pos                  = {.x_pos = 0, .y_pos = 35},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_ACTIVE,
        .string               = L"\xF21E",
        .font                 = fontawesome_ecg,
        .string_h_align       = CENTER,
        .string_v_align       = MID,
        .text_color           = COLOR_WHITE,
        .bg_color             = COLOR_BLUE,
        .border_color         = COLOR_BLACK,
        .event                = fsm_ecg
      }
    },
    {
      .item_print_function = lcd_print_area,
      .item.area =
      {
        .width                = 655,
        .height               = 145,
        .pos                  = {.x_pos = 145, .y_pos = 35},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_ACTIVE,
        .string               = L"ECG",
        .font                 = myriad_pro_bold_20,
        .string_h_align       = CENTER,
        .string_v_align       = MID,
        .text_color           = COLOR_BLACK,
        .bg_color             = COLOR_CYAN,
        .border_color         = COLOR_BLUE,
        .event                = fsm_ecg
      }
    },
    /* H2H */
    {
      .item_print_function = lcd_print_area,
      .item.area =
      {
        .width                = 145,
        .height               = 145,
        .pos                  = {.x_pos = 0, .y_pos = 185},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_ACTIVE,
        .string               = L"\xF0FA",
        .font                 = fontawesome_h2h,
        .string_h_align       = CENTER,
        .string_v_align       = MID,
        .text_color           = COLOR_WHITE,
        .bg_color             = COLOR_BLUE,
        .border_color         = COLOR_BLACK,
        .event                = fsm_h2h
      }
    },
    {
      .item_print_function = lcd_print_area,
      .item.area =
      {
        .width                = 655,
        .height               = 145,
        .pos                  = {.x_pos = 145, .y_pos = 185},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_ACTIVE,
        .string               = L"H2H",
        .font                 = myriad_pro_bold_20,
        .string_h_align       = CENTER,
        .string_v_align       = MID,
        .text_color           = COLOR_BLACK,
        .bg_color             = COLOR_CYAN,
        .border_color         = COLOR_BLUE,
        .event                = fsm_h2h
      }
    },
    /* Settings */
    {
      .item_print_function = lcd_print_area,
      .item.area =
      {
        .width                = 145,
        .height               = 145,
        .pos                  = {.x_pos = 0, .y_pos = 335},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_ACTIVE,
        .string               = L"\xF085",
        .font                 = fontawesome_settings,
        .string_h_align       = CENTER,
        .string_v_align       = MID,
        .text_color           = COLOR_WHITE,
        .bg_color             = COLOR_BLUE,
        .border_color         = COLOR_BLACK,
        .event                = fsm_settings
      }
    },
    {
      .item_print_function = lcd_print_area,
      .item.area =
      {
        .width                = 655,
        .height               = 145,
        .pos                  = {.x_pos = 145, .y_pos = 335},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_ACTIVE,
        .string               = L"Settings",
        .font                 = myriad_pro_bold_20,
        .string_h_align       = CENTER,
        .string_v_align       = MID,
        .text_color           = COLOR_BLACK,
        .bg_color             = COLOR_CYAN,
        .border_color         = COLOR_BLUE,
        .event                = fsm_settings
      }
    }
  }
};