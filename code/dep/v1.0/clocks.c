/*
 * clocks.c
 *
 *  Created on: 2/9/2015
 *      Author: slopez
 */

#include "clocks.h"

void clocks_setup()
{
	//Activate LFXT pins
		PJSEL0 |= BIT4;								//Set PJ.4 and PJ.5 as LFXIN and LFXOUT
		PJSEL1 &= ~BIT4;							// |

	//Activate HFXT pins
		PJSEL0 |= BIT6;								//Set PJ.6 and PJ.7 as HFXIN and HFXOUT
		PJSEL1 &= ~BIT6;							// |


	//Generate SMCLK
		PJDIR |= BIT0;								//Set PJ.0 to output
		PJSEL0 |= BIT0;								//Set PJ.0 as SMCLK
		PJSEL1 &= ~BIT0;							// |

	//Generate ACLK
		PJDIR |= BIT2;								//Set PJ.2 to output
		PJSEL0 |= BIT2;								//Set PJ.2 as ACLK
		PJSEL1 &= ~BIT2;							// |


	//FRAM wait states
		FRCTL0 = FRCTLPW | NWAITS_1;				//Unlock FRAM registers
													//Add 1 waitstate

	//Clock control registers
		CSCTL0 = CSKEY;								//Unlock clock registers
		CSCTL1 = DCORSEL | DCOFSEL_4;				//DCO frecuency set to 16 MHz
		CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK;
													//ACLK source = LFXTCLK
													//SMCLK source = DCOCLK
													//MCLK source = DCOCLK
		CSCTL3 = DIVA_0 | DIVS_3 | DIVM_0;
													//ACLK divider = /1 => ACLK @ 32.768 KHz
													//SMCLK divider = /8 => SMCLK @ 2 MHz
													//MCLK divider = /1 => MCLK @ 16 MHz
}
