/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
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
#include "dma.h"
#include "i2c.h"
#include "rtc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"

/* USER CODE BEGIN Includes */
#include "afe.h"
#include "gauge.h"
#include "lcd.h"
#include "touch.h"

#include "myriad_pro_semibold.h"

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
#define QUARTER 350
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
void two_secs_wakeup(void);
void note_flash(uint16_t dur , uint16_t frec);
void epic_sax_guy(void);
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
lcd_t lcd;
touch_t touch;

uint16_t fg_soc;
char bat_soc[4];

RTC_TimeTypeDef time_now;
RTC_DateTypeDef date_now;
color_t rtc_color;

char hours[5];
char minutes[5];
char seconds[5];

char day[5];
char month[5];
char year[5];

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Wake up check */
  HAL_DBGMCU_EnableDBGStandbyMode();
  two_secs_wakeup();
  
  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_FSMC_Init();
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_SPI2_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  MX_TIM6_Init();
  MX_RTC_Init();

  /* USER CODE BEGIN 2 */
    uint16_t voltage, temperature, int_temp;
    int16_t avg_current, max_current;
    
    /* Starting blink */
    HAL_GPIO_TogglePin(GPIOC, UI_LED_R_Pin|UI_LED_G_Pin|UI_LED_B_Pin);
    HAL_Delay(200); 
    HAL_GPIO_TogglePin(GPIOC, UI_LED_R_Pin|UI_LED_G_Pin|UI_LED_B_Pin); 
    
    /* Initializations */
    //afe_init();
    lcd_init(&lcd, &hsram1, LCD_REG, LCD_DATA, LCD_X_SIZE, LCD_Y_SIZE);
    fg_init();
    touch_init(&touch, &hspi2, &lcd, TOUCH_X_SIZE, TOUCH_Y_SIZE);
    
    /* First fg reading */
    fg_soc = fg_read_reg16(FG_SOC);
    sprintf(bat_soc, "%d", fg_soc);
    
    lcd_set_brightness(&lcd, 255);
    
//TEST
    color_t text_color = (color_t) COLOR_WHITE;
    color_t bg_color = (color_t) COLOR_RED;
    color_t border_color = (color_t) COLOR_BLUE;
    gui_status_t gui_status;
    
    area_t test_area;
    gui_status = area_init(&test_area, 
              150, 100,
              250, 150,
              LEFT, TOP,
              RECTANGLE,
              //Text area
              50, 100,
              myriad_pro_semibold17x23, "Testerino",
              LEFT, TOP,
              LEFT, TOP,
              //---------
              ACTIVE,
              &text_color, &bg_color, &border_color);
    
    lcd_print_area(&lcd, &test_area);

    
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while(1)
    {
      sprintf(hours, "%d", time_now.Hours);
      sprintf(minutes, "%d", time_now.Minutes);
      sprintf(seconds, "%d", time_now.Seconds);
      
      sprintf(day, "%d", date_now.Date);
      sprintf(month, "%d", date_now.Month);
      sprintf(year, "%d", date_now.Year);
          
      HAL_Delay(200);
      
      /* Fuel gauge data */
//      temperature = fg_read_reg16(FG_TEMP);
//      voltage = fg_read_reg16(FG_VOLT);
//      avg_current = fg_read_reg16(FG_AVG_CURRENT);
//      int_temp = fg_read_reg16(FG_INTERNAL_TEMP);
//      max_current = fg_read_reg16(FG_MAX_LOAD_CURRENT);
      
      /* Touch data */
//      touch_get_position(&last_pos);
//      
//      sprintf(x_pos, "%d", last_pos.x_pos);
//      sprintf(y_pos, "%d", last_pos.y_pos);
//      sprintf(pressure, "%d", last_pos.pressure);
    }
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_LSE, RCC_MCODIV_1);

  HAL_RCC_EnableCSS();

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */
/**
  * @brief  Implements the initial delay of 2secs for wake up .
  * @param  None
  * @retval None
*/
void two_secs_wakeup(void){
  // Checks if the system is resumed from standby
  if (__HAL_PWR_GET_FLAG(PWR_FLAG_WU) == SET)
  {
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
    wk_falling_edge_detection();
    HAL_Delay(2000);
    
//    if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_0) == SET)
//    {
      __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
      HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);
      HAL_PWR_EnterSTANDBYMode();
//    }
  }
  
}
/**
  * @brief  Activate PWM for note and 3 leds for white flashing.
  * @param  dur:duration of the note in ms.
  * @param  frec:frequency of the note in Hz.
  * @retval None
*/

void note_flash(uint16_t dur , uint16_t frec){   
  uint16_t period = 16000000/((htim3.Init.Prescaler + 1)*frec);
  htim3.Init.Period = period;
  HAL_TIM_PWM_Init(&htim3);
  HAL_GPIO_WritePin(GPIOC, nSHUTD_Pin|UI_LED_R_Pin|UI_LED_G_Pin|UI_LED_B_Pin, GPIO_PIN_SET);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
  HAL_Delay(dur);
  HAL_GPIO_WritePin(GPIOC, nSHUTD_Pin|UI_LED_R_Pin|UI_LED_G_Pin|UI_LED_B_Pin, GPIO_PIN_RESET);
  HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_3);
}

/**
  * @brief  Epic sax guy song.
  * @param  None
  * @retval None
*/    
void epic_sax_guy(void){
  
  int i = 0;
  for( i = 0; i < 2; i++){
    note_flash(QUARTER>>1, 784);
    HAL_Delay((QUARTER*3)>>1);
    note_flash(QUARTER>>1, 784);
    HAL_Delay(50);
    note_flash(QUARTER>>2, 784);
    HAL_Delay(50);
    note_flash(QUARTER>>2, 784);
    HAL_Delay(50);
    note_flash(QUARTER>>2, 698);
    HAL_Delay(50);
    note_flash((QUARTER*3)>>2, 784);
    HAL_Delay(50);
  }
  note_flash(QUARTER>>1, 784);
  HAL_Delay(QUARTER);
  note_flash(QUARTER, 932);
  HAL_Delay(50);
  note_flash(QUARTER, 784);
  HAL_Delay(50);
  note_flash(QUARTER, 698);
  HAL_Delay(50);
  note_flash(QUARTER, 622);
  HAL_Delay(50);
  note_flash(QUARTER>>1, 523);
  HAL_Delay(50);
  note_flash(QUARTER>>1, 523);
  HAL_Delay(50);
  note_flash(QUARTER>>1, 587);
  HAL_Delay(50);
  note_flash(QUARTER>>1, 622);
  HAL_Delay(50);    
  note_flash(QUARTER>>1, 523);
  HAL_Delay(50); 
  note_flash(QUARTER, 784);
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
