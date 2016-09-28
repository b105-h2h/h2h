/*
 * ecg_data.c
 *
 *  Created on: 29/9/2015
 *      Author: slopez
 */

#include "ecg_data.h"

void ecg_data_clear(ecg_data_t* ecg_data)
{
	ecg_data->data = 0x0000;
}

void ecg_data_write(ecg_data_t* ecg_data, uint8_t msbyte, uint8_t midbyte, uint8_t lsbyte)
{
	int data_tmp = 0x0000;

	//Make 16 bit data
	data_tmp = ((int) msbyte << 12) | ((int) midbyte << 4) | ((int) lsbyte >> 4);

	//Store data
	ecg_data->data = (int) data_tmp ;
}

void ecg_data_copy(ecg_data_t* origin, ecg_data_t* destination)
{
	destination->data = origin->data;
}
