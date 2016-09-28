/*
 * afe_hal.c
 *
 *  Created on: 20/06/2016
 *      Author: slopez
 */


#include "afe_hal.h"

/*
 * AFE low level functions
 *
 */
void afe_hal_init(afe_t *afe, SPI_HandleTypeDef *hspi)
{
  /* Associate with spi handle */
  afe->hspi = hspi;

  /* Reset last data */
  afe->last_data.rld_lead_off  = IS_NOT_CONNECTED;
  afe->last_data.in2n_lead_off = IS_NOT_CONNECTED;
  afe->last_data.in2p_lead_off = IS_NOT_CONNECTED;
  afe->last_data.in1n_lead_off = IS_NOT_CONNECTED;
  afe->last_data.in1p_lead_off = IS_NOT_CONNECTED;

  afe->last_data.ch1_data = 0;
  afe->last_data.ch2_data = 0;

  afe->afe_gain_ch1 = 6;        /* Default gain */
  afe->afe_gain_ch2 = 6;        /* Default gain */
}

/**
  * @brief  Writes a single byte to one register
  * @param  reg: Address of the afe register to write
  * @param  data: Byte to be send
  * @retval None
*/
void afe_hal_write_register(afe_t *afe, uint8_t reg, uint8_t data)
{
  uint8_t buf[3] = {reg | AFE_WREG, 0x00, data};

  HAL_SPI_Transmit(afe->hspi, buf, 3, 100);
}

/**
  * @brief  Reads a single byte from one register
  * @param  reg: Address of the afe register to read
  * @param  data: Byte to be received
  * @retval None
*/
void afe_hal_read_register(afe_t *afe, uint8_t reg, uint8_t *data)
{
  uint8_t dummy = 0x00;
  uint8_t buf[2] = {reg | AFE_RREG, 0x00};

  HAL_SPI_Transmit(afe->hspi, buf, 2, 100);
  HAL_SPI_TransmitReceive(afe->hspi, &dummy, data, 1, 100);
}

/**
  * @brief  Writes a command
  * @param  command: Command to be issued
  * @retval None
*/
void afe_hal_write_command(afe_t *afe, uint8_t command)
{
  HAL_SPI_Transmit(afe->hspi, &command, 1, 100);
}