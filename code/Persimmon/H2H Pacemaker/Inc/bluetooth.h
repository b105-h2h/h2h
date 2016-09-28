/*
 * bluetooth.h
 *
 *  Created on: 22/6/2015
 *      Author: Smau
 */
 
#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

#include "HCITRANS.h"
#include "BTVSAPI.h"             /* Vendror Specific header.                  */
#include "bluetooth_internal.h"

#define VENDOR_BAUD_RATE                                    2000000L

void bluetooth_init(void);

#endif /* BLUETOOTH_H_ */