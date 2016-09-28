#include "item_lcd_config.h"

void item_lcd_config_init(item_lcd_config_t *config, uint8_t backlight_level)
{
  /* Set backlight PWM level */
  config->backlight_level = backlight_level;
}
