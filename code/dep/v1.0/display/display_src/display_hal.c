/*
 * display_hal.c
 *
 *  Created on: 29/9/2015
 *      Author: slopez
 */

#include "display_hal.h"

void display_hal_input()
{
	P6DIR |= 0x7F;					//Clear P6.0 to P6.6
	P6DIR &= ~0x7F;					//Set P6.0 to P6.6 as input
	P7DIR &= ~BIT0;					//Set P7.0 as input
}

void display_hal_output()
{
	P6DIR &= ~0x7F;					//Clear P6.0 to P6.6
	P6DIR |= 0x7F;					//Set P6.0 to P6.6 as output
	P7DIR |= BIT0;					//Set P7.0 as output
}

void display_hal_screen_on()
{
	P9OUT &= ~BIT6;					//Turn screen on
}

void display_hal_screen_off()
{
	P9OUT |= BIT6;					//Turn screen off
}

static void display_hal_write(uint8_t data_out)
{
	//display_hal_output();			//Set GPIO as output

	P6OUT &= ~0x7F;					//Clear P6.0 to P6.6
	P6OUT |= 0x7F & (data_out >> 1);	//Write 7 MSB of data to P6.0 to P6.6

	P7OUT &= ~0x01;					//Clear P7.0
	P7OUT |= 0x01 & data_out;			//Write LSB of data to P7.0
}

static uint8_t display_hal_read()
{
	uint8_t data_in = 0x00;			//Returned value

	//display_hal_input();			//Set GPIO as input

	data_in |= (P6IN & 0x7F) << 1;	//Read from P6.0 to P6.6
	data_in &= ~0x01;				//Clear LSB in data_in
	data_in |= (P7IN & 0x01);		//Read LSB from P7.0

	return data_in;
}

uint16_t display_hal_read_reg(uint8_t reg)
{
	uint8_t data_hi_af = 0;
	uint8_t data_lo = 0;
	uint16_t data = 0;

	//Start operation
		P9OUT &= ~BIT7;			//Set display_CS to '0'

	//Write command
		P2OUT &= ~BIT3;			//Set display_RS to '0'

		display_hal_write(0x00);
		P9OUT &= ~BIT4;			//Set display_WR to '0'
		P9OUT |= BIT4;			//Set display_WR to '1'

		display_hal_write(reg);	//Write register to be accessed
		P9OUT &= ~BIT4;			//Set display_WR to '0'
		P9OUT |= BIT4;			//Set display_WR to '1'

	//Read data
		P2OUT |= BIT3;			//Set display_RS to '1'

		P9OUT &= ~BIT5;			//Set display_RD to '0'
		data_hi_af = display_hal_read();
		P9OUT |= BIT5;			//Set display_RD to '1'

		P9OUT &= ~BIT5;			//Set display_RD to '0'
		data_lo = display_hal_read();
		P9OUT |= BIT5;			//Set display_RD to '1'

	//End operation
		P9OUT |= BIT7;			//Set display_CS to '1'

	data = ((uint16_t) data_hi_af << 8) | data_lo;

	return data;
}

void display_hal_write_reg(uint8_t reg, uint8_t data_hi, uint8_t data_lo)
{
	//Start operation
		P9OUT &= ~BIT7;			//Set display_CS to '0'

	//Write command
		P2OUT &= ~BIT3;			//Set display_RS to '0'

		display_hal_write(0x00);
		P9OUT &= ~BIT4;			//Set display_WR to '0'
		P9OUT |= BIT4;			//Set display_WR to '1'

		display_hal_write(reg);	//Write register to be accessed
		P9OUT &= ~BIT4;			//Set display_WR to '0'
		P9OUT |= BIT4;			//Set display_WR to '1'

	//Write data
		P2OUT |= BIT3;			//Set display_RS to '1'

		display_hal_write(data_hi);
		P9OUT &= ~BIT4;			//Set display_WR to '0'
		P9OUT |= BIT4;			//Set display_WR to '1'

		display_hal_write(data_lo);
		P9OUT &= ~BIT4;			//Set display_WR to '0'
		P9OUT |= BIT4;			//Set display_WR to '1'

	//End operation
		P9OUT |= BIT7;			//Set display_CS to '1'
}

void display_hal_write_GRAM(color_t color)
{
	//Start operation
		P9OUT &= ~BIT7;			//Set display_CS to '0'

	//Write command
		P2OUT &= ~BIT3;			//Set display_RS to '0'

		display_hal_write(0x00);
		P9OUT &= ~BIT4;			//Set display_WR to '0'
		P9OUT |= BIT4;			//Set display_WR to '1'

	//Request GRAM writing operation
		display_hal_write(0x22);
		P9OUT &= ~BIT4;			//Set display_WR to '0'
		P9OUT |= BIT4;			//Set display_WR to '1'

	//Write data
		P2OUT |= BIT3;			//Set display_RS to '1'

		display_hal_write(color.red);
		P9OUT &= ~BIT4;			//Set display_WR to '0'
		P9OUT |= BIT4;			//Set display_WR to '1'

		display_hal_write(color.green);
		P9OUT &= ~BIT4;			//Set display_WR to '0'
		P9OUT |= BIT4;			//Set display_WR to '1'

		display_hal_write(color.blue);
		P9OUT &= ~BIT4;			//Set display_WR to '0'
		P9OUT |= BIT4;			//Set display_WR to '1'

	//End operation
		P9OUT |= BIT7;			//Set display_CS to '1'
}

void display_hal_set_GRAM_position(uint16_t x, uint16_t y)
{
	uint8_t x_low, x_high, y_low, y_high;

	x_low = (uint8_t) x;
	x_high = (uint8_t) (x>>8);
	y_low = (uint8_t) y;
	y_high = (uint8_t) (y>>8);

	display_hal_write_reg(0x21, x_high, x_low);		//X position register
	display_hal_write_reg(0x20, y_high, y_low);		//Y position register
}
