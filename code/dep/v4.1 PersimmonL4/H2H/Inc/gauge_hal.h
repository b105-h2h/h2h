/*
 * gauge_hal.h
 */

#ifndef GAUGE_HAL_H_
#define GAUGE_HAL_H_

#include <stdint.h>

#include "i2c.h"
#include "gpio.h"

#include "gauge_constants.h"

typedef struct gauge_data_t
{
  uint32_t cntl;
  uint32_t flags;

  int32_t temp;                 // 0.1 K
  int32_t volt;                 // mV

  uint32_t nom_avail_cap;       // mAh
  uint32_t full_avail_cap;      // mAh
  uint32_t remaining_cap;       // mAh
  uint32_t full_chg_cap;        // mAh

  int32_t avg_current;          // mA
  int32_t standby_current;      // mA
  int32_t max_load_current;     // mA
  int32_t avg_power;            // mW

  uint32_t soc;                 // % (0 - 100)
  uint32_t soh;                 // %
  uint32_t soh_status;
}
gauge_data_t;

typedef struct gauge_t
{
  /* Hardware/memory map location */
  I2C_HandleTypeDef *hi2c;

  /* Last data received */
  uint8_t last_data_buf[34];
  gauge_data_t last_data;
}
gauge_t;

void gauge_hal_init(gauge_t *gauge, I2C_HandleTypeDef *hi2c);

void gauge_hal_write_reg8 (gauge_t *gauge, uint8_t reg, uint8_t   param);
void gauge_hal_write_reg16(gauge_t *gauge, uint8_t reg, uint16_t  param);

void gauge_hal_read_reg8  (gauge_t *gauge, uint8_t reg, uint8_t  *param);
void gauge_hal_read_reg16 (gauge_t *gauge, uint8_t reg, uint16_t *param);

void gauge_hal_ram_update(gauge_t *gauge,
                          uint8_t data_subclass, uint8_t data_offset,
                          uint8_t *data_buffer,  uint8_t data_size);

#endif /* GAUGE_HAL_H_ */