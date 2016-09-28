/*
 * afe_hal.h
 */

#ifndef AFE_HAL_H_
#define AFE_HAL_H_

#include <stdint.h>
#include "spi.h"
#include "gpio.h"

#include "afe_constants.h"

typedef enum afe_connected_t
{
  IS_CONNECTED = 0,
  IS_NOT_CONNECTED  = 1
}
afe_connected_t;

typedef struct afe_data_t
{
  afe_connected_t rld_lead_off;
  afe_connected_t in2n_lead_off;
  afe_connected_t in2p_lead_off;
  afe_connected_t in1n_lead_off;
  afe_connected_t in1p_lead_off;

  int32_t ch1_data;     // In microvolts
  int32_t ch2_data;     // In microvolts
}
afe_data_t;

typedef struct afe_t
{
  /* Hardware/memory map location */
  SPI_HandleTypeDef *hspi;

  /* Gains */
  uint8_t afe_gain_ch1;
  uint8_t afe_gain_ch2;

  /* Last data received */
  uint8_t last_data_buf[9];
  afe_data_t last_data;
}
afe_t;

void afe_hal_init(afe_t *afe, SPI_HandleTypeDef *hspi);

void afe_hal_write_register(afe_t *afe, uint8_t reg, uint8_t data);
void afe_hal_read_register(afe_t *afe, uint8_t reg, uint8_t *data);
void afe_hal_write_command(afe_t *afe, uint8_t command);

#endif /* AFE_HAL_H_ */