#include "touch_hal.h"

/*
 * Touchpad low level functions
 *
 */

void touch_hal_init(touch_t *touch,
                    SPI_HandleTypeDef *hspi)
{
  /* Associate with spi handle */
  touch->hspi = hspi;
}

uint32_t touch_hal_read(touch_t *touch, uint8_t cntl_byte)
{
  uint8_t data_buffer[2];
  uint8_t dummy[2] = {0x00, 0x00};
  uint32_t data;

  /* Enable CS */
  HAL_GPIO_WritePin(GPIOB, TP_CS_Pin, GPIO_PIN_RESET);  // Enable CS

  /* Send control byte */
  HAL_SPI_Transmit(touch->hspi, &cntl_byte, 1, 100);

  /* Recieve data */
  HAL_SPI_TransmitReceive(touch->hspi, dummy, data_buffer, 2, 100);

  /* Disable CS */
  HAL_GPIO_WritePin(GPIOB, TP_CS_Pin, GPIO_PIN_SET);    // Disable CS

  data = (((uint32_t) data_buffer[0]) << 5) |
         (((uint32_t) data_buffer[1]) >> 3);

  return data;
}