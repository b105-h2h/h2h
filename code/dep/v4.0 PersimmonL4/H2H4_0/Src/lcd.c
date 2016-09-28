/*
 * lcd.c
 *
 *  Created on: 10/05/2016
 *      Author: Smau
 */
#include "lcd.h"

/* LCD init parameters */
uint16_t lcd_param_pll_mn[]                     = {0x0023, 0x0022, 0x0004};     //M=35, N=2, PLL freq=120 Mhz
uint16_t lcd_param_pll[]                        = {0x0001};
uint16_t lcd_param_pll_lock[]                   = {0x0003};

uint16_t lcd_param_lshift_freq[]                = {0x0003, 0x0033, 0x0033};

uint16_t lcd_param_lcd_mode[]                   = {0x0020, 0x0000, 0x0003, 0x001F, 0x0001, 0x00DF, 0x0000};
uint16_t lcd_param_hori_period[]                = {0x0004, 0x001F, 0x0000, 0x00D2, 0x0000, 0x0000, 0x0000, 0x0000};
uint16_t lcd_param_vert_period[]                = {0x0002, 0x000C, 0x0000, 0x0022, 0x0000, 0x0000, 0x0000};

uint16_t lcd_param_gpio_conf[]                  = {0x000F, 0x0001};
uint16_t lcd_param_gpio_value[]                 = {0x0001};

uint16_t lcd_param_pixel_data_interface[]       = {0x0003};

uint16_t lcd_param_address_mode[]               = {0x0008};
uint16_t lcd_param_column_address[]             = {0x0000, 0x0000, 0x0000, 0x0000};
uint16_t lcd_param_page_address[]               = {0x0000, 0x0000, 0x0000, 0x0000};

uint16_t lcd_param_gamma_curve[]                = {0x0001};

uint16_t lcd_param_partial_area[]               = {0x0000, 0x0000, 0x0000, 0x0000};
uint16_t lcd_param_scroll_area[]                = {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000};
uint16_t lcd_param_scroll_start[]               = {0x0000, 0x0000};

uint16_t lcd_param_pwm_conf[]                   = {0x000C, 0x0000, 0x0001, 0x0000, 0x0000, 0x0001};
uint16_t lcd_param_dbc_conf[]                   = {0x0042};
uint16_t lcd_param_post_proc[]                  = {0x0040, 0x0080, 0x0040, 0x0001};

static uint32_t lcd_get_string_width(wchar_t *string, const uint8_t *font);
static uint32_t lcd_get_string_height(wchar_t *string, const uint8_t *font);

