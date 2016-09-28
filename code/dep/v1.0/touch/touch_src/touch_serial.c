/*
 * touch_serial.c
 *
 *  Created on: 29/9/2015
 *      Author: Smau
 */

#include "touch_serial.h"

uint8_t touch_serial_send(uint8_t data)
{
	__delay_cycles(120);                    //Wait 7.5us (assuming 24MHz MCLK), required to allow t_sdecode to be 4 T_clk

	UCA1RXBUF;								//Dummy read to empty RX buffer
											//and clear any overrun conditions
	while (!(UCA1IFG & UCTXIFG));           //Wait while not ready for TX

	UCA1TXBUF = data;				//Transmit byte (automatically clears UCTXIFG)
    while(UCA1STATW & UCBUSY);					//Wait for transmission
    while (!(UCA1IFG & UCRXIFG));

    return UCA1RXBUF;				//Return received data if any
}

uint8_t touch_send(uint8_t data)
{
	uint8_t rxValue = 0x00;

	P3OUT &= ~BIT7;							//Enable CS

	UCA1RXBUF;								//Dummy read to empty RX buffer
											//and clear any overrun conditions
	while (!(UCA1IFG & UCTXIFG));           //Wait while not ready for TX

	UCA1TXBUF = data;				//Transmit byte (automatically clears UCTXIFG)
    while(UCA1STATW & UCBUSY);					//Wait for transmission

    rxValue = UCA1RXBUF;				//Return received data if any
	P3OUT |= BIT7;							//Disable CS

	return rxValue;
}
