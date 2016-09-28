/*
 * gauge.c
 *
 *  Created on: 10/05/2016
 *      Author: Smau
 */
#include "gauge.h"

/* 
 * Fuel gauge design parameters
 *
 * DESIGN_CAPACITY      1050   mAh
 * DESIGN_ENERGY        3885   mWh
 * TERMINATE_VOLTAGE    3500   mV
 * RESERVE_CAPACITY     10     mAh
 * TAPER_RATE           187    0.1 h
 * TAPER_CURRENT        56     mA
 */
uint8_t fg_param_design_capacity[]      = {0x04, 0x1A};
uint8_t fg_param_design_energy[]        = {0x0F, 0x2D};
uint8_t fg_param_terminate_voltage[]    = {0x0D, 0xAC};
uint8_t fg_param_reserve_capacity[]     = {0x00, 0x0A};
uint8_t fg_param_taper_rate[]           = {0x00, 0xBB};

uint8_t fg_operation_config[]           = {0x25, 0xF8};

void fg_ram_update(uint8_t data_subclass, uint8_t data_offset,
                   uint8_t *data_buffer, uint8_t data_size)
{
  uint8_t data_block_offset;
  uint8_t old_checksum, tmp_checksum, checksum;
  uint8_t old_data_buffer[4];
  uint32_t i;
  
  /* Enable block data memory control */
  fg_write_reg8(FG_BLOCK_DATA_CNTL, 0x00);
  
  /* Access subclass */
  fg_write_reg8(FG_DATA_CLASS, data_subclass);
  
  /* Access subclass with offset */
  if (data_offset < 32)
  {
    data_block_offset = 0x00;
  }
  else
  {
    data_block_offset = 0x01;
  }
  fg_write_reg8(FG_DATA_BLOCK, data_block_offset);
  
  /* Read old checksum */
  old_checksum = fg_read_reg8(FG_BLOCK_DATA_CHECK_SUM);

  /* Read old values */
  for (i = 0; i < data_size; i++)
  {
    old_data_buffer[i] = fg_read_reg8(0x40 + (data_offset % 32) + i);
  }
  
  /* Write new values */
  for (i = 0; i < data_size; i++)
  {
    fg_write_reg8(0x40 + (data_offset % 32) + i, data_buffer[i]);
  }
  
  /* 
   * Compute new checksum
   *
   * This calculation includes some modulo 256 operations
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
  fg_write_reg8(FG_BLOCK_DATA_CHECK_SUM, checksum);
}

/**
  * @brief  Initializes FG module
  * @retval None
*/
void fg_init()
{
  uint16_t flags;

  /* Unseal device */
  fg_write_reg16(FG_CNTL, FG_UNSEAL);
  fg_write_reg16(FG_CNTL, FG_UNSEAL);                   // Send unseal key twice

  /* Enter config update mode */
  fg_write_reg16(FG_CNTL, FG_CNTL_SET_CFGUPDATE);
  do
  {
    flags = fg_read_reg16(FG_FLAGS);
  }
  while (!(flags & 0x0010));        // Wait for CFGUPMODE bit to be set
  
  /* Update parameters */
  fg_ram_update(82, 10, fg_param_design_capacity, 2);
  fg_ram_update(82, 12, fg_param_design_energy, 2);
  fg_ram_update(82, 16, fg_param_terminate_voltage, 2);
  fg_ram_update(82, 3,  fg_param_reserve_capacity, 2);
  fg_ram_update(82, 27, fg_param_taper_rate, 2);
  fg_ram_update(64, 0,  fg_operation_config, 2);
  
  /* Exit config update mode (performing a soft reset)*/
  fg_write_reg16(FG_CNTL, FG_CNTL_SOFT_RESET);
  do
  {
    flags = fg_read_reg16(FG_FLAGS);
  }
  while (flags & 0x0010);           // Wait for CFGUPMODE bit to be cleared
  
  /* Seal device */
  fg_write_reg16(FG_CNTL, FG_CNTL_SEALED);
}

uint8_t fg_read_reg8(uint8_t reg)
{
  uint8_t param_buffer;

  HAL_I2C_Master_Transmit(&hi2c1, FG_I2C_ADDRESS, &reg, 1, 100);
  HAL_I2C_Master_Receive(&hi2c1, FG_I2C_ADDRESS, &param_buffer, 2, 100);

  HAL_Delay(1);
  
  /* Recieve one byte of data from a register position */
  return param_buffer;
}

void fg_write_reg8(uint8_t reg, uint8_t param)
{
  uint8_t param_buffer[2];

  param_buffer[0] = reg;
  param_buffer[1] = param;

  /* Send register position, and one byte of data */
  HAL_I2C_Master_Transmit(&hi2c1, FG_I2C_ADDRESS, param_buffer, 2, 100);
  
  HAL_Delay(1);
}

uint16_t fg_read_reg16(uint8_t reg)
{
  uint8_t param_buffer[2];

  HAL_I2C_Master_Transmit(&hi2c1, FG_I2C_ADDRESS, &reg, 1, 100);
  HAL_I2C_Master_Receive(&hi2c1, FG_I2C_ADDRESS, param_buffer, 2, 100);

  HAL_Delay(1);
  
  /* Recieve two bytes of data from a register position */
  return (((uint16_t) param_buffer[1]) << 8) | ((uint16_t) param_buffer[0]);
}

void fg_write_reg16(uint8_t reg, uint16_t param)
{
  uint8_t param_buffer[3];

  param_buffer[0] = reg;
  param_buffer[1] = (uint8_t)  (param & 0x00FF);         // param LSB
  param_buffer[2] = (uint8_t) ((param & 0xFF00) >> 8);   // param MSB

  /* Send register position, and two bytes of data */
  HAL_I2C_Master_Transmit(&hi2c1, FG_I2C_ADDRESS, param_buffer, 3, 100);
  
  HAL_Delay(1);
}