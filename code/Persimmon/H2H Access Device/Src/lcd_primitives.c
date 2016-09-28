#include "lcd_primitives.h"

extern DMA_HandleTypeDef hdma_memtomem_dma2_channel1;

/**
  * @brief  Draws a pixel on a specific location.
  * @param  x_pos: Pixel horizontal position.
  * @param  y_pos: Pixel vertical position.
  * @param  color: Color of pixel.
  * @retval None
*/
void lcd_draw_pixel(lcd_t *lcd, uint16_t x_pos, uint16_t y_pos, color_t *color)
{
  lcd_hal_set_drawing_address(lcd, x_pos, x_pos, y_pos, y_pos);
  lcd_hal_write_data(lcd, color->color565);
}

/**
  * @brief  Draws a line from two points.
  * @param  x0: Starting point horizontal position.
  * @param  y0: Starting point vertical position.
  * @param  x1: Finishing point horizontal position.
  * @param  y1: Finishing point vertical position.
  * @param  color: Color of pixel.
  * @retval None
*/
void lcd_draw_line(lcd_t *lcd, uint16_t origin_x, uint16_t origin_y,
                               uint16_t end_x, uint16_t end_y, color_t *color)
{
  int32_t dist_x, dist_y, step, step_y, current_x, current_y, i, j;

  // WARNING: Can't draw vertical lines, use draw_rectangle instead

  if(origin_x < end_x)
  {
    current_x = origin_x;
    dist_x = end_x - origin_x;
  }
  else
  {
    current_x = end_x;
    dist_x = origin_x - end_x;
  }

  current_y = origin_y;

  if(origin_y < end_y)
  {
    step_y = 1;
    dist_y = end_y - origin_y + 1;
  }
  else
  {
    step_y = -1;
    dist_y = origin_y - end_y + 1;
  }
  
  if( dist_x !=0)
  {
    step = dist_y/dist_x;
  }
  else
  {
    step = dist_y;
  }
  for(i = dist_x; i > 0; i--)
  {
    for(j = step; j > 0;j--)
    {
      lcd_draw_pixel(lcd, current_x, current_y, color);
      current_y = current_y + step_y;
    }
    current_x++;
  }
}

void lcd_draw_rectangle(lcd_t *lcd,
                        uint16_t x_pos, uint16_t width,
                        uint16_t y_pos, uint16_t height,
                        color_t *color)
{
  int32_t remaining_pixels = 0;
  uint32_t chunk_size = 0;

  /* Send screen pixel limits */
  lcd_hal_set_drawing_address(lcd, x_pos, x_pos + width - 1,
                                   y_pos, y_pos + height - 1);

  /* Calculate number of pixels to be sent */
  remaining_pixels = (int32_t) width * (int32_t) height;

  /* DMA transfer */
  while (remaining_pixels > 0)
  {
    /* Calculate chunk size */
    if (remaining_pixels > DMA_MAX_CHUNK_SIZE)
    {
      chunk_size = DMA_MAX_CHUNK_SIZE;
    }
    else
    {
      chunk_size = remaining_pixels;
    }

    /* Start chunk transfer */
    HAL_DMA_Start(&hdma_memtomem_dma2_channel1,
                  (uint32_t) &color->color565, (uint32_t) lcd->lcd_data, chunk_size);
    while (HAL_DMA_PollForTransfer(&hdma_memtomem_dma2_channel1, HAL_DMA_FULL_TRANSFER, HAL_MAX_DELAY) != HAL_OK);

    /* Update remaining pixels */
    remaining_pixels -= chunk_size;
  }
}

/**
  * @brief  Draws a char on a specific location.
  * @param  character: Character to display.
  * @param  current_font: Font.
  * @param  char_color: Character color.
  * @param  x_pos: Col position.
  * @param  y_pos: Row position.
  * @retval None
*/
void lcd_draw_char(lcd_t *lcd,
                   wchar_t character,
                   const uint8_t *font,
                   color_t *char_color, color_t *bg_color,
                   uint16_t *x_pos, uint16_t *y_pos)
{
  uint32_t line = 0x00000000;
  uint32_t mask = 0x00000001;

  uint32_t font_starting_char;          // Font array starting character (character number == 0)
  uint32_t font_ending_char;            // Font array ending character
  uint32_t character_number;            // Character number in font array

  uint32_t character_properties_index;  // Character properties position in font array
  uint32_t character_height;            // Character height in pixels
  uint32_t character_width;             // Character width in pixels
  uint32_t character_position;          // Character drawing position in font array

  int8_t width_calc;                    // Variables used to calculate number of bytes per horizontal line
  uint8_t bytes_per_line = 0;           //

  /* Check if character is in font array */
  font_starting_char = ((uint32_t) font[3] << 8) | (uint32_t) font[2];
  font_ending_char   = ((uint32_t) font[5] << 8) | (uint32_t) font[4];

  /* If the character is off limits do not print anything */
  if ((character < font_starting_char) ||
      (character > font_ending_char))
  {
    return;
  };

  /* Search character in font array */
  character_number = character - font_starting_char;

  /* Get character properties */
  character_properties_index = 8 + 4*character_number;
  character_height = font[6];
  character_width = font[character_properties_index];
  character_position = ((font[character_properties_index + 3]) << 16) |
                       ((font[character_properties_index + 2]) << 8)  |
                        (font[character_properties_index + 1]);

  /* Set character drawing zone */
  lcd_hal_set_drawing_address(lcd,
                              *x_pos,
                              *x_pos + character_width - 1,
                              *y_pos,
                              *y_pos + character_height - 1);

  /* Iterate: for each line... */
  for (int i = 0; i < character_height; i++)
  {
    /* Retrieve char line bits */
    line = 0x00000000;    // Reset line
    bytes_per_line = 0;
    for (width_calc = character_width; width_calc > 0; width_calc -= 8)
    {
      line = (line) | (font[character_position++] << (8*bytes_per_line));
      bytes_per_line++;
    }

    /* Read font array and send line pixels */
    mask = 0x00000001;
    for (int j = 0; j < character_width; j++)
    {
      if(line & mask)
      {
        lcd_hal_write_data(lcd, char_color->color565);
      }
      else
      {
        lcd_hal_write_data(lcd, bg_color->color565);
      }
      mask = mask << 1;
    }
  }

  /* Update x position */
  *x_pos += character_width;
}

/**
  * @brief  Draws a string on a specific location.
  * @param  string: String to display.
  * @param  current_font: Font.
  * @param  char_color: Character color.
  * @param  x_pos: Col position.
  * @param  y_pos: Row position.
  * @retval None
*/
void lcd_draw_string(lcd_t *lcd,
                     wchar_t *string,
                     const uint8_t *current_font,
                     color_t *char_color, color_t *bg_color,
                     uint16_t x_pos, uint16_t y_pos)
{
  wchar_t character;
  uint32_t char_index = 0;
  uint16_t x_pos_sweep = x_pos;

  /* While there is still a character left in the string to be printed... */
  while ((character = string[char_index++]) != 0)
  {
    lcd_draw_char(lcd,
                  character,
                  current_font,
                  char_color, bg_color,
                  &x_pos_sweep, &y_pos);
  }
}

/**
  * @brief  Paints the whole screen with a specific color.
  * @param  bg_color: Screen color.
  * @retval None
*/
void lcd_draw_background(lcd_t *lcd, color_t *bg_color)
{
  lcd_draw_rectangle(lcd,
                     0, lcd->lcd_x_size,
                     0, lcd->lcd_y_size,
                     bg_color);
}