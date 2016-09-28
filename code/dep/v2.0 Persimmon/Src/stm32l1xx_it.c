/**
  ******************************************************************************
  * @file    stm32l1xx_it.c
  * @brief   Interrupt Service Routines.
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
/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx_hal.h"
#include "stm32l1xx.h"
#include "stm32l1xx_it.h"

/* USER CODE BEGIN 0 */
#include "spi.h"
#include "afe.h"
#include "gauge.h"
#include "lcd.h"
#include "touch.h"

#include <stdio.h>

/* Touch variables */
extern touch_pos_t last_pos;

/* FG variables */
extern uint16_t fg_soc;
extern char bat_soc[4];
color_t bat_color;

/* RTC variables */
extern RTC_TimeTypeDef time_now;
extern RTC_DateTypeDef date_now;

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern RTC_HandleTypeDef hrtc;
extern DMA_HandleTypeDef hdma_spi1_rx;
extern TIM_HandleTypeDef htim6;

/******************************************************************************/
/*            Cortex-M3 Processor Interruption and Exception Handlers         */ 
/******************************************************************************/

/**
* @brief This function handles Non maskable interrupt.
*/
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  HAL_RCC_NMI_IRQHandler();
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32L1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32l1xx.s).                    */
/******************************************************************************/

/**
* @brief This function handles EXTI line0 interrupt.
*/
void EXTI0_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI0_IRQn 0 */
  // Checks if the interrupt has been triggered by a rising edge( first 
  // pressing of button), starts a 2secs timer and make the line falling
  // edge sensitive.
  if ((EXTI-> RTSR) & (GPIO_PIN_0))
  {
    HAL_TIM_Base_Start_IT(&htim6);
    EXTI-> RTSR &= (~GPIO_PIN_0);
    EXTI-> FTSR |= GPIO_PIN_0;
  }
  else
  {
    // If a falling edge is detected( button is released) before the countdown
    // ends, we stop the countdown, reset it and make the line rising
    // edge sensitive again.
    HAL_TIM_Base_Stop_IT(&htim6);
    __HAL_TIM_SET_COUNTER(&htim6,0);
    EXTI-> FTSR &= (~GPIO_PIN_0);
    EXTI-> RTSR |= GPIO_PIN_0;
  }
  /* USER CODE END EXTI0_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
  /* USER CODE BEGIN EXTI0_IRQn 1 */

  /* USER CODE END EXTI0_IRQn 1 */
}

/**
* @brief This function handles EXTI line1 interrupt.
*/
void EXTI1_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI1_IRQn 0 */
  
//  HAL_SPI_Receive_DMA(&hspi1, uint8_t * pData, AFE_BYTES_PER_CONV);
  
  uint8_t dummy[3] = {0,0,0}, data_ch1[3] = {0,0,0}, data_ch2[3] = {0,0,0};
  int32_t data_tmp_ch1, data_tmp_ch2;
  static int32_t data_log_ch1[1000], data_log_ch2[1000];
  static int32_t *ptr_1 = data_log_ch1; 
  static int32_t *ptr_2 = data_log_ch2;
  
  //Read 3 ADS1291 status bytes
  HAL_GPIO_WritePin(GPIOA,AFE_CS_Pin,GPIO_PIN_RESET); //Enable CS
  
  HAL_SPI_Receive(&hspi1, dummy, 3, 100);

  HAL_SPI_Receive(&hspi1, data_ch1, 3, 100);
  
  HAL_SPI_Receive(&hspi1, data_ch2, 3, 100);
  
  HAL_GPIO_WritePin(GPIOA,AFE_CS_Pin,GPIO_PIN_SET); //Disable CS   

  //Store signal data into ecg signal buffer
  data_tmp_ch1 = (((int32_t) ((int8_t) data_ch1[0])) << 16) |
                 (((int32_t) data_ch1[1]) << 8)             |
                  ((int32_t) data_ch1[2]);
  data_tmp_ch2 = (((int32_t) ((int8_t) data_ch2[0])) << 16) |
                 (((int32_t) data_ch2[1]) << 8)             |
                  ((int32_t) data_ch2[2]);
  
  *(ptr_1++) = data_tmp_ch1;
  *(ptr_2++) = data_tmp_ch2;

  if (ptr_1 == &(data_log_ch1[999])) { 
    ptr_1 = data_log_ch1;
  }
  if (ptr_2 == &(data_log_ch2[999])) { 
    ptr_2 = data_log_ch2;
  }
  /* USER CODE END EXTI1_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
  /* USER CODE BEGIN EXTI1_IRQn 1 */

  /* USER CODE END EXTI1_IRQn 1 */
}

