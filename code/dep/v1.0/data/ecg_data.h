/*
 * ecg_data.h
 *
 *  Created on: 29/9/2015
 *      Author: slopez
 */

#ifndef DATA_ECG_DATA_H_
#define DATA_ECG_DATA_H_

#include <msp430.h>
#include <stdint.h>

/*
 * AFE data type
 */
typedef struct ecg_data_t
{
//	int32_t data;
	int data;
}
volatile ecg_data_t;

void	ecg_data_clear(ecg_data_t* data);
void	ecg_data_write(ecg_data_t* data, uint8_t msbyte, uint8_t midbyte, uint8_t lsbyte);
void	ecg_data_copy(ecg_data_t* origin, ecg_data_t* destination);

#endif /* DATA_ECG_DATA_H_ */
