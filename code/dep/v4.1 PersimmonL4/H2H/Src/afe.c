#include "afe.h"

void afe_init(afe_t *afe, SPI_HandleTypeDef *hspi)
{
  uint8_t afe_id;

  /* Initialize AFE structure */
  afe_hal_init(afe, hspi);

  /* Disable DRDY interrupts */
  HAL_NVIC_DisableIRQ(EXTI1_IRQn);

  /*
   * AFE pins should be low on power up
   *
   * AFE_START
   * AFE_RESET
   */
  HAL_GPIO_WritePin(GPIOA, AFE_START_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOA, AFE_RESET_Pin, GPIO_PIN_RESET);

  /* Power up AFE */
  HAL_GPIO_WritePin(GPIOC, VDD_AFE_D_EN_Pin|VDD_AFE_A_EN_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOA, AFE_RESET_Pin, GPIO_PIN_SET);
  HAL_Delay(50);        // tPOR > (2^12*tMOD = 32 ms)

  /* Issue reset pulse */
  HAL_GPIO_WritePin(GPIOA, AFE_RESET_Pin, GPIO_PIN_RESET);
  HAL_Delay(1);		// tRST > (tMOD = 7.8125 us)
  HAL_GPIO_WritePin(GPIOA, AFE_RESET_Pin, GPIO_PIN_SET);
  HAL_Delay(1);         // t > (18*tCLK = 35.15625 us)

  /* Enable serial interface */
  HAL_GPIO_WritePin(GPIOA, AFE_CS_Pin, GPIO_PIN_RESET); //Enable CS
  HAL_Delay(1);         // t > (4*tCLK = 7.8125 us)

  /* Stop read data continuous: Send SDATAC so registers can be accessed */
  afe_hal_write_command(afe, AFE_SDATAC);
  HAL_Delay(1);         // t > (4*tCLK = 7.8125 us)

  /* Enable internal reference and wait for it to settle */
  afe_hal_write_register(afe, AFE_REG_CONFIG2, 0xA0);
  HAL_Delay(100);       // Reference start-up time = 100 ms

  /* Check AFE id */
  afe_hal_read_register(afe, AFE_REG_ID, &afe_id);
  if (1) //afe_id == 0x53)
  {
    /* Write configuration */
      /*
       * Continuous conversion mode
       * 500 SPS
       */
      afe_hal_write_register(afe, AFE_REG_CONFIG1, 0x02);
      /*
       * Lead-off comparator disabled
       * Internal reference enabled
       * 2.42 V reference
       *
       * Oscillator clock output disabled
       * Test signal OFF
       * Test frequency DC
       */
      afe_hal_write_register(afe, AFE_REG_CONFIG2, 0xA0);

      /*
       * Lead-off comparator threshold 5%-95%
       * Lead-off current magnitude 6 nA
       * Lead-off frequency DC
       */
      afe_hal_write_register(afe, AFE_REG_LOFF, 0x10);
      /*
       * CH1 enabled
       * Gain = 6
       * CH1 input: negative input connected to RLDIN
       */
      afe_hal_write_register(afe, AFE_REG_CH1SET, 0x00);
      /*
       * CH2 enabled
       * Gain = 6
       * CH2 input: negative input connected to RLDIN
       */
      afe_hal_write_register(afe, AFE_REG_CH2SET, 0x00);

      /*
       * PGA chop frequency = fMOD/4
       * RLD buffer enabled
       * RLD lead-off sense disabled
       *
       * RLD2N not connected
       * RLD2P not connected
       * RLD1N not connected
       * RLD1P not connected
       */
      afe_hal_write_register(afe, AFE_REG_RLD_SENS, 0xE0);
      /*
       * Lead-off CH2 current flip disabled
       * Lead-off CH1 current flip disabled
       *
       * LOFF2N disabled
       * LOFF2P disabled
       * LOFF1N disabled
       * LOFF1P disabled
       */
      afe_hal_write_register(afe, AFE_REG_LOFF_SENS, 0x00);
      /*
       * fMOD = fCLK / 4
       */
      afe_hal_write_register(afe, AFE_REG_LOFF_STAT, 0x00);

      /*
       * Must write 0x02 for ADS1291 and ADS1292 devices
       */
      afe_hal_write_register(afe, AFE_REG_RESP1, 0x02);
      /*
       * Calibration OFF
       *
       * RLD reference signal = (AVDD - AVSS) / 2
       */
      afe_hal_write_register(afe, AFE_REG_RESP2, 0x07);
      /*
       * GPIO 1 input
       * GPIO 2 input
       */
      afe_hal_write_register(afe, AFE_REG_GPIO, 0x0C);

    /* Start conversions */
    HAL_GPIO_WritePin(GPIOA, AFE_START_Pin, GPIO_PIN_SET);
    HAL_Delay(1);         // t > (4*tCLK = 7.8125 us)

    /* Put the device back in read data continuous mode */
    afe_hal_write_command(afe, AFE_RDATAC);
    HAL_Delay(1);         // t > (4*tCLK = 7.8125 us)

    /* Wait for the device and filter to settle */
    HAL_Delay(5);         // tSETTLE > (516*tMOD = 4.03125 ms @ 1 KSPS)
    HAL_Delay(5);         // tFILTER > (3*tDR = 3 ms @ 1 KSPS);

    /* Set gains */
    afe_set_gain(afe, 1, 6);
    afe_set_gain(afe, 2, 6);

    /* Calibrate input offset */
    afe_calibrate(afe);

    /* Enable DRDY interrupts */
    HAL_NVIC_EnableIRQ(EXTI1_IRQn);
  }
}

