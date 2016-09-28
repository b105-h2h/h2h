/*
 * filters.c
 *
 *  Created on: 15/12/2015
 *      Author: tvalno
 */

#include "dsp.h"


#define POLE_CONSTANT 1022
#define LP_151_HZ_ORDER 141
#define LP_151_HZ_FOLDING_CONST  ((LP_151_HZ_ORDER - 1) >> 1)
#define LP_41_HZ_ORDER 146
#define LP_41_HZ_FOLDING_CONST  (LP_151_HZ_ORDER >> 1)
#define DIF_ORDER 10
#define DIF_FOLDING_CONST  (DIF_ORDER >> 1)
#define INTEGRATION_LENGTH 16
#define INTEGRATION_FACTOR 4            // Shifting factor
/* The following coeficients were obtained with matlab tool "filterbuilder"     */
/* All of the coeficients are stored to make it clearer for the reader, but     */
/* we will only use one half ( because they are simetric ).                     */
/* Design parameters are annotated in case someone wants to reproduce it and    */
/* take a deeper look. The scaling factor for fixed point is 1024               */

/* FIR low pass filter with f3db = 151 Hz                                       */
/* fpass = 145	fstop=165	ripple = 0.1	at = 60                         */

const int32_t low_pass_coef_151hz[LP_151_HZ_ORDER] = { 
  0, 1, 1, 1, 1, 0, -1, -1, 0, 1, 1, 1, -1, -1, -1, 0,2, 2, 0, -2, -2, -1, 2, 3,
  1, -2, -3, -2, 1, 4, 4, 0, -4, -5, -1, 4, 6, 3, -3, -7, -5, 2, 8, 8, 0, -8, 
  -10, -3, 8, 13, 7, -7, -16, -12, 4, 19, 18, 0, -21, -27, -8, 23, 40, 21, -25,
  -64, -54, 26, 152, 268, 315, 268, 152, 26, -54, -64, -25, 21, 40, 23, -8, -27,
  -21, 0, 18, 19, 4, -12,-16, -7, 7, 13, 8, -3, -10, -8, 0, 8, 8, 2, -5, -7, -3, 
  3, 6, 4, -1, -5, -4, 0, 4, 4, 1,-2, -3, -2, 1, 3, 2, -1, -2, -2, 0, 2, 2, 0,
  -1,-1, -1, 1, 1, 1, 0, -1, -1, 0, 1, 1, 1, 1, 0};

/* FIR low pass filter with f3db = 41 Hz                                       */
/* fpass = 35	fstop=45	ripple = 0.1	at = 60                        */

const int32_t low_pass_coef_41hz[LP_41_HZ_ORDER] = {
  1, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 1, 1, 2, 2, 2, 2, 
  2, 2, 1, 1, 0, -1, -2, -3, -4, -4, -5, -4, -4, -3, -2, -1, 1, 3, 5, 6, 8, 8, 
  9, 8, 7, 5, 2, -1, -4, -8, -11, -14, -17, -18, -18, -16, -13, -8, -1, 7, 16,
  27, 37, 48, 59, 68, 76, 83, 87, 90, 90, 87, 83, 76, 68, 59, 48, 37, 27, 16, 7,
  -1, -8, -13, -16, -18, -18, -17, -14, -11, -8, -4, -1, 2, 5, 7, 8, 9, 8, 8, 6,
  5, 3, 1, -1, -2, -3, -4, -4, -5, -4, -4, -3, -2, -1, 0, 1, 1, 2, 2, 2, 2, 2,
  2, 1, 1, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 1};

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
  
  for(i = 0 ; i < LP_151_HZ_FOLDING_CONST; i++ ){
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

int32_t low_pass_filterino_41Hz(int32_t value){
  static int32_t lp_buffer_41Hz[LP_41_HZ_ORDER] = {0};
  int i;
  int32_t y_n = 0; 

  lp_buffer_41Hz[0] = value;
  
  for(i = 0 ; i < LP_41_HZ_FOLDING_CONST; i++ ){
    y_n += ((low_pass_coef_41hz[i] * 
             (lp_buffer_41Hz[i] + lp_buffer_41Hz[(LP_41_HZ_ORDER - 1) - i]))>>10);
  } 

  for(i = (LP_41_HZ_ORDER - 1) ; i > 0; i-- ){
    lp_buffer_41Hz[i] = lp_buffer_41Hz[i-1];
  }  
 
  return y_n;
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

/* This is supposed to be the fastest way, yet not the cleanest nor the faciest*/
int32_t chuster_abs(int32_t value) 
{
  return value * ((value>0) - (value<0));
}

int32_t integrator_3000(int32_t value)
{
  static int32_t buffer_x[INTEGRATION_LENGTH] = {0};
  int32_t x_n = value, y_n = 0;
  uint8_t i;

  for ( i = INTEGRATION_LENGTH - 1; i > 0; i--){
    y_n += buffer_x[i];
    buffer_x[i] = buffer_x[i-1];
  }

  y_n += buffer_x[0];
  buffer_x[0] = x_n;

  return (y_n >> INTEGRATION_FACTOR);
}

int32_t bpm_preprocessing(int32_t value)
{
  static int32_t normalize_value = 1;
  int32_t lp_filtered, dif_filtered, fixed_point_normalized ,squared, integrated;
  float normalized;
  
  if( chuster_abs(value) > normalize_value)
  {
    normalize_value = chuster_abs(value);
  }
  else if (normalize_value > 1)
  {
    normalize_value = ((normalize_value*511)>>9);
  }
  
/* we limit amplitude between +- 10000 ( +-1 if we have float hardware support */
  normalized = (float)value/(float)normalize_value;
  fixed_point_normalized = (int32_t)(normalized * 10000);
  
  dif_filtered = diferentiator_3000(fixed_point_normalized);
  lp_filtered = low_pass_filterino_41Hz(dif_filtered);

  /* We elminate the negative part of the dif signal since it introduce one     */
  /* aditional max which distords the detection( sometimes this max is bigger   */
  /* than the one we want to detect)                                            */
//   if( lp_filtered > 0){
    squared = lp_filtered*lp_filtered;
    /* squared max value = 10000*10000 * Gdiff * Glp, so we to "renormalize"    */
    squared = squared / 1000;
    integrated = integrator_3000(squared);
    
    return integrated;
//  }
//  else
//  {
//    return 0;
//  }
}