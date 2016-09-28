//const int32_t IIR_band_pass_coef[30] = { 1024, -1857, 1024, -1881, 981, // Coeficients for first stage
//									1024, -2036, 1024, -2013, 1007, // Coeficients for second stage
//									1024, -1463, 1024, -1878, 928, 	// Coeficients for third stage
//									1024, -2044, 1024, -1957, 963, 	// Coeficients for fourth stage
//									1024, -1888, 1024, -1896, 1015, // Coeficients for fifth stage
//									1024, -2034, 1024, -2029, 1021};// Coeficients for sixth stage

//
//const int32_t high_pass_coef[H_ORDER] = {-7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
//		-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3,
//		-3, -3, -3, -3, -3, -3, -3, -3, -3, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4,
//		-4, -4, -4, -4, -4, -4, -4, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5,
//		-5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5,
//		-5, -5, 1019, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5,
//		-5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -4, -4,
//		-4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -3, -3, -3, -3,
//		-3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -2, -2, -2, -2, -2, -2, -2,
//		-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -7
//
//};
//
//const int32_t low_pass_coef[L_ORDER] = {-4, -4, 1, 12, 24, 26, 14, -6, -21, -17, 7, 32, 33, 1, -46, -65, -22, 85, 215,
//		303, 303, 215, 85, -22, -65, -46, 1, 33, 32, 7, -17, -21, -6, 14, 26, 24, 12, 1, -4, -4};

//const int32_t diferentiator_coef[5] = { 2, 1, 0, -1, -2};

/*
int32_t IIR_band_pass_filterino(int32_t value)
{
	static int32_t buffer_x_0[2] = {0,0},buffer_x_1[2] = {0,0},
			buffer_x_2[2] = {0,0},buffer_x_3[2] = {0,0},
			buffer_x_4[2] = {0,0},buffer_x_5[2] = {0,0};

	static int32_t buffer_y_0[2] = {0,0},buffer_y_1[2] = {0,0},
			buffer_y_2[2] = {0,0},buffer_y_3[2] = {0,0},
			buffer_y_4[2] = {0,0},buffer_y_5[2] = {0,0};

	int32_t x_n = value, y_n = 0;

    y_n = (band_pass_coef[0] * x_n + band_pass_coef[1] * buffer_x_0[0]
			+ band_pass_coef[2] * buffer_x_0[1] - band_pass_coef[3] * buffer_y_0[0]
			- band_pass_coef[4] * buffer_y_0[1])>>10;

    buffer_y_0[1] = buffer_y_0[0];
    buffer_y_0[0] = y_n;
    buffer_x_0[1] = buffer_x_0[0];
    buffer_x_0[0] = x_n;
    x_n = y_n;

    y_n = (band_pass_coef[5] * x_n + band_pass_coef[6] * buffer_x_1[0]
        		+ band_pass_coef[7] * buffer_x_1[1] - band_pass_coef[8] * buffer_y_1[0]
    			- band_pass_coef[9] * buffer_y_1[1])>>10;
    buffer_y_1[1] = buffer_y_1[0];
    buffer_y_1[0] = y_n;
    buffer_x_1[1] = buffer_x_1[0];
    buffer_x_1[0] = x_n;

    x_n = y_n;

    y_n = (band_pass_coef[10] * x_n + band_pass_coef[11] * buffer_x_2[0]
    			+ band_pass_coef[12] * buffer_x_2[1] - band_pass_coef[13] * buffer_y_2[0]
    			- band_pass_coef[14] * buffer_y_2[1])>>10;
    buffer_y_2[1] = buffer_y_2[0];
    buffer_y_2[0] = y_n;
    buffer_x_2[1] = buffer_x_2[0];
    buffer_x_2[0] = x_n;

    x_n = y_n;

    y_n = (band_pass_coef[15] * x_n + band_pass_coef[16] * buffer_x_3[0]
        		+ band_pass_coef[17] * buffer_x_3[1] - band_pass_coef[18] * buffer_y_3[0]
    			- band_pass_coef[19] * buffer_y_3[1])>>10;
    buffer_y_3[1] = buffer_y_3[0];
    buffer_y_3[0] = y_n;
    buffer_x_3[1] = buffer_x_3[0];
    buffer_x_3[0] = x_n;
    x_n = y_n;

    y_n = (band_pass_coef[20] * x_n + band_pass_coef[21] * buffer_x_4[0]
        		+ band_pass_coef[22] * buffer_x_4[1]	- band_pass_coef[23] * buffer_y_4[0]
    			- band_pass_coef[24] * buffer_y_4[1])>>10;
    buffer_y_4[1] = buffer_y_4[0];
    buffer_y_4[0] = y_n;
    buffer_x_4[1] = buffer_x_4[0];
    buffer_x_4[0] = x_n;
    x_n = y_n;

    y_n = (band_pass_coef[25] * x_n + band_pass_coef[26] * buffer_x_5[0]
        		+ band_pass_coef[27] * buffer_x_5[1] - band_pass_coef[28] * buffer_y_5[0]
    			- band_pass_coef[29] * buffer_y_5[1])>>10;
    buffer_y_5[1] = buffer_y_5[0];
    buffer_y_5[0] = y_n;
    buffer_x_5[1] = buffer_x_5[0];
    buffer_x_5[0] = x_n;

	return y_n;
}
*/

//
//int32_t high_pass_filterino(int32_t value)
//{
//	static int32_t buffer[(H_ORDER - 1)];
//	int i;
//	int32_t y_n = 0;
//
//	for(i = (H_ORDER - 2); i > 0; i-- ){
//        y_n += (band_pass_coef[i + 1]) * (buffer[i]);
//        buffer[i] = buffer[i-1];
//	}
//
//    y_n += band_pass_coef[1] * buffer[0];
//    buffer[0] = value;
//
//    y_n = y_n + band_pass_coef[0] * value;
//
//    y_n = (y_n >> 10);
//
//    return y_n;
//}
//
//int32_t low_pass_filterino(int32_t value)
//{
//	static int32_t buffer[(L_ORDER - 1)];
//	int i;
//	int32_t y_n = 0;
//
//	for(i = (L_ORDER - 2); i > 0; i-- ){
//        y_n += (low_pass_coef[i + 1]) * (buffer[i]);
//        buffer[i] = buffer[i-1];
//	}
//
//    y_n += low_pass_coef[1] * buffer[0];
//    buffer[0] = value;
//
//    y_n = y_n + low_pass_coef[0] * value;
//
//    y_n = (y_n >> 10);
//
//    return y_n;
//}
//int32_t diferentiator_3000(int32_t value)
//{
//	static int32_t buffer_x[4] = {0,0,0,0};
//	int32_t x_n = value, y_n = 0;
//	uint8_t i;
//
//	y_n = (diferentiator_coef[0] * x_n + diferentiator_coef[1] * buffer_x[0]
//				+ diferentiator_coef[2] * buffer_x[1] + diferentiator_coef[3] * buffer_x[2]
//				+ diferentiator_coef[4] * buffer_x[3])>>3;
//
//    for ( i = 3; i > 0; i--)
//    {
//		buffer_x[i] = buffer_x[i-1];
//    }
//    buffer_x[0] = x_n;
//
//	return y_n;
//}
