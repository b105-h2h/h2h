#include "lcd_primitives.h"

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
  
  /* Send pixel color */
  lcd_hal_write_command(lcd, LCD_WRITE_MEMORY_START);
  lcd_hal_write_data(lcd, color->color565);
}

/* Function that returns -1,0,1 depending on whether x */
/* is <0, =0, >0 respectively */
#define sign(x) ((x>0)?1:((x<0)?-1:0))

/**
  * @brief  Draws a line from two points.
  * @param  x0: Starting point horizontal position.
  * @param  y0: Starting point vertical position.
  * @param  x1: Finishing point horizontal position.
  * @param  y1: Finishing point vertical position.
  * @param  color: Color of pixel.
  * @retval None
*/
void lcd_draw_line(lcd_t *lcd, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, color_t *color)
{
  // Source: http://code-blocks.blogspot.com.es/2014/12/generalized-bresenhams-line-drawing.html
  
  int32_t dx, dy;
  int32_t d, sx, sy, temp;
  uint16_t x, y;
  uint8_t swap = 0;

  /* Calculate abs value of dx and dy */
  if ((x1 - x0) >= 0)
  {
    dx = x1 - x0;
  }
  else
  {
    dx = x0 - x1;
  }
  
  if ((y1 - y0) >= 0)
  {
    dy = y1 - y0;
  }
  else
  {
    dy = y0 - y1;
  }
  
  /* Calculate signs */
  sx = sign(x1-x0);
  sy = sign(y1-y0);

  /* Check if dx or dy has a greater range */
  /* if dy has a greater range than dx swap dx and dy */
  if (dy > dx)
  {
    temp = dx;
    dx = dy;
    dy = temp;
    
    swap = 1;
  }

  /* Set the initial decision parameter and the initial point */
  d = (2 * dy) - dx;
  x = x0;
  y = y0;

  for(uint32_t i = 1; i <= dx; i++)
  {
    lcd_draw_pixel(lcd, x, y, color);
    while (d >= 0) 
    {
      if (swap)
      {
        x = x + sx;
      }
      else 
      {
        y = y + sy;
        d = d - 2* dx;
      }
    }
    if (swap)
    {
      y = y + sy;
    }
    else
    {
      x = x + sx;
    }
    d = d + (2 * dy);
  }
}

void lcd_draw_rectangle(lcd_t *lcd,
                        uint16_t x_pos, uint16_t width,
                        uint16_t y_pos, uint16_t height,
                        color_t *color)
{
  /* Send screen pixel limits */
  lcd_hal_set_drawing_address(lcd, x_pos, x_pos + width - 1,
                                   y_pos, y_pos + height - 1);
  
  /* Write pixels */
  lcd_hal_write_command(lcd, LCD_WRITE_MEMORY_START);
  for (int i = 0; i < height; i++)
  {
    for (int j = 0; j < width; j++)
    {
      HAL_SRAM_Write_16b(&hsram1, lcd->lcd_data, &(color->color565), 1);
    }
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
                   char character,
                   const uint8_t *font, 
                   color_t *char_color, color_t *bg_color,
                   uint16_t *x_pos, uint16_t *y_pos)
{
  // Hardcoded -> TO-DO: build font structure
  
  uint32_t line = 0x00000000;
  uint32_t mask = 0x00000001;  
  
  uint8_t font_starting_char;           // Font array starting character (character number == 0)
  uint8_t character_number;             // Character number in font array
  
  uint32_t character_properties_index;  // Character properties position in font array
  uint8_t  character_height;            // Character height in pixels
  uint8_t  character_width;             // Character width in pixels
  uint32_t character_position;          // Character drawing position in font array
  
  int8_t width_calc;                    // Variables used to calculate number of bytes per horizontal line
  uint8_t bytes_per_line = 0;           //
  
  /* Search character in font array */
  font_starting_char = font[2];
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
  
  /* Send character pixels */
  lcd_hal_write_command(lcd, LCD_WRITE_MEMORY_START);
  
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
    
    /* Read font array and send pixels of the line*/
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
                     char *string,
                     const uint8_t *current_font, 
                     color_t *char_color, color_t *bg_color,
                     uint16_t x_pos, uint16_t y_pos)
{
  uint32_t char_index = 0;
  char character;
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
  /* Set value in lcd structure */
  lcd->bg_color = *bg_color;

  /* Send screen pixel limits */
  lcd_hal_set_drawing_address(lcd,
                              0,
                              lcd->lcd_x_size - 1,
                              0,
                              lcd->lcd_y_size - 1);

  /* Write pixels */
  lcd_hal_write_command(lcd, LCD_WRITE_MEMORY_START);
  for (int i = 0; i < lcd->lcd_y_size; i++)
  {
    for (int j = 0; j < lcd->lcd_x_size; j++)
    {
      HAL_SRAM_Write_16b(&hsram1, lcd->lcd_data, &(bg_color->color565), 1);
    }
  }
}