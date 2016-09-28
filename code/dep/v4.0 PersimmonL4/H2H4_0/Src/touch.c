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
                uint16_t touch_x_size, uint16_t touch_y_size)
{
  uint8_t cntl_byte = 0;

  touch_hal_init(touch, hspi);

  /* Disable CS */
  HAL_GPIO_WritePin(GPIOB, TP_CS_Pin, GPIO_PIN_SET);    // Disable CS

  /* Set touch dimensions */
  touch->touch_x_size = touch_x_size;
  touch->touch_y_size = touch_y_size;

  /* Coarse calibration values */
  touch->calib.alpha_x = (800 << 12) / touch->touch_x_size;
  touch->calib.beta_x = 0;
  touch->calib.delta_x = 0;

  touch->calib.alpha_y = 0;
  touch->calib.beta_y = (480 << 12) / touch->touch_y_size;;
  touch->calib.delta_y = 0;

  /* Reset click */
  touch->is_clicked = T_NOT_CLICKED;

  /* First control byte */
  cntl_byte = TOUCH_CNTL_S | TOUCH_CNTL_X |
              TOUCH_CNTL_MODE_12_BIT |
              TOUCH_CNTL_DFR | TOUCH_CNTL_PD;

  /* Send byte */
  touch_hal_read(touch, cntl_byte);

  /* Clear pending interrupts and enable them */
  __HAL_GPIO_EXTI_CLEAR_IT(TP_PEN_Pin);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
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

lcd_pos_t touch_get_lcd_pos(touch_t *touch)
{
  lcd_pos_t lcd_pos;

  /* Calculate lcd position from touchpad position
   *
   * A 12 bit shift is needed because calibration
   * values are stored in Q20.12 fixed point format
   */
  lcd_pos.x_pos = (touch->calib.alpha_x * touch->current_pos.x_pos +
                   touch->calib.beta_x  * touch->current_pos.y_pos +
                   touch->calib.delta_x) >> 12;
  lcd_pos.y_pos = (touch->calib.alpha_y * touch->current_pos.x_pos +
                   touch->calib.beta_y  * touch->current_pos.y_pos +
                   touch->calib.delta_y) >> 12;
  return lcd_pos;
}

//}