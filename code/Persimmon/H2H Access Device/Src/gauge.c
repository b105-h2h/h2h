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
uint8_t gauge_param_design_capacity[]      = {0x04, 0x1A};
uint8_t gauge_param_design_energy[]        = {0x0F, 0x2D};
uint8_t gauge_param_terminate_voltage[]    = {0x0D, 0xAC};
uint8_t gauge_param_reserve_capacity[]     = {0x00, 0x0A};
uint8_t gauge_param_taper_rate[]           = {0x00, 0xBB};
uint8_t gauge_operation_config[]           = {0x25, 0xF8};

/**
  * @brief  Initializes FG module
  * @retval None
*/
void gauge_init(gauge_t *gauge, I2C_HandleTypeDef *hi2c)
{
  uint16_t flags;

  /* Initialize gauge structure */
  gauge_hal_init(gauge, hi2c);

  /* Unseal device */
  gauge_hal_write_reg16(gauge, FG_CNTL, FG_UNSEAL);
  gauge_hal_write_reg16(gauge, FG_CNTL, FG_UNSEAL);     // Send unseal key twice

  /* Enter config update mode */
  gauge_hal_write_reg16(gauge, FG_CNTL, FG_CNTL_SET_CFGUPDATE);
  do
  {
    gauge_hal_read_reg16 (gauge, FG_FLAGS, &flags);
  }
  while (!(flags & 0x0010));        // Wait for CFGUPMODE bit to be set

  /* Update parameters */
  gauge_hal_ram_update(gauge, 82, 10, gauge_param_design_capacity, 2);
  gauge_hal_ram_update(gauge, 82, 12, gauge_param_design_energy, 2);
  gauge_hal_ram_update(gauge, 82, 16, gauge_param_terminate_voltage, 2);
  gauge_hal_ram_update(gauge, 82, 3,  gauge_param_reserve_capacity, 2);
  gauge_hal_ram_update(gauge, 82, 27, gauge_param_taper_rate, 2);
  gauge_hal_ram_update(gauge, 64, 0,  gauge_operation_config, 2);

  /* Exit config update mode (performing a soft reset)*/
  gauge_hal_write_reg16(gauge, FG_CNTL, FG_CNTL_SOFT_RESET);
  do
  {
    gauge_hal_read_reg16 (gauge, FG_FLAGS, &flags);
  }
  while (flags & 0x0010);           // Wait for CFGUPMODE bit to be cleared

  /* Seal device */
  gauge_hal_write_reg16(gauge, FG_CNTL, FG_CNTL_SEALED);
}

void gauge_start_read(gauge_t *gauge)
{
  uint8_t initial_reg = FG_CNTL;

  HAL_I2C_Master_Transmit(gauge->hi2c, FG_I2C_ADDRESS, &initial_reg, 1, 100);
  HAL_I2C_Master_Receive_DMA(gauge->hi2c, FG_I2C_ADDRESS, gauge->last_data_buf, 34);
}

void gauge_format_data(gauge_t *gauge)
{
  gauge->last_data.cntl  = ((uint32_t) gauge->last_data_buf[FG_CNTL + 1] << 8) |
                           ((uint32_t) gauge->last_data_buf[FG_CNTL]);
  gauge->last_data.flags = ((uint32_t) gauge->last_data_buf[FG_FLAGS + 1] << 8) |
                           ((uint32_t) gauge->last_data_buf[FG_FLAGS]);

  gauge->last_data.temp = ((int32_t) ((int8_t) gauge->last_data_buf[FG_TEMP + 1] << 8)) |
                          ((int32_t) gauge->last_data_buf[FG_TEMP]);
  gauge->last_data.volt = ((int32_t) ((int8_t) gauge->last_data_buf[FG_VOLT + 1] << 8)) |
                          ((int32_t) gauge->last_data_buf[FG_VOLT]);

  gauge->last_data.nom_avail_cap  = ((uint32_t) gauge->last_data_buf[FG_NOM_AVAIL_CAP + 1] << 8) |
                                    ((uint32_t) gauge->last_data_buf[FG_NOM_AVAIL_CAP]);
  gauge->last_data.full_avail_cap = ((uint32_t) gauge->last_data_buf[FG_FULL_AVAIL_CAP + 1] << 8) |
                                    ((uint32_t) gauge->last_data_buf[FG_FULL_AVAIL_CAP]);
  gauge->last_data.remaining_cap  = ((uint32_t) gauge->last_data_buf[FG_REMAINING_CAP + 1] << 8) |
                                    ((uint32_t) gauge->last_data_buf[FG_REMAINING_CAP]);
  gauge->last_data.full_chg_cap   = ((uint32_t) gauge->last_data_buf[FG_FULL_CHG_CAP + 1] << 8) |
                                    ((uint32_t) gauge->last_data_buf[FG_FULL_CHG_CAP]);

  gauge->last_data.avg_current       = ((int32_t) ((int8_t) gauge->last_data_buf[FG_AVG_CURRENT + 1] << 8)) |
                                       ((int32_t) gauge->last_data_buf[FG_AVG_CURRENT]);
  gauge->last_data.standby_current   = ((int32_t) ((int8_t) gauge->last_data_buf[FG_STANDBY_CURRENT + 1] << 8)) |
                                       ((int32_t) gauge->last_data_buf[FG_STANDBY_CURRENT]);
  gauge->last_data.max_load_current  = ((int32_t) ((int8_t) gauge->last_data_buf[FG_MAX_LOAD_CURRENT + 1] << 8)) |
                                       ((int32_t) gauge->last_data_buf[FG_MAX_LOAD_CURRENT]);
  gauge->last_data.avg_power         = ((int32_t) ((int8_t) gauge->last_data_buf[FG_AVG_POWER + 1] << 8)) |
                                       ((int32_t) gauge->last_data_buf[FG_AVG_POWER]);

  gauge->last_data.soc        = ((uint32_t) gauge->last_data_buf[FG_SOC + 1] << 8) |
                                ((uint32_t) gauge->last_data_buf[FG_SOC]);
  gauge->last_data.soh        = ((uint32_t) gauge->last_data_buf[FG_SOH]);
  gauge->last_data.soh_status = ((uint32_t) gauge->last_data_buf[FG_SOH + 1]);

}
