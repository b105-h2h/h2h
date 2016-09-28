/*
 * ADS1291_constants.h
 *
 * ADS1291 Medical Analog Front End constant definitions
 *
 *  Created on: 28/7/2015
 *      Author: slopez
 */

#ifndef AFE_AFE_SRC_ADS1291_CONSTANTS_H_
#define AFE_AFE_SRC_ADS1291_CONSTANTS_H_

///// SPI commands
/*
 * System commands
 */
#define WAKEUP			0x02
#define STANDBY			0x04
#define RESET			0x06
#define START			0x08
#define STOP			0x0A
#define OFFSETCAL		0x1A

/*
 * Data read commands
 */
#define RDATAC			0x10
#define SDATAC			0x11
#define RDATA			0x12

/*
 * Register read commands
 */
#define RREG			0x20
#define WREG			0x40


///// Register address definition
/*
 * Device settings
 */
#define REG_ID			0x00
/*
 * Global settings across channels
 */
#define REG_CONFIG1		0x01
#define REG_CONFIG2		0x02
#define REG_LOFF		0x03
/*
 * Channel-Specific Settings
 */
#define REG_CH1SET		0x04
#define REG_CH2SET		0x05
#define REG_RLD_SENS	0x06
#define REG_LOFF_SENS	0x07
#define REG_LOFF_STAT	0x08
/*
 * GPIO and Other Registers
 */
#define REG_RESP1		0x09
#define REG_RESP2		0x0A
#define REG_GPIO		0x0B


#endif /* AFE_AFE_SRC_ADS1291_CONSTANTS_H_ */
