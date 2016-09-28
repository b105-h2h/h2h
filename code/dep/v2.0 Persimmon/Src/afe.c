/*
 * afe.c
 *
 *  Created on: 29/9/2015
 *      Author: Smau
 */
#include "afe.h"

/**
  * @brief  Writes a single command to one register.
  * @param  reg: Address of the afe register to write.
  * @param  command: command to be send.
  * @retval None
*/
void afe_serial_write_register(uint8_t reg, uint8_t command){
  // This is specific for one command in one register since the AFE,
  // allows to write multiple commands to consecutive registers in 
  // one spi transmision.
  uint8_t data[3] = {reg | AFE_WREG, 0x00, command};
  
  HAL_SPI_Transmit(&hspi1, data, 3, 100);
}

void afe_init()
{
  uint8_t data[1];
  //AFE reset and stop continuous data conversion mode	
  //Power-On-Reset: hold reset line high for 1 second
  HAL_GPIO_WritePin(GPIOA, AFE_RESET_Pin, GPIO_PIN_SET);
  HAL_Delay(1000);	                              
  //Reset pulse: (>= 18 t_clk) => (>= 10 us)
  HAL_GPIO_WritePin(GPIOA, AFE_RESET_Pin, GPIO_PIN_RESET);
  HAL_Delay(1);		//At least 10 useconds ( need to implement us_delay() )
  HAL_GPIO_WritePin(GPIOA, AFE_RESET_Pin, GPIO_PIN_SET);

  HAL_GPIO_WritePin(GPIOA, AFE_CS_Pin, GPIO_PIN_RESET); //Enable CS    
  
  data[0] = AFE_SDATAC;
  HAL_SPI_Transmit(&hspi1, data, 1, 100);
  //Write config commands to AFE
  
  afe_serial_write_register(AFE_REG_CONFIG1, 0x03);
  afe_serial_write_register(AFE_REG_CONFIG2, 0xA3);     // 0xA0 : normal input | 0xA3 : test signal
  afe_serial_write_register(AFE_REG_LOFF, 0x10);
  
  afe_serial_write_register(AFE_REG_CH1SET, 0x07);
  afe_serial_write_register(AFE_REG_CH2SET, 0x07);
  
  afe_serial_write_register(AFE_REG_RLD_SENS, 0xE0);
  afe_serial_write_register(AFE_REG_LOFF_SENS, 0x00);
  afe_serial_write_register(AFE_REG_LOFF_STAT, 0x00);
  
  afe_serial_write_register(AFE_REG_RESP1, 0x02);
  afe_serial_write_register(AFE_REG_RESP2, 0x07);
    
  afe_serial_write_register(AFE_REG_GPIO, 0x0C);
  
  
  /* Calibrate */
  afe_serial_write_register(AFE_REG_RESP2, 0x87);
  
  afe_serial_write_register(AFE_REG_CH1SET, 0x01);
  afe_serial_write_register(AFE_REG_CH2SET, 0x01);
  
  data[0] = AFE_OFFSETCAL;
  HAL_SPI_Transmit(&hspi1, data, 1, 100);

  afe_serial_write_register(AFE_REG_CH1SET, 0x05);      // 0x07 : normal input | 0x05 : test signal
  afe_serial_write_register(AFE_REG_CH2SET, 0x05);
  
  afe_serial_write_register(AFE_REG_RESP2, 0x07);
  
  /* End of calibration */


  //Start capturing data		
  HAL_GPIO_WritePin(GPIOA, AFE_START_Pin, GPIO_PIN_SET); //Start conversions
  HAL_Delay(1);	
  data[0] = AFE_RDATAC;                        
  HAL_SPI_Transmit(&hspi1, data, 1, 100);     //Enable continuous output of conversion data
                                              //In this mode, a SDATAC command must be issued
                                              //before other commands can be sent to the device

  HAL_GPIO_WritePin(GPIOA, AFE_CS_Pin, GPIO_PIN_SET); //Disable CS     
}
