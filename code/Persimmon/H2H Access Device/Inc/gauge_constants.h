/*
 * gauge_constants.h
 *
 * BQ27441-G1 fuel gauge definitions
 *
 *  Created on: 13/05/2016
 *      Author: slopez
 */
 
#ifndef GAUGE_GAUGE_CONSTANTS_H_
#define GAUGE_GAUGE_CONSTANTS_H_

#define FG_I2C_ADDRESS          (0x0055<<1)
#define FG_UNSEAL	        0x8000

/* Standard commands */

#define FG_CNTL                 0x00
#define FG_TEMP                 0x02
#define FG_VOLT                 0x04
#define FG_FLAGS                0x06

#define FG_NOM_AVAIL_CAP        0x08
#define FG_FULL_AVAIL_CAP       0x0A
#define FG_REMAINING_CAP        0x0C
#define FG_FULL_CHG_CAP         0x0E

#define FG_AVG_CURRENT          0x10
#define FG_STANDBY_CURRENT      0x12
#define FG_MAX_LOAD_CURRENT     0x14
#define FG_AVG_POWER            0x18

#define FG_SOC                  0x1C
#define FG_INTERNAL_TEMP        0x1E
#define FG_SOH                  0x20

#define FG_REMAINING_CAP_UNFIL  0x28
#define FG_REMAINING_CAP_FIL    0x2A
#define FG_FULL_CHG_CAP_UNFIL   0x2C
#define FG_FULL_CHG_CAP_FIL     0x2E
#define FG_SOC_UNFIL            0x30

/* Control subcommands */

#define FG_CNTL_STATUS          0x0000
#define FG_CNTL_DEVICE_TYPE     0x0001
#define FG_CNTL_FW_VERSION      0x0002
#define FG_CNTL_DM_CODE         0x0004
#define FG_CNTL_PREV_MACWRITE   0x0007
#define FG_CNTL_CHEM_ID         0x0008
#define FG_CNTL_BAT_INSERT      0x000C
#define FG_CNTL_BAT_REMOVE      0x000D
#define FG_CNTL_SET_HIBERNATE   0x0011
#define FG_CNTL_CLEAR_HIBERNATE 0x0012
#define FG_CNTL_SET_CFGUPDATE   0x0013
#define FG_CNTL_SHUTDOWN_EN     0x001B
#define FG_CNTL_SHUTDOWN        0x001C
#define FG_CNTL_SEALED          0x0020
#define FG_CNTL_TOGGLE_GPOUT    0x0023
#define FG_CNTL_RESET           0x0041
#define FG_CNTL_SOFT_RESET      0x0042
#define FG_CNTL_EXIT_CFGUPDATE  0x0043
#define FG_CNTL_EXIT_RESIM      0x0044

/* Extended commands */

#define FG_OP_CONFIG            0x3A
#define FG_DESIGN_CAP           0x3C
#define FG_DATA_CLASS           0x3E
#define FG_DATA_BLOCK           0x3F
#define FG_BLOCK_DATA           0x40
#define FG_BLOCK_DATA_CHECK_SUM 0x60
#define FG_BLOCK_DATA_CNTL      0x61

#endif /* GAUGE_GAUGE_CONSTANTS_H_ */