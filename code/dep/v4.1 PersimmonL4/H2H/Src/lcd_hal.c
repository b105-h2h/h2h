#include "lcd_hal.h"

void lcd_hal_init(lcd_t *lcd,
                  SRAM_HandleTypeDef *hsram, uint32_t *lcd_reg, uint32_t *lcd_data)
{
  /* Associate with fsmc handle */
  lcd->hsram = hsram;
  lcd->lcd_reg = lcd_reg;
  lcd->lcd_data = lcd_data;
}

/**
  * @brief  Writes one command.
  * @param  reg: Command to send.
  * @retval None
*/
void lcd_hal_write_command(lcd_t *lcd, uint16_t command)
{
  /* Send command */
  *(uint16_t *)lcd->lcd_reg = command;
}

/**
  * @brief  Writes one word of data.
  * @param  reg: Data to send.
  * @retval None
*/
void lcd_hal_write_data(lcd_t *lcd, uint16_t data)
{
  /* Send data */
  *(uint16_t *)lcd->lcd_data = data;
}

/**
  * @brief  Reads n params of a register.
  * @param  reg: Address of the lcd register to read.
  * @param  param_buffer: Array to store all parameters.
  * @param  param_buffer_size: Number of parameters to read.
  * @retval None
*/
void lcd_hal_read_reg(lcd_t *lcd,
                      uint16_t reg,
                      uint16_t *param_buffer, uint32_t param_buffer_size)
{
  uint32_t i;

  /* Send register */
  *(uint16_t *)lcd->lcd_reg = reg;

  /* Get parameters */
  for (i = 0; i < param_buffer_size; i++)
  {
    param_buffer[i] = *(uint16_t *)lcd->lcd_data;
  }
}

/**
  * @brief  Writes n params of a register.
  * @param  reg: Address of the lcd register to read.
  * @param  param_buffer: Array to store all parameters.
  * @param  param_buffer_size: Number of parameters to write.
  * @retval None
*/
void lcd_hal_write_reg(lcd_t *lcd,
                       uint16_t reg,
                       uint16_t *param_buffer, uint32_t param_buffer_size)
{
  uint32_t i;

  /* Send register */
  *(uint16_t *)lcd->lcd_reg = reg;

  /* Send parameters */
  for (i = 0; i < param_buffer_size; i++)
  {
    *(uint16_t *)lcd->lcd_data = param_buffer[i];
  }
}

/**
  * @brief  Sets the portion of the frame buffer to be accessed sequentially.
  * @param  start_col: Start column (x axis).
  * @param  end_col: End column (x axis).
  * @param  start_row: Start row (y axis).
  * @param  end_row: End row (y axis).
  * @retval None
*/
void lcd_hal_set_drawing_address(lcd_t *lcd,
                                 uint16_t start_col, uint16_t end_col,
                                 uint16_t start_row, uint16_t end_row)
{
  uint16_t pixel_column_address[4], pixel_page_address[4];

  /* Prepare x_pos to be sent as SET_COLUMN_ADDRESS parameters */
  pixel_column_address[0] = (start_col >> 8) & 0x00FF;
  pixel_column_address[1] =  start_col       & 0x00FF;
  pixel_column_address[2] = (end_col >> 8)   & 0x00FF;
  pixel_column_address[3] =  end_col         & 0x00FF;

  /* Prepare y_pos to be sent as SET_PAGE_ADDRESS parameters */
  pixel_page_address[0]   = (start_row >> 8) & 0x00FF;
  pixel_page_address[1]   =  start_row       & 0x00FF;
  pixel_page_address[2]   = (end_row >> 8)   & 0x00FF;
  pixel_page_address[3]   =  end_row         & 0x00FF;

  /* Send boundary addresses */
  lcd_hal_write_reg(lcd, LCD_SET_COLUMN_ADDRESS, pixel_column_address, 4);
  lcd_hal_write_reg(lcd, LCD_SET_PAGE_ADDRESS, pixel_page_address, 4);

  /* Send ready to write command */
  lcd_hal_write_command(lcd, LCD_WRITE_MEMORY_START);
}

