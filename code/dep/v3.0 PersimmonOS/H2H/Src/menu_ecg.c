#include "menu.h"

menu_t menu_ecg =
{
  .item_num = 6,
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
        .height               = 100,
        .pos                  = {.x_pos = 0, .y_pos = 380},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_ACTIVE,
        .string               = L"120",
        .font                 = myriad_pro_semibold_24,
        .string_h_align       = CENTER,
        .string_v_align       = MID,
        .text_color           = COLOR_WHITE,
        .bg_color             = COLOR_BLUE,
        .border_color         = COLOR_BLACK,
        .event                = fsm_no_event
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
    },
    {
      .item_print_function = lcd_print_area,
      .item.area =
      {
        .width                = 156,
        .height               = 60,
        .pos                  = {.x_pos = 161, .y_pos = 420},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_ACTIVE,
        .string               = L"1 lead ECG",
        .font                 = myriad_pro_semibold_16,
        .string_h_align       = CENTER,
        .string_v_align       = MID,
        .text_color           = COLOR_WHITE,
        .bg_color             = COLOR_BLUE,
        .border_color         = COLOR_BLACK,
        .event                = fsm_ecg_1_lead
      }
    },
    {
      .item_print_function = lcd_print_area,
      .item.area =
      {
        .width                = 156,
        .height               = 60,
        .pos                  = {.x_pos = 322, .y_pos = 420},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_ACTIVE,
        .string               = L"2 lead ECG",
        .font                 = myriad_pro_semibold_16,
        .string_h_align       = CENTER,
        .string_v_align       = MID,
        .text_color           = COLOR_WHITE,
        .bg_color             = COLOR_BLUE,
        .border_color         = COLOR_BLACK,
        .event                = fsm_ecg_2_lead
      }
    },
    {
      .item_print_function = lcd_print_area,
      .item.area =
      {
        .width                = 156,
        .height               = 60,
        .pos                  = {.x_pos = 483, .y_pos = 420},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_ACTIVE,
        .string               = L"6 lead ECG",
        .font                 = myriad_pro_semibold_16,
        .string_h_align       = CENTER,
        .string_v_align       = MID,
        .text_color           = COLOR_WHITE,
        .bg_color             = COLOR_BLUE,
        .border_color         = COLOR_BLACK,
        .event                = fsm_ecg_6_lead
      }
    },
  }
};