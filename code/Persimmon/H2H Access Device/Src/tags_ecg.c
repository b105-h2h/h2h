#include "menu.h"

const menu_t tags_ecg_1_lead =
{
  .item_num = 1,
  .items =
  {
    /* 1 lead */
    {
      .item_print_function = lcd_print_area,
      .item.area =
      {
        .width                = 80,
        .height               = 40,
        .pos                  = {.x_pos = 48, .y_pos = 67},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_ACTIVE,
        .string               = L"Lead I",
        .font                 = myriad_pro_semibold_16,
        .string_h_align       = LEFT,
        .string_v_align       = TOP,
        .text_color           = COLOR_WHITE,
        .bg_color             = COLOR_BLUE,
        .border_color         = COLOR_BLACK,
        .event                = fsm_no_event
      }
    },
  }
};

const menu_t tags_ecg_2_lead =
{
  .item_num = 2,
  .items =
  {
    /* 2 lead */
    {
      .item_print_function = lcd_print_area,
      .item.area =
      {
        .width                = 70,
        .height               = 40,
        .pos                  = {.x_pos = 45, .y_pos = 65},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_ACTIVE,
        .string               = L"Lead I",
        .font                 = myriad_pro_semibold_16,
        .string_h_align       = LEFT,
        .string_v_align       = TOP,
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
        .width                = 70,
        .height               = 40,
        .pos                  = {.x_pos = 45, .y_pos = 215},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_ACTIVE,
        .string               = L"Lead II",
        .font                 = myriad_pro_semibold_16,
        .string_h_align       = LEFT,
        .string_v_align       = TOP,
        .text_color           = COLOR_WHITE,
        .bg_color             = COLOR_BLUE,
        .border_color         = COLOR_BLACK,
        .event                = fsm_no_event
      }
    },
  }
};

const menu_t tags_ecg_6_lead =
{
  .item_num = 6,
  .items =
  {
    /* 6 lead */
    {
      .item_print_function = lcd_print_area,
      .item.area =
      {
        .width                = 60,
        .height               = 40,
        .pos                  = {.x_pos = 45, .y_pos = 65},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_ACTIVE,
        .string               = L"Lead I",
        .font                 = myriad_pro_semibold_12,
        .string_h_align       = LEFT,
        .string_v_align       = TOP,
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
        .width                = 60,
        .height               = 40,
        .pos                  = {.x_pos = 45, .y_pos = 165},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_ACTIVE,
        .string               = L"Lead II",
        .font                 = myriad_pro_semibold_12,
        .string_h_align       = LEFT,
        .string_v_align       = TOP,
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
        .width                = 60,
        .height               = 40,
        .pos                  = {.x_pos = 45, .y_pos = 265},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_ACTIVE,
        .string               = L"Lead III",
        .font                 = myriad_pro_semibold_12,
        .string_h_align       = LEFT,
        .string_v_align       = TOP,
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
        .width                = 60,
        .height               = 40,
        .pos                  = {.x_pos = 410, .y_pos = 65},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_ACTIVE,
        .string               = L"Lead aVR",
        .font                 = myriad_pro_semibold_12,
        .string_h_align       = LEFT,
        .string_v_align       = TOP,
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
        .width                = 60,
        .height               = 40,
        .pos                  = {.x_pos = 410, .y_pos = 165},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_ACTIVE,
        .string               = L"Lead aVL",
        .font                 = myriad_pro_semibold_12,
        .string_h_align       = LEFT,
        .string_v_align       = TOP,
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
        .width                = 60,
        .height               = 40,
        .pos                  = {.x_pos = 410, .y_pos = 265},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_ACTIVE,
        .string               = L"Lead aVF",
        .font                 = myriad_pro_semibold_12,
        .string_h_align       = LEFT,
        .string_v_align       = TOP,
        .text_color           = COLOR_WHITE,
        .bg_color             = COLOR_BLUE,
        .border_color         = COLOR_BLACK,
        .event                = fsm_no_event
      }
    },
  }
};