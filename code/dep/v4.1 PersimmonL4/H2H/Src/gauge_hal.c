#include "gauge_hal.h"

void gauge_hal_init(gauge_t *gauge, I2C_HandleTypeDef *hi2c)
{
  /* Associate with i2c handle */
  gauge->hi2c = hi2c;
}

void gauge_hal_write_reg8(gauge_t *gauge, uint8_t reg, uint8_t param)
{
  uint8_t param_buffer[2];

  param_buffer[0] = reg;
  param_buffer[1] = param;

  /* Send register position, and one byte of data */
  HAL_I2C_Master_Transmit(gauge->hi2c, FG_I2C_ADDRESS, param_buffer, 2, 100);
}

void gauge_hal_write_reg16(gauge_t *gauge, uint8_t reg, uint16_t param)
{
  uint8_t param_buffer[3];

  param_buffer[0] = reg;
  param_buffer[1] = (uint8_t)  (param & 0x00FF);         // param LSB
  param_buffer[2] = (uint8_t) ((param & 0xFF00) >> 8);   // param MSB

  /* Send register position, and two bytes of data */
  HAL_I2C_Master_Transmit(gauge->hi2c, FG_I2C_ADDRESS, param_buffer, 3, 100);
}

void gauge_hal_read_reg8(gauge_t *gauge, uint8_t reg, uint8_t *param)
{
  HAL_I2C_Master_Transmit(gauge->hi2c, FG_I2C_ADDRESS, &reg, 1, 100);
  HAL_I2C_Master_Receive(gauge->hi2c, FG_I2C_ADDRESS, param, 1, 100);
}

void gauge_hal_read_reg16(gauge_t *gauge, uint8_t reg, uint16_t *param)
{
  uint8_t param_buffer[2];

  HAL_I2C_Master_Transmit(gauge->hi2c, FG_I2C_ADDRESS, &reg, 1, 100);
  HAL_I2C_Master_Receive(gauge->hi2c, FG_I2C_ADDRESS, param_buffer, 2, 100);

  /* Recieve two bytes of data from a register position */
  *param = (((uint16_t) param_buffer[1]) << 8) | ((uint16_t) param_buffer[0]);
}

void gauge_hal_ram_update(gauge_t *gauge,
                          uint8_t data_subclass, uint8_t data_offset,
                          uint8_t *data_buffer,  uint8_t data_size)
{
  uint8_t data_block_offset;
  uint8_t old_checksum, tmp_checksum, checksum;
  uint8_t old_data_buffer[4];
  uint32_t i;

  /* Enable block data memory control */
  gauge_hal_write_reg8(gauge, FG_BLOCK_DATA_CNTL, 0x00);

  /* Access subclass */
  gauge_hal_write_reg8(gauge, FG_DATA_CLASS, data_subclass);

  /* Access subclass with offset */
  if (data_offset < 32)
  {
    data_block_offset = 0x00;
  }
  else
  {
    data_block_offset = 0x01;
  }
  gauge_hal_write_reg8(gauge, FG_DATA_BLOCK, data_block_offset);

  /* Read old checksum */
  gauge_hal_read_reg8(gauge, FG_BLOCK_DATA_CHECK_SUM, &old_checksum);

  /* Read old values */
  for (i = 0; i < data_size; i++)
  {
    gauge_hal_read_reg8(gauge, (0x40 + (data_offset % 32) + i), &(old_data_buffer[i]));
  }

  /* Write new values */
  for (i = 0; i < data_size; i++)
  {
    gauge_hal_write_reg8(gauge, (0x40 + (data_offset % 32) + i), data_buffer[i]);
  }

  /*
   * Compute new checksum
   *
   * This computation includes some modulo 256 operations
   * but we leave them out as we are working with uint8_t
   * variables, which intrinsically perform this operation
   * as they only can store numbers between 0 and 255.
   *
   * Please refer to page 14 of BQ27441-G1 Technical Reference
   * for further information on how to make this computation
   */
  tmp_checksum = 255 - old_checksum;
  for (i = 0; i < data_size; i++)
  {
    tmp_checksum -= old_data_buffer[i];
  }
  for (i = 0; i < data_size; i++)
  {
    tmp_checksum += data_buffer[i];
  }
  checksum = 255 - tmp_checksum;

  /* Write new checksum */
  gauge_hal_write_reg8(gauge, FG_BLOCK_DATA_CHECK_SUM, checksum);
}