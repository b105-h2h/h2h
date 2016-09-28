#include "lcd_config.h"

void lcd_config_init(lcd_config_t *config, uint8_t backlight_level)
{
  /* Set backlight PWM level */
  config->backlight_level = backlight_level;
}