void afe_start_read(afe_t *afe)
{
  uint8_t dummy[9] = {0};
  HAL_SPI_TransmitReceive_DMA(afe->hspi, dummy, afe->last_data_buf, 9);
}

void afe_format_data(afe_t *afe)
{
  int32_t adc_raw_data_ch1, adc_raw_data_ch2;
  //Change to actual enum, so the compiler will stfu
  /* 3 status bytes (1100 + LOFF_STAT[4:0] + GPIO[1:0] + 13 '0's) */
  afe->last_data.rld_lead_off  = ((afe->last_data_buf[0] & 0x08)?IS_NOT_CONNECTED:IS_CONNECTED);
  afe->last_data.in2n_lead_off = ((afe->last_data_buf[0] & 0x04)?IS_NOT_CONNECTED:IS_CONNECTED);
  afe->last_data.in2p_lead_off = ((afe->last_data_buf[0] & 0x02)?IS_NOT_CONNECTED:IS_CONNECTED);
  afe->last_data.in1n_lead_off = ((afe->last_data_buf[0] & 0x01)?IS_NOT_CONNECTED:IS_CONNECTED);
  afe->last_data.in1p_lead_off = ((afe->last_data_buf[1] & 0x80)?IS_NOT_CONNECTED:IS_CONNECTED);

  /* 3 data bytes per channel */
  adc_raw_data_ch1 = (((int32_t) ((int8_t) afe->last_data_buf[3])) << 16) |
                     (((int32_t) afe->last_data_buf[4]) << 8)             |
                      ((int32_t) afe->last_data_buf[5]);
  adc_raw_data_ch2 = (((int32_t) ((int8_t) afe->last_data_buf[6])) << 16) |
                     (((int32_t) afe->last_data_buf[7]) << 8)             |
                      ((int32_t) afe->last_data_buf[8]);

  /* Turn into microvolts */
  /* OH MY GOD, DATA IS EVOLVING */
  afe->last_data.ch1_data = (adc_raw_data_ch1  * AFE_V_PER_BIT ) / (10 * 1000 * afe->afe_gain_ch1);
  afe->last_data.ch2_data = (adc_raw_data_ch2  * AFE_V_PER_BIT ) / (10 * 1000 * afe->afe_gain_ch2);

  /* CONGRATS, DATA EVOLVED INTO MICROVOLTS */
}

void afe_calibrate(afe_t *afe)
{
  uint8_t reg_resp2_prev,  reg_ch1set_prev,  reg_ch2set_prev;
  uint8_t reg_resp2_calib, reg_ch1set_calib, reg_ch2set_calib;

  /* Stop read data continuous: Send SDATAC so registers can be written */
  afe_hal_write_command(afe, AFE_SDATAC);
  HAL_Delay(1);         // t > (4*tCLK = 7.8125 us)

  /* Store previous values */
  afe_hal_read_register(afe, AFE_REG_CH1SET, &reg_ch1set_prev);
  afe_hal_read_register(afe, AFE_REG_CH2SET, &reg_ch2set_prev);
  afe_hal_read_register(afe, AFE_REG_RESP2,  &reg_resp2_prev);

  /*
   * Generate calibration commands
   *
   * CH1/CH2 channel power down = Disabled
   * CH1/CH2 input selection = Input shorted (for offset measurements)
   * RESP2 calibration on = Enabled
   */
  reg_ch1set_calib = (reg_ch1set_prev & 0x70) | 0x01;
  reg_ch2set_calib = (reg_ch2set_prev & 0x70) | 0x01;
  reg_resp2_calib  =  reg_resp2_prev | 0x80;

  /* Calibrate */
  afe_hal_write_register(afe, AFE_REG_CH1SET, reg_ch1set_calib);
  afe_hal_write_register(afe, AFE_REG_CH2SET, reg_ch2set_calib);
  afe_hal_write_register(afe, AFE_REG_RESP2,  reg_resp2_calib);

  afe_hal_write_command(afe, AFE_OFFSETCAL);
  HAL_Delay(100);

  /* Restore previous values */
  afe_hal_write_register(afe, AFE_REG_RESP2,  reg_resp2_prev);
  afe_hal_write_register(afe, AFE_REG_CH1SET, reg_ch1set_prev);
  afe_hal_write_register(afe, AFE_REG_CH2SET, reg_ch2set_prev);

  /* Put the device back in read data continuous mode */
  afe_hal_write_command(afe, AFE_RDATAC);
  HAL_Delay(1);         // t > (4*tCLK = 7.8125 us)

  /* Enable DRDY interrupts */
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  /* End of calibration */
}

void afe_test_signal_on(afe_t *afe)
{
  uint8_t reg_config2_prev, reg_ch1set_prev, reg_ch2set_prev;
  uint8_t reg_config2_ts,   reg_ch1set_ts,   reg_ch2set_ts;

  /* Disable DRDY interrupts */
  HAL_NVIC_DisableIRQ(EXTI1_IRQn);

  /* Stop read data continuous: Send SDATAC so registers can be written */
  afe_hal_write_command(afe, AFE_SDATAC);
  HAL_Delay(1);         // t > (4*tCLK = 7.8125 us)

  /* Store previous values */
  afe_hal_read_register(afe, AFE_REG_CH1SET,  &reg_ch1set_prev);
  afe_hal_read_register(afe, AFE_REG_CH2SET,  &reg_ch2set_prev);
  afe_hal_read_register(afe, AFE_REG_CONFIG2, &reg_config2_prev);

  /*
   * Generate test signal commands
   *
   * CH1/CH2 channel power down = Disabled
   * CH1/CH2 input selection = Test signal
   * CONFIG2 test signal = ON
   * CONFIG2 test frequency = 1 Hz
   */
  reg_ch1set_ts  = (reg_ch1set_prev & 0x70) | 0x05;
  reg_ch2set_ts  = (reg_ch2set_prev & 0x70) | 0x05;
  reg_config2_ts =  reg_config2_prev | 0x03;

  /* Activate test signal */
  afe_hal_write_register(afe, AFE_REG_CH1SET,  reg_ch1set_ts);
  afe_hal_write_register(afe, AFE_REG_CH2SET,  reg_ch2set_ts);
  afe_hal_write_register(afe, AFE_REG_CONFIG2, reg_config2_ts);

  /* Put the device back in read data continuous mode */
  afe_hal_write_command(afe, AFE_RDATAC);
  HAL_Delay(1);         // t > (4*tCLK = 7.8125 us)

  /* Enable DRDY interrupts */
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);
}

void afe_test_signal_off(afe_t *afe)
{
  uint8_t reg_config2_prev, reg_ch1set_prev, reg_ch2set_prev;
  uint8_t reg_config2_ts,   reg_ch1set_ts,   reg_ch2set_ts;

  /* Disable DRDY interrupts */
  HAL_NVIC_DisableIRQ(EXTI1_IRQn);

  /* Stop read data continuous: Send SDATAC so registers can be written */
  afe_hal_write_command(afe, AFE_SDATAC);
  HAL_Delay(1);         // t > (4*tCLK = 7.8125 us)

  /* Store previous values */
  afe_hal_read_register(afe, AFE_REG_CH1SET,  &reg_ch1set_prev);
  afe_hal_read_register(afe, AFE_REG_CH2SET,  &reg_ch2set_prev);
  afe_hal_read_register(afe, AFE_REG_CONFIG2, &reg_config2_prev);

  /*
   * Generate test signal commands
   *
   * CH1/CH2 channel power down = Disabled
   * CH1/CH2 input selection = Input shorted (for offset measurements)
   * CONFIG2 test signal = OFF
   * CONFIG2 test frequency = DC
   */
  reg_ch1set_ts  = (reg_ch1set_prev & 0x70) | 0x01;
  reg_ch2set_ts  = (reg_ch2set_prev & 0x70) | 0x01;
  reg_config2_ts =  reg_config2_prev & 0xFC;

  /*
   * Deactivate test signal
   *
   * CH1 and CH2 inputs are shorted after test signal is disabled
   */
  afe_hal_write_register(afe, AFE_REG_CONFIG2, reg_config2_ts);
  afe_hal_write_register(afe, AFE_REG_CH1SET,  reg_ch1set_ts);
  afe_hal_write_register(afe, AFE_REG_CH2SET,  reg_ch2set_ts);

  /* Put the device back in read data continuous mode */
  afe_hal_write_command(afe, AFE_RDATAC);
  HAL_Delay(1);         // t > (4*tCLK = 7.8125 us)

  /* Enable DRDY interrupts */
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);
}

void afe_set_gain(afe_t *afe, uint8_t channel, uint8_t gain)
{
  uint8_t reg_ch1set_prev, reg_ch2set_prev;
  uint8_t reg_ch1set_ts,   reg_ch2set_ts;

  uint8_t translated_gain = 0;

  /* Check some sweet gains */
  switch (gain)
  {
    case 1:
      translated_gain = 0x01;
      break;
    case 2:
      translated_gain = 0x02;
      break;
    case 3:
      translated_gain = 0x03;
      break;
    case 4:
      translated_gain = 0x04;
      break;
    case 6:
      translated_gain = 0x00;
      break;
    case 8:
      translated_gain = 0x05;
      break;
    case 12:
      translated_gain = 0x06;
      break;
    default:
      return;
  }

  /* Disable DRDY interrupts */
  HAL_NVIC_DisableIRQ(EXTI1_IRQn);

  /* Stop read data continuous: Send SDATAC so registers can be written */
  afe_hal_write_command(afe, AFE_SDATAC);
  HAL_Delay(1);         // t > (4*tCLK = 7.8125 us)

  /* Store previous values and write new ones*/
  switch (channel)
  {
    case 1:
      afe_hal_read_register(afe, AFE_REG_CH1SET,  &reg_ch1set_prev);
      reg_ch1set_ts  = (reg_ch1set_prev & 0x8F) | (translated_gain << 4);
      afe_hal_write_register(afe, AFE_REG_CH1SET,  reg_ch1set_ts);

      afe->afe_gain_ch1 = gain;

      break;
    case 2:
      afe_hal_read_register(afe, AFE_REG_CH2SET,  &reg_ch2set_prev);
      reg_ch2set_ts  = (reg_ch2set_prev & 0x8F) | (translated_gain << 4);
      afe_hal_write_register(afe, AFE_REG_CH2SET,  reg_ch2set_ts);

      afe->afe_gain_ch2 = gain;

      break;
    default:
      break;
  }

  /* Put the device back in read data continuous mode */
  afe_hal_write_command(afe, AFE_RDATAC);
  HAL_Delay(1);         // t > (4*tCLK = 7.8125 us)

  /* Enable DRDY interrupts */
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);
}