/**
  * @brief  Initializes LCD module
  * @retval None
*/
void lcd_init(lcd_t *lcd, SRAM_HandleTypeDef *hsram,
              uint32_t *lcd_reg, uint32_t *lcd_data,
              uint16_t lcd_x_size, uint16_t lcd_y_size)
{
  uint16_t lcd_device_descriptor[5];
  uint8_t device_descriptor_ok;
  color_t bg_color;

  lcd_hal_init(lcd, hsram, lcd_reg, lcd_data);

  lcd->lcd_x_size = lcd_x_size;
  lcd->lcd_y_size = lcd_y_size;
  bg_color = (color_t) COLOR_BLACK;

  /* Check device descriptor */
  lcd_hal_read_reg(lcd, LCD_READ_DDB, lcd_device_descriptor, 5);
  device_descriptor_ok = (lcd_device_descriptor[0] == 0x0001) &&        // Supplier ID (high byte)
                         (lcd_device_descriptor[1] == 0x0057) &&        // Supplier ID (low byte)
                         (lcd_device_descriptor[2] == 0x0061) &&        // Product ID
                         (lcd_device_descriptor[3] == 0x0001) &&        // Revision code
                         (lcd_device_descriptor[4] == 0x00FF);          // End code

  device_descriptor_ok = 1;

  if (device_descriptor_ok)
  {
    /* Set PLL */
    lcd_hal_write_reg(lcd, LCD_SET_PLL_MN, lcd_param_pll_mn, 3);
    /* Enable PLL */
    lcd_hal_write_reg(lcd, LCD_SET_PLL, lcd_param_pll, 1);
    /* Wait for PLL to start up */
    HAL_Delay(1);
    /* Lock PLL */
    lcd_hal_write_reg(lcd, LCD_SET_PLL, lcd_param_pll_lock, 1);

    /* Soft-reset LCD controller */
    lcd_hal_write_command(lcd, LCD_SOFT_RESET);
    HAL_Delay(10);

    /* Set the pixel clock frequency */
    lcd_hal_write_reg(lcd, LCD_SET_LSHIFT_FREQ, lcd_param_lshift_freq, 3);

    /* Set LCD modes, horizontal and vertical sync, and pixel data interface */
    lcd_hal_write_reg(lcd, LCD_SET_LCD_MODE, lcd_param_lcd_mode, 7);
    lcd_hal_write_reg(lcd, LCD_SET_HORI_PERIOD, lcd_param_hori_period, 8);
    lcd_hal_write_reg(lcd, LCD_SET_VERT_PERIOD, lcd_param_vert_period, 7);

    /* Set GPIO configuration */
    lcd_hal_write_reg(lcd, LCD_SET_GPIO_CONF, lcd_param_gpio_conf, 2);
    lcd_hal_write_reg(lcd, LCD_SET_GPIO_VALUE, lcd_param_gpio_value, 1);

    /* Set pixel data format */
    lcd_hal_write_reg(lcd, LCD_SET_PIXEL_DATA_INTERFACE, lcd_param_pixel_data_interface, 1);

    /* Set address scan mode and column and row starting pixel */
    lcd_hal_write_reg(lcd, LCD_SET_ADDRESS_MODE, lcd_param_address_mode, 1);
    lcd_hal_write_reg(lcd, LCD_SET_COLUMN_ADDRESS, lcd_param_column_address, 4);
    lcd_hal_write_reg(lcd, LCD_SET_PAGE_ADDRESS, lcd_param_page_address, 4);

    /* Set gamma curve */
    lcd_hal_write_reg(lcd, LCD_SET_GAMMA_CURVE, lcd_param_gamma_curve, 1);

    /* Set partial area and scrolling configuration */
    lcd_hal_write_reg(lcd, LCD_SET_PARTIAL_AREA, lcd_param_partial_area, 4);
    lcd_hal_write_reg(lcd, LCD_SET_SCROLL_AREA, lcd_param_scroll_area, 6);
    lcd_hal_write_reg(lcd, LCD_SET_SCROLL_START, lcd_param_scroll_start, 2);

    /* Set backlight brightness and image post processing defaults */
    lcd_hal_write_reg(lcd, LCD_SET_PWM_CONF, lcd_param_pwm_conf, 6);
    lcd_hal_write_reg(lcd, LCD_SET_DBC_CONF, lcd_param_dbc_conf, 1);
    lcd_hal_write_reg(lcd, LCD_SET_POST_PROC, lcd_param_post_proc, 4);

    /* Turn display off and set TE signal off */
    lcd_hal_write_command(lcd, LCD_SET_TEAR_OFF);

    /* Clean frame buffer */
    lcd_draw_background(lcd, &bg_color);

    /* Initialize lcd structure */
    lcd->lcd_x_size = LCD_X_SIZE;
    lcd->lcd_y_size = LCD_Y_SIZE;
    lcd->backlight_level = 100;

    /* Exit sleep mode and turn display on */
    lcd_hal_write_command(lcd, LCD_EXIT_SLEEP_MODE);
    HAL_Delay(10);
    //lcd_hal_write_command(lcd, LCD_SET_DISPLAY_ON);    //LCD_EXIT_SLEEP_MODE triggers LCD_SET_DISPLAY_ON
  }
}

gui_status_t lcd_print_area(lcd_t *lcd, item_t *item)
{
  uint16_t x_pos, x_pos_width;
  uint16_t y_pos, y_pos_height;

  uint16_t string_x_pos, string_y_pos;
  uint32_t string_width = 0, string_height = 0;

  color_t text_color;
  color_t bg_color;
  color_t border_color;

  text_color = item->area.text_color;
  bg_color = item->area.bg_color;
  border_color = item->area.border_color;

  /* Check if area lies within lcd boundaries */
  if (item->area.pos.x_pos + item->area.width > lcd->lcd_x_size)
  {
    return GUI_SIZE_ERROR;
  }
  if (item->area.pos.y_pos + item->area.height > lcd->lcd_y_size)
  {
    return GUI_SIZE_ERROR;
  }

  /* Check if string fits in area */
  string_width = lcd_get_string_width(item->area.string, item->area.font);
  string_height = lcd_get_string_height(item->area.string, item->area.font);
  if (item->area.width < string_width)
  {
    return GUI_SIZE_ERROR;
  }
  if (item->area.height < string_height)
  {
    return GUI_SIZE_ERROR;
  }

  /* Draw area rectangle */
  lcd_draw_rectangle(lcd,
                     item->area.pos.x_pos, item->area.width,
                     item->area.pos.y_pos, item->area.height,
                     &bg_color);

  /* Draw border if present */
  if (item->area.border == GUI_RECTANGLE)
  {
    x_pos = item->area.pos.x_pos;
    y_pos = item->area.pos.y_pos;
    x_pos_width = item->area.pos.x_pos + item->area.width - 1;
    y_pos_height = item->area.pos.y_pos + item->area.height - 1;

    /* Top line */
    lcd_draw_line(lcd,
                  x_pos, y_pos,
                  x_pos_width, y_pos,
                  &border_color);
    /* Bottom line */
    lcd_draw_line(lcd,
                  x_pos, y_pos_height,
                  x_pos_width, y_pos_height,
                  &border_color);
    /* Left line */
    lcd_draw_line(lcd,
                  x_pos, y_pos,
                  x_pos, y_pos_height,
                  &border_color);
    /* Right line */
    lcd_draw_line(lcd,
                  x_pos_width, y_pos,
                  x_pos_width, y_pos_height,
                  &border_color);
  }

  /* Draw text */
  switch (item->area.string_h_align)
  {
    case LEFT:
      string_x_pos = item->area.pos.x_pos;
      break;
    case CENTER:
      string_x_pos = item->area.pos.x_pos +
                     (item->area.width >> 1) -
                     (string_width >> 1);
      break;
    case RIGHT:
      string_x_pos = item->area.pos.x_pos +
                     item->area.width -
                     string_width;
      break;
    default:
      string_x_pos = item->area.pos.x_pos;
      break;
  }

  switch (item->area.string_v_align)
  {
    case TOP:
      string_y_pos = item->area.pos.y_pos;
      break;
    case MID:
      string_y_pos = item->area.pos.y_pos +
                     (item->area.height >> 1) -
                     (string_height >> 1);
      break;
    case RIGHT:
      string_y_pos = item->area.pos.y_pos +
                     item->area.height -
                     string_height;
      break;
    default:
      string_y_pos = item->area.pos.y_pos;
      break;
  }

  lcd_draw_string(lcd,
                  item->area.string,
                  item->area.font,
                  &text_color, &bg_color,
                  string_x_pos, string_y_pos);

  return GUI_OK;
}

