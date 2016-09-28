/*
 * touch_module.c
 *
 * Resistive touch surface controller
 *
 *  Created on: 29/7/2015
 *      Author: slopez
 */

#include "touch.h"

void touch_setup(touch_t* touch)
{
	//Configure touchpad control lines
		//IRQ touch event
		P1DIR &= ~BIT3;								//Set P1.3 to input
														//P1.3 = IRQ touch event (interrupt)
		P1IES |= BIT3;								//Interrupt P1.3 triggered with a high-to-low transition
		P1IFG &= ~BIT3;								//Clear P1.3 interrupt flag

		//BUSY signal
		P3DIR &= ~BIT0;								//Set P3.0 to input
														//P3.0 = Touchpad controller busy signal

	//Configure USCI_A1 for SPI operation
		P3SEL0 |= BIT6;								//Set P3.6 as UCA1CLK
		P3SEL1 &= ~BIT6;							// |
		P3SEL0 |= BIT4;								//Set P3.4 as UCA1SIMO
		P3SEL1 &= ~BIT4;							// |
		P3SEL0 |= BIT5;								//Set P3.5 as UCA1SOMI
		P3SEL1 &= ~BIT5;							// |
		P3DIR |= BIT7;								//Set P3.7 as output (CS)
		P3SEL0 &= ~BIT7;							// |
		P3SEL1 &= ~BIT7;							// |
		P3OUT |= BIT7;								//Hold CS high by default

		UCA1CTLW0 = UCSWRST;						//Hold USCI_A1 in reset state - necessary while configuring registers

		UCA1CTLW0 |= UCCKPH | UCMSB | UCMST | UCMODE_2 | UCSYNC | UCSSEL_2 | UCSTEM;
													//Clock phase = 1
													//Clock polarity = 0
													//MSB first
													//8-bit data
													//Master mode
													//SPI mode / 4-pin SPI with CS active low
													//Synchronous mode
													//Clock source = SMCLK
													//STE = Chip select for a 4-wire slave

		UCA1BRW = 0x0002;							//Clock prescaler = /2
													//SMCLK = 2 MHz => BRCLK = 1 MHz

		UCA1CTLW0 &= ~UCSWRST;						//Get USCI_A1 out of reset state
}

void touch_initialize(touch_t* touch)
{
		P1IE |= BIT3;								//Enable DRDY interrupt
}

void touch_request_position(touch_t* touch)
{
	P3OUT &= ~BIT7;							//Enable CS

		static uint8_t touch_xPos_low, touch_xPos_high;
		static uint8_t touch_yPos_low, touch_yPos_high;

		static uint16_t touch_xPos, touch_yPos;

	//Request touchscreen x position
		touch_serial_send(TOUCH_X_POS);
		touch_xPos_high = touch_serial_send(0x00);
		touch_xPos_low = touch_serial_send(0x00);

	//Request touchscreen y position
		touch_serial_send(TOUCH_Y_POS);
		touch_yPos_high = touch_serial_send(0x00);
		touch_yPos_low = touch_serial_send(0x00);

	P3OUT |= BIT7;							//Disable CS

	//Rebuild 12-bit positions
		touch_xPos = (((uint16_t) touch_xPos_high) << 5) | (((uint16_t) touch_xPos_low) >> 3);
		touch_yPos = (((uint16_t) touch_yPos_high) << 5) | (((uint16_t) touch_yPos_low) >> 3);

		touch_coordinate_set(&touch->touch_last_position, touch_xPos, touch_yPos);
}