/**
* @brief This function handles EXTI line3 interrupt.
*/
void EXTI3_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI3_IRQn 0 */
  bat_color = (color_t) COLOR_WHITE;
  
  /* Read new battery soc value */
  fg_soc = fg_read_reg16(FG_SOC);

  /* Convert to string
   * http://www.keil.com/support/man/docs/c51/c51_sprintf.htm
   */
  sprintf(bat_soc, "%d", fg_soc);

  /* Draw it */
  
  /* USER CODE END EXTI3_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
  /* USER CODE BEGIN EXTI3_IRQn 1 */

  /* USER CODE END EXTI3_IRQn 1 */
}

/**
* @brief This function handles DMA1 channel2 global interrupt.
*/
void DMA1_Channel2_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel2_IRQn 0 */

  /* USER CODE END DMA1_Channel2_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_spi1_rx);
  /* USER CODE BEGIN DMA1_Channel2_IRQn 1 */

  /* USER CODE END DMA1_Channel2_IRQn 1 */
}

/**
* @brief This function handles EXTI line[15:10] interrupts.
*/
void EXTI15_10_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI15_10_IRQn 0 */
  
  /* Disable touchpad interrupts */
  HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
  
  /* Get position */
//  touch_get_position(&last_pos);
  
  /* USER CODE END EXTI15_10_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_10);
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_11);
  /* USER CODE BEGIN EXTI15_10_IRQn 1 */

  /* Enable touchpad interrupts */
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
  
  /* USER CODE END EXTI15_10_IRQn 1 */
}

/**
* @brief This function handles RTC alarms A and B interrupt through EXTI line 17.
*/
void RTC_Alarm_IRQHandler(void)
{
  /* USER CODE BEGIN RTC_Alarm_IRQn 0 */

  /* Get time and date from RTC registers */
  HAL_RTC_GetTime(&hrtc, &time_now, RTC_FORMAT_BIN);
  HAL_RTC_GetDate(&hrtc, &date_now, RTC_FORMAT_BIN);
  
  /* USER CODE END RTC_Alarm_IRQn 0 */
  HAL_RTC_AlarmIRQHandler(&hrtc);
  /* USER CODE BEGIN RTC_Alarm_IRQn 1 */

  /* USER CODE END RTC_Alarm_IRQn 1 */
}

/**
* @brief This function handles TIM6 global interrupt.
*/
void TIM6_IRQHandler(void)
{
  /* USER CODE BEGIN TIM6_IRQn 0 */
  
  /* Turn off LED */
  HAL_GPIO_WritePin(GPIOC,UI_LED_R_Pin|UI_LED_B_Pin|UI_LED_G_Pin,GPIO_PIN_RESET);

  /* Disable RTC alarms because they wake the system up */
  HAL_NVIC_DisableIRQ(RTC_Alarm_IRQn);
  
  /* USER CODE END TIM6_IRQn 0 */
  HAL_TIM_IRQHandler(&htim6);
  /* USER CODE BEGIN TIM6_IRQn 1 */
  
  /* Set wake up pin and go to sleep (little prince) */
  HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);
  __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
  HAL_PWR_EnterSTANDBYMode();
  
  /* USER CODE END TIM6_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
