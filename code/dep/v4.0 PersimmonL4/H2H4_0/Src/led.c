#include "led.h"

void led_toggle(uint16_t led)
{
  HAL_GPIO_TogglePin(GPIOC, led);
}

void led_on(uint16_t led)
{
  HAL_GPIO_WritePin(GPIOC, led, GPIO_PIN_SET);
}

void led_off(uint16_t led)
{
  HAL_GPIO_WritePin(GPIOC, led, GPIO_PIN_RESET);
}