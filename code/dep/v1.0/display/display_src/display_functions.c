/*
 * display_functions.c
 *
 *  Created on: 29/9/2015
 *      Author: slopez
 */

#include "display_functions.h"

/*
 * Font -> Translation from char variables to pixel positions
 */
extern const font_t font;

void display_functions_write_pixel(color_t color, uint16_t x, uint16_t y)
{
	display_hal_set_GRAM_position(x, y);
	display_hal_write_GRAM(color);
}

void display_functions_write_line(color_t color, uint16_t origin_x, uint16_t origin_y,
								  	  	  	  	 uint16_t end_x, uint16_t end_y)
{
	int16_t dist_x, dist_y, step, step_y, current_x, current_y, i, j;

	if(origin_x < end_x)
	{
		current_x = origin_x;
		dist_x = end_x - origin_x;
	}
	else
	{
		current_x = end_x;
		dist_x = origin_x - end_x;
	}

	current_y = origin_y;

	if(origin_y < end_y)
	{
		step_y = 1;
		dist_y = end_y - origin_y + 1;
	}
	else
	{
		step_y = -1;
		dist_y = origin_y - end_y + 1;
	}

	step = dist_y/dist_x;

	for(i = dist_x; i > 0; i--)
	{
		for(j = step; j > 0;j--)
		{
			display_functions_write_pixel(color, current_x, current_y);
			current_y = current_y + step_y;
		}
		current_x++;
	}

}

void display_functions_write_char(char character, color_t color, color_t bg_color,
												  uint16_t x, uint16_t y)
{
	uint8_t* char_starting_position;
	char_starting_position = font_get_char(&font, character);	//Pointer to starting byte

	uint16_t line;
	uint16_t local_x = x;
	uint16_t local_y = y;

	uint16_t i,j;
	uint16_t mask;

	for(i = font.font_height; i > 0; i--){
		line = *char_starting_position++;
		line = (line << 8) | (*char_starting_position++);

		mask = 0x8000;

		//Draw line
		for(j =  font.font_width; j > 0; j--)
		{
			if (line & mask)
			{
				display_functions_write_pixel(color, local_x, local_y);
			}
			else
			{
				display_functions_write_pixel(bg_color, local_x, local_y);
			}
			local_x++;
			mask = mask >> 1;
		}

		//Move to the beginning of the next line
		local_y++;
		local_x = x;
	}
}

void display_functions_write_string(char* string, color_t color, color_t bg_color,
												  uint16_t x, uint16_t y)
{
	unsigned int i = 0;
	char character;

	while ((character = string[i]) != 0)
	{
		i++;
		display_functions_write_char(character, color, bg_color, x, y);		//Write char
		x = x + font.font_width;											//Move to next char position
	}
}

void display_functions_sleep()
{
	display_hal_write_reg(0x07, 0x01, 0x31);	//Set D1=0, D0=1
	delay_ms(10);
	display_hal_write_reg(0x07, 0x01, 0x30);	//Set D1=0, D0=0
	delay_ms(10);
	display_hal_write_reg(0x07, 0x00, 0x00);	//Display OFF

	//************* Power OFF sequence **************//
	display_hal_write_reg(0x10, 0x00, 0x80);	//SAP, BT[3:0], APE, AP, DSTB, SLP
	display_hal_write_reg(0x11, 0x00, 0x00);	//DC1[2:0], DC0[2:0], VC[2:0]
	display_hal_write_reg(0x12, 0x00, 0x00);	//VREG1OUT voltage
	display_hal_write_reg(0x13, 0x00, 0x00);	//VDV[4:0] for VCOM amplitude
	delay_ms(200);									//Dis-charge capacitor power voltage
	display_hal_write_reg(0x10, 0x00, 0x82);	//SAP, BT[3:0], APE, AP, DSTB, SLP.
}

void display_functions_wake_up()
{
	//************* Power ON sequence ***************//
	display_hal_write_reg(0x10, 0x00, 0x80);	//SAP, BT[3:0], AP, DSTB, SLP
	display_hal_write_reg(0x11, 0x00, 0x00);	//DC1[2:0],  DC0[2:0],  VC[2:0]
	display_hal_write_reg(0x12, 0x00, 0x00);	//VREG1OUT voltage
	display_hal_write_reg(0x13, 0x00, 0x00);	//VDV[4:0] for VCOM amplitude
	delay_ms(200);									//Dis-charge capacitor power voltage
	display_hal_write_reg(0x10, 0x12, 0x90);	//SAP, BT[3:0], AP, DSTB, SLP, STB
	display_hal_write_reg(0x11, 0x02, 0x27);	//Set DC1[2:0], DC0[2:0], VC[2:0]
	delay_ms(50);
	display_hal_write_reg(0x12, 0x00, 0x1B);	//External reference voltage = Vci;
	delay_ms(50);
	display_hal_write_reg(0x13, 0x19, 0x00);	//Set VDV[4:0] for VCOM amplitude
	display_hal_write_reg(0x29, 0x00, 0x0F);	//Set VCM[5:0] for VCOMH
	delay_ms(50);
	display_hal_write_reg(0x07, 0x01, 0x33);	//262K color and display ON
}
