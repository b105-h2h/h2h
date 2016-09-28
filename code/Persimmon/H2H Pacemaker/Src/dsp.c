/*
 * filters.c
 *
 *  Created on: 15/12/2015
 *      Author: tvalno
 */

#include "dsp.h"


#define POLE_CONSTANT 1022
#define LP_151_HZ_ORDER 71
#define LP_151_HZ_FOLDING_CONST  ((LP_151_HZ_ORDER - 1) >> 1)
#define DIF_ORDER 10
#define DIF_FOLDING_CONST  (DIF_ORDER >> 1)
#define INTEGRATION_LENGTH 16
#define INTEGRATION_FACTOR 4            // Shifting factor
/* The following coeficients were obtained with matlab tool "filterbuilder"     */
/* All of the coeficients are stored to make it clearer for the reader, but     */
/* we will only use one half ( because they are simetric ).                     */
/* Design parameters are annotated in case someone wants to reproduce it and    */
/* take a deeper look. The scaling factor for fixed point is 1024               */

/* FIR low pass filter with f3db = 151 Hz (500SPS)                              */
/* fpass = 145	fstop=165	ripple = 0.1	at = 60                         */

const int32_t low_pass_coef_151hz[LP_151_HZ_FOLDING_CONST + 1] = { 
  0, 2, 2, -2, 0, 2, -1, -2, 3, 0, -4, 4, 3, -7, 2, 7, -8, -3, 12, -6, -10, 16, 
  0, -21, 16, 13, -32, 8, 36, -42, -15, 79, -49, -108, 304, 631};

/* FIR all pass differenciator filter                                          */
/* fpass = 500	fstop=500	ripple = 0.1	at = 60(irrelevant)            */
/* Note that in this case the coeficients are anti-simetric                    */
const int32_t all_pass_diff_coef[DIF_ORDER] = {
  20, -40, 56, -148, 1306, -1306, 148, -56, 40, -20};

int32_t dc_blocker(int32_t value)
{
  static int32_t prev_x = 0, prev_y = 0;
  int32_t dife = 0, inte = 0;
  
  dife = value - prev_x;
  prev_x = value;
  
  inte = ((POLE_CONSTANT * prev_y) >> 10) + dife;
  prev_y = inte;
  
  return inte;
}

//FOLDED VERSION
int32_t low_pass_filterino_151Hz(int32_t value){
  static int32_t lp_buffer_151Hz[LP_151_HZ_ORDER] = {0};
  int i;
  int32_t y_n = 0; 

  lp_buffer_151Hz[0] = value;
  
  for(i = 0 ; i < LP_151_HZ_FOLDING_CONST - 1; i++ ){
    y_n += ((low_pass_coef_151hz[i] * 
             (lp_buffer_151Hz[i] + lp_buffer_151Hz[(LP_151_HZ_ORDER - 1) - i]))>>10);
  } 
  y_n += ((low_pass_coef_151hz[LP_151_HZ_FOLDING_CONST] 
           * lp_buffer_151Hz[LP_151_HZ_FOLDING_CONST])>>10);
  
  for(i = (LP_151_HZ_ORDER - 1) ; i > 0; i-- ){
    lp_buffer_151Hz[i] = lp_buffer_151Hz[i-1];
  }  
 
  return y_n;
}

int32_t show_filter(int32_t value)
{
  int32_t dc_blocked, lp_filtered;
  
  dc_blocked = dc_blocker(value);
  lp_filtered = low_pass_filterino_151Hz(dc_blocked);
  
  return lp_filtered;
}

int32_t diferentiator_3000(int32_t value){
  static int32_t dif_buffer[DIF_ORDER] = {0};
  int i;
  int32_t y_n = 0; 

  dif_buffer[0] = value;
  
  for(i = 0 ; i < DIF_FOLDING_CONST; i++ ){
    y_n += ((all_pass_diff_coef[i] * 
             (dif_buffer[i] - dif_buffer[(DIF_ORDER - 1) - i]))>>10);
  } 
  
  for(i = (DIF_ORDER - 1) ; i > 0; i-- ){
    dif_buffer[i] = dif_buffer[i-1];
  }  
 
  return y_n;
}


int32_t bpm_preprocessing(int32_t value)
{
  int32_t dif_filtered;

  dif_filtered = diferentiator_3000(value);

  return dif_filtered;

}