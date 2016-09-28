/*
 * touch.c
 *
 *  Created on: 24/05/2016
 *      Author: Smau
 */
#include "touch.h"

/**
  * @brief  Initializes touch module
  * @retval None
*/
void touch_init(touch_t *touch, SPI_HandleTypeDef *hspi,
                lcd_t *lcd,
                uint16_t touch_x_size, uint16_t touch_y_size)
{
  touch_calib_t calib;
  uint8_t cntl_byte = 0;
  
  touch_hal_init(touch, hspi);
  
  /* Set touch dimensions */
  touch->touch_x_size = touch_x_size;
  touch->touch_y_size = touch_y_size;
  
  /* Coarse calibration values */
  calib.alpha_x = (lcd->lcd_x_size << 12) / touch->touch_x_size;
  calib.beta_x = 0;
  calib.delta_x = 0;
  
  calib.alpha_y = 0;
  calib.beta_y = (lcd->lcd_y_size << 12) / touch->touch_y_size;;
  calib.delta_y = 0;
  
  *touch->calib = calib;
  
  /* Reset click */
  touch->is_clicked = T_NOT_CLICKED;
  
  /* First control byte */
  cntl_byte = TOUCH_CNTL_S | TOUCH_CNTL_X |
              TOUCH_CNTL_MODE_12_BIT |
              TOUCH_CNTL_DFR | TOUCH_CNTL_PD;
  
  /* Send byte */
  touch_hal_read(touch, cntl_byte);
}

void touch_read_position(touch_t *touch)
{
  uint8_t cntl_byte = 0;
  uint32_t z1, z2;
  uint32_t tmp_pressure_cal;

  /* Get X param */
  cntl_byte = TOUCH_CNTL_S | TOUCH_CNTL_X |
              TOUCH_CNTL_MODE_12_BIT |
              TOUCH_CNTL_DFR | TOUCH_CNTL_PD;
  touch->current_pos.x_pos = touch_hal_read(touch, cntl_byte);
  
  /* Get Y param */
  cntl_byte = TOUCH_CNTL_S | TOUCH_CNTL_Y |
              TOUCH_CNTL_MODE_12_BIT |
              TOUCH_CNTL_DFR | TOUCH_CNTL_PD;
  touch->current_pos.y_pos = touch_hal_read(touch, cntl_byte);
  
  /* Get Z1 param */
  cntl_byte = TOUCH_CNTL_S | TOUCH_CNTL_Z1 |
              TOUCH_CNTL_MODE_12_BIT |
              TOUCH_CNTL_DFR | TOUCH_CNTL_PD;
  z1 = touch_hal_read(touch, cntl_byte);
  
  /* Get Z2 param */
  cntl_byte = TOUCH_CNTL_S | TOUCH_CNTL_Z2 |
              TOUCH_CNTL_MODE_12_BIT |
              TOUCH_CNTL_DFR | TOUCH_CNTL_PD;
  z2 = touch_hal_read(touch, cntl_byte);
  
  /* Calculate and store pressure level
   * http://www.unclelarry.com/?p=101
   */
  tmp_pressure_cal = (touch->current_pos.x_pos * (((z2 << 12) / z1) - 4096)) >> 12;
  
  /* Check tmp_pressure_cal values */
  if (tmp_pressure_cal == 0)
  {
    /* If there is nothing touching the screen
     * the controller sends a maximum pressure value
     * (i.e. tmp_pressure_cal = 0).
     * 
     * When that happens, store a minimum pressure
     * value instead.
     */
    tmp_pressure_cal = TOUCH_PRESSURE_THRESHOLD;
  }
  else if (tmp_pressure_cal > TOUCH_PRESSURE_THRESHOLD)
  {
    /* If tactile pressure doesn't reach our
     * defined minimum pressure value, store a 
     * minimum pressure value.
     */
    tmp_pressure_cal = TOUCH_PRESSURE_THRESHOLD;
  }
  
  /* Rearrange values to make position->pressure
   * higher with higher pressure values
   */
  touch->current_pos.pressure = TOUCH_PRESSURE_THRESHOLD - tmp_pressure_cal;

  /* Check and store 'screen is clicked' condition */
  if (touch->current_pos.pressure > 0)
  {
    touch->is_clicked = T_CLICKED;
  }
  else
  {
    touch->is_clicked = T_NOT_CLICKED;
  }
}