gui_status_t lcd_print_graph(lcd_t *lcd, item_t *item)
{
  uint16_t x_pos, x_pos_width;
  uint16_t y_pos, y_pos_height;

  color_t text_color;
  color_t bg_legend_color;
  color_t border_color;

  text_color = item->graph.text_color;
  bg_legend_color = item->graph.bg_legend_color;
  border_color = item->graph.border_color;
  // Esto lo pongo para que el compilador deje de joder
  // se puede quitar a cholon
  UNUSED(text_color);
  /* Check if area lies within lcd boundaries */
  if (item->graph.pos.x_pos + item->graph.width_legend + item->graph.width_graph > lcd->lcd_x_size)
  {
    return GUI_SIZE_ERROR;
  }
  if (item->graph.pos.y_pos + item->graph.height > lcd->lcd_y_size)
  {
    return GUI_SIZE_ERROR;
  }

  /* Draw legend rectangle */
  lcd_draw_rectangle(lcd,
                     item->graph.pos.x_pos, item->graph.width_legend,
                     item->graph.pos.y_pos, item->graph.height,
                     &bg_legend_color);

  /* Draw graph rectangle */
  lcd_draw_rectangle(lcd,
                     item->graph.pos.x_pos + item->graph.width_legend, item->graph.width_graph,
                     item->graph.pos.y_pos, item->graph.height,
                     &(item->graph.bg_graph_color));

  /* Draw border if present */
  if (item->graph.border == GUI_RECTANGLE)
  {
    x_pos = item->graph.pos.x_pos;
    y_pos = item->graph.pos.y_pos;
    x_pos_width = item->graph.pos.x_pos + item->graph.width_legend + item->graph.width_graph - 1;
    y_pos_height = item->graph.pos.y_pos + item->graph.height - 1;

    /* Top line */
    lcd_draw_line(lcd,
                  x_pos, y_pos,
                  x_pos_width, y_pos,
                  &border_color);
    /* Bottom line */
    lcd_draw_line(lcd,
                  x_pos, y_pos_height,
                  x_pos_width, y_pos_height,
                  &border_color);
    /* Left line */
    lcd_draw_line(lcd,
                  x_pos, y_pos,
                  x_pos, y_pos_height,
                  &border_color);
    /* Right line */
    lcd_draw_line(lcd,
                  x_pos_width, y_pos,
                  x_pos_width, y_pos_height,
                  &border_color);
  }

  return GUI_OK;
}

gui_status_t lcd_update_graph(lcd_t *lcd, item_t *item)
{
  uint32_t y_pos_second_to_last, y_pos_last;

  /* Calculate graph points */
  y_pos_second_to_last = (item->graph.second_to_last_value * (item->graph.height >> 1)) /
                         (item->graph.y_axis_full_scale);
  y_pos_last           = (item->graph.last_value * (item->graph.height >> 1)) /
                         (item->graph.y_axis_full_scale);

  if (item->graph.value_index < item->graph.width_graph - 1)
  {
    lcd_draw_line(lcd,
                  item->graph.pos.x_pos + item->graph.width_legend + item->graph.value_index,
                  item->graph.pos.y_pos + (item->graph.height >> 1) + y_pos_second_to_last,
                  item->graph.pos.x_pos + item->graph.width_legend + item->graph.value_index + 1,
                  item->graph.pos.y_pos + (item->graph.height >> 1) + y_pos_last,
                  &(item->graph.line_color));
    item->graph.value_index++;
  }
  else
  {
    item->graph.value_index = 0;
  }

  return GUI_OK;
}
/**
  * @brief  Sets the backlight brightness level.
  * @param  level: Backlight PWM duty cycle (0 - 255).
  * @retval None
*/
gui_status_t lcd_set_config(lcd_t *lcd, item_t *item)
{
  uint16_t param_pwm_conf_tmp[7];

  /* Read previous configuration */
  lcd_hal_read_reg(lcd, LCD_GET_PWM_CONF, param_pwm_conf_tmp, 7);

  /* Change PWM value */
  param_pwm_conf_tmp[1] = item->config.backlight_level;

  /* Send new values */
  lcd_hal_write_reg(lcd, LCD_SET_PWM_CONF, param_pwm_conf_tmp, 6);

  return GUI_OK;
}

static uint32_t lcd_get_string_width(wchar_t *string, const uint8_t *font)
{
  uint32_t string_width = 0;

  uint32_t font_starting_char;          // Font array starting character (character number == 0)
  uint32_t character_number;            // Character number in font array

  uint32_t character_properties_index;  // Character properties position in font array
  uint32_t character_width;             // Character width in pixels

  wchar_t character;
  uint32_t char_index = 0;

  /* Find starting char in font */
  font_starting_char = ((uint32_t) font[3] << 8) | (uint32_t) font[2];

  /* Accumulate character widths */
  while ((character = string[char_index++]) != 0)
  {
    /* Get character number */
    character_number = character - font_starting_char;

    /* Get character width */
    character_properties_index = 8 + 4*character_number;
    character_width = font[character_properties_index];

    /* Add to string total value */
    string_width += character_width;
  }

  return string_width;
}

static uint32_t lcd_get_string_height(wchar_t *string, const uint8_t *font)
{
  return font[6];
}