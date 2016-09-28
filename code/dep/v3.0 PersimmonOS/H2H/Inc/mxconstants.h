/**
  ******************************************************************************
  * File Name          : mxconstants.h
  * Description        : This file contains the common defines of the application
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MXCONSTANT_H
#define __MXCONSTANT_H
  /* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define NC_Pin GPIO_PIN_2
#define NC_GPIO_Port GPIOE
#define FG_GPOUT_Pin GPIO_PIN_3
#define FG_GPOUT_GPIO_Port GPIOE
#define CHRG_PG_Pin GPIO_PIN_4
#define CHRG_PG_GPIO_Port GPIOE
#define CHRG_CHG_Pin GPIO_PIN_5
#define CHRG_CHG_GPIO_Port GPIOE
#define CHRG_JACK_DET_Pin GPIO_PIN_6
#define CHRG_JACK_DET_GPIO_Port GPIOE
#define NCC13_Pin GPIO_PIN_13
#define NCC13_GPIO_Port GPIOC
#define X2_P_Pin GPIO_PIN_14
#define X2_P_GPIO_Port GPIOC
#define X2_N_Pin GPIO_PIN_15
#define X2_N_GPIO_Port GPIOC
#define X1_P_Pin GPIO_PIN_0
#define X1_P_GPIO_Port GPIOH
#define X1_N_Pin GPIO_PIN_1
#define X1_N_GPIO_Port GPIOH
#define NCC0_Pin GPIO_PIN_0
#define NCC0_GPIO_Port GPIOC
#define NCC1_Pin GPIO_PIN_1
#define NCC1_GPIO_Port GPIOC
#define VDD_AFE_D_EN_Pin GPIO_PIN_2
#define VDD_AFE_D_EN_GPIO_Port GPIOC
#define VDD_AFE_A_EN_Pin GPIO_PIN_3
#define VDD_AFE_A_EN_GPIO_Port GPIOC
#define SYS_WKUP_Pin GPIO_PIN_0
#define SYS_WKUP_GPIO_Port GPIOA
#define AFE_DRDY_Pin GPIO_PIN_1
#define AFE_DRDY_GPIO_Port GPIOA
#define AFE_RESET_Pin GPIO_PIN_2
#define AFE_RESET_GPIO_Port GPIOA
#define AFE_START_Pin GPIO_PIN_3
#define AFE_START_GPIO_Port GPIOA
#define AFE_CS_Pin GPIO_PIN_4
#define AFE_CS_GPIO_Port GPIOA
#define AFE_SCLK_Pin GPIO_PIN_5
#define AFE_SCLK_GPIO_Port GPIOA
#define AFE_DOUT_Pin GPIO_PIN_6
#define AFE_DOUT_GPIO_Port GPIOA
#define AFE_DIN_Pin GPIO_PIN_7
#define AFE_DIN_GPIO_Port GPIOA
#define NCC4_Pin GPIO_PIN_4
#define NCC4_GPIO_Port GPIOC
#define VDD_SCREEN_EN_Pin GPIO_PIN_5
#define VDD_SCREEN_EN_GPIO_Port GPIOC
#define UI_BUZZER_Pin GPIO_PIN_0
#define UI_BUZZER_GPIO_Port GPIOB
#define NCB1_Pin GPIO_PIN_1
#define NCB1_GPIO_Port GPIOB
#define BOOT1_Pin GPIO_PIN_2
#define BOOT1_GPIO_Port GPIOB
#define TP_BUSY_Pin GPIO_PIN_10
#define TP_BUSY_GPIO_Port GPIOB
#define TP_PEN_Pin GPIO_PIN_11
#define TP_PEN_GPIO_Port GPIOB
#define TP_CS_Pin GPIO_PIN_12
#define TP_CS_GPIO_Port GPIOB
#define TP_SCLK_Pin GPIO_PIN_13
#define TP_SCLK_GPIO_Port GPIOB
#define TP_MISO_Pin GPIO_PIN_14
#define TP_MISO_GPIO_Port GPIOB
#define TP_MOSI_Pin GPIO_PIN_15
#define TP_MOSI_GPIO_Port GPIOB
#define UI_BACKLIGHT_Pin GPIO_PIN_12
#define UI_BACKLIGHT_GPIO_Port GPIOD
#define NCD13_Pin GPIO_PIN_13
#define NCD13_GPIO_Port GPIOD
#define VDD_RF_PW_EN_Pin GPIO_PIN_6
#define VDD_RF_PW_EN_GPIO_Port GPIOC
#define VDD_RF_IO_EN_Pin GPIO_PIN_7
#define VDD_RF_IO_EN_GPIO_Port GPIOC
#define NCC8_Pin GPIO_PIN_8
#define NCC8_GPIO_Port GPIOC
#define nSHUTD_Pin GPIO_PIN_9
#define nSHUTD_GPIO_Port GPIOC
#define BT_SLOW_CLK_Pin GPIO_PIN_8
#define BT_SLOW_CLK_GPIO_Port GPIOA
#define BT_TX_Pin GPIO_PIN_9
#define BT_TX_GPIO_Port GPIOA
#define BT_RX_Pin GPIO_PIN_10
#define BT_RX_GPIO_Port GPIOA
#define BT_CTS_Pin GPIO_PIN_11
#define BT_CTS_GPIO_Port GPIOA
#define BT_RTS_Pin GPIO_PIN_12
#define BT_RTS_GPIO_Port GPIOA
#define NCH2_Pin GPIO_PIN_2
#define NCH2_GPIO_Port GPIOH
#define UI_LED_R_Pin GPIO_PIN_10
#define UI_LED_R_GPIO_Port GPIOC
#define UI_LED_G_Pin GPIO_PIN_11
#define UI_LED_G_GPIO_Port GPIOC
#define UI_LED_B_Pin GPIO_PIN_12
#define UI_LED_B_GPIO_Port GPIOC
#define NCD2_Pin GPIO_PIN_2
#define NCD2_GPIO_Port GPIOD
#define NCD3_Pin GPIO_PIN_3
#define NCD3_GPIO_Port GPIOD
#define NCD6_Pin GPIO_PIN_6
#define NCD6_GPIO_Port GPIOD
#define NCB5_Pin GPIO_PIN_5
#define NCB5_GPIO_Port GPIOB
#define FG_SCL_Pin GPIO_PIN_6
#define FG_SCL_GPIO_Port GPIOB
#define FG_SDA_Pin GPIO_PIN_7
#define FG_SDA_GPIO_Port GPIOB
#define NCB8_Pin GPIO_PIN_8
#define NCB8_GPIO_Port GPIOB
#define NCB9_Pin GPIO_PIN_9
#define NCB9_GPIO_Port GPIOB
#define NCE0_Pin GPIO_PIN_0
#define NCE0_GPIO_Port GPIOE
#define NCE1_Pin GPIO_PIN_1
#define NCE1_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MXCONSTANT_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
