/*
 * lcd_constants.h
 *
 * SSD1963 LCD controller definitions
 *
 *  Created on: 11/05/2016
 *      Author: slopez
 */

#ifndef LCD_CONSTANTS_H_
#define LCD_CONSTANTS_H_

/* Lcd struct defines */
#define LCD_REG        ((uint32_t *)(FMC_BASE))
#define LCD_DATA       ((uint32_t *)(FMC_BASE + 0x00020000U))  //See p.620 of STM32L162VD ref. manual

#define LCD_X_SIZE     (uint16_t) 800
#define LCD_Y_SIZE     (uint16_t) 480

/* Lcd commands */
#define LCD_NOP                         0x0000
#define LCD_SOFT_RESET                  0x0001
#define LCD_GET_POWER_MODE              0x000A
#define LCD_GET_ADDRESS_MODE            0x000B
#define LCD_GET_DISPLAY_MODE            0x000D
#define LCD_GET_TEAR_EFFECT_STATUS      0x000E
#define LCD_ENTER_SLEEP_MODE            0x0010
#define LCD_EXIT_SLEEP_MODE             0x0011

#define LCD_ENTER_PARTIAL_MODE          0x0012
#define LCD_ENTER_NORMAL_MODE           0x0013
#define LCD_EXIT_INVERT_MODE            0x0020
#define LCD_ENTER_INVERT_MODE           0x0021

#define LCD_SET_GAMMA_CURVE             0x0026

#define LCD_SET_DISPLAY_OFF             0x0028
#define LCD_SET_DISPLAY_ON              0x0029

#define LCD_SET_COLUMN_ADDRESS          0x002A
#define LCD_SET_PAGE_ADDRESS            0x002B

#define LCD_WRITE_MEMORY_START          0x002C
#define LCD_READ_MEMORY_START           0x002E

#define LCD_SET_PARTIAL_AREA            0x0030
#define LCD_SET_SCROLL_AREA             0x0033

#define LCD_SET_TEAR_OFF                0x0034
#define LCD_SET_TEAR_ON                 0x0035

#define LCD_SET_ADDRESS_MODE            0x0036
#define LCD_SET_SCROLL_START            0x0037

#define LCD_EXIT_IDLE_MODE              0x0038
#define LCD_ENTER_IDLE_MODE             0x0039

#define LCD_WRITE_MEMORY_CONTINUE       0x003C
#define LCD_READ_MEMORY_CONTINUE        0x003E

#define LCD_SET_TEAR_SCANLINE           0x0044
#define LCD_GET_SCANLINE                0x0045

#define LCD_READ_DDB                    0x00A1

#define LCD_SET_LCD_MODE                0x00B0
#define LCD_GET_LCD_MODE                0x00B1
#define LCD_SET_HORI_PERIOD             0x00B4
#define LCD_GET_HORI_PERIOD             0x00B5
#define LCD_SET_VERT_PERIOD             0x00B6
#define LCD_GET_VERT_PERIOD             0x00B7

#define LCD_SET_GPIO_CONF               0x00B8
#define LCD_GET_GPIO_CONF               0x00B9
#define LCD_SET_GPIO_VALUE              0x00BA
#define LCD_GET_GPIO_STATUS             0x00BB

#define LCD_SET_POST_PROC               0x00BC
#define LCD_GET_POST_PROC               0x00BD

#define LCD_SET_PWM_CONF                0x00BE
#define LCD_GET_PWM_CONF                0x00BF

#define LCD_SET_LCD_GEN0                0x00C0
#define LCD_GET_LCD_GEN0                0x00C1
#define LCD_SET_LCD_GEN1                0x00C2
#define LCD_GET_LCD_GEN1                0x00C3
#define LCD_SET_LCD_GEN2                0x00C4
#define LCD_GET_LCD_GEN2                0x00C5
#define LCD_SET_LCD_GEN3                0x00C6
#define LCD_GET_LCD_GEN3                0x00C7

#define LCD_SET_GPIO0_ROP               0x00C8
#define LCD_GET_GPIO0_ROP               0x00C9
#define LCD_SET_GPIO1_ROP               0x00CA
#define LCD_GET_GPIO1_ROP               0x00CB
#define LCD_SET_GPIO2_ROP               0x00CC
#define LCD_GET_GPIO2_ROP               0x00CD
#define LCD_SET_GPIO3_ROP               0x00CE
#define LCD_GET_GPIO3_ROP               0x00CF

#define LCD_SET_DBC_CONF                0x00D0
#define LCD_GET_DBC_CONF                0x00D1
#define LCD_SET_DBC_TH                  0x00D4
#define LCD_GET_DBC_TH                  0x00D5

#define LCD_SET_PLL                     0x00E0
#define LCD_SET_PLL_MN                  0x00E2
#define LCD_GET_PLL_MN                  0x00E3
#define LCD_GET_PLL_STATUS              0x00E4

#define LCD_SET_DEEP_SLEEP              0x00E5
#define LCD_SET_LSHIFT_FREQ             0x00E6
#define LCD_GET_LSHIFT_FREQ             0x00E7

#define LCD_SET_PIXEL_DATA_INTERFACE    0x00F0
#define LCD_GET_PIXEL_DATA_INTERFACE    0x00F1

#endif /* LCD_CONSTANTS_H_ */