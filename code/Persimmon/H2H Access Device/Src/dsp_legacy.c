/*
 * filters.c
 *
 *  Created on: 15/12/2015
 *      Author: tvalno
 */

/* There was a time when only coding and dreaming was important,        */
/* when there was a simply euristic method for bpm detection and        */
/* nothing else mattered. But then it came the project documentatio     */
/* and the neccesity of justify the desing choices and criteria         */
/* and our little world shattered into pieces....                       */
/* I will always love you...                                            */
/* PD: I'm commenting out this our compiler go apeshit crazy ~~         */
//#include "dsp_legacy.h"
//
//#define BP_ORDER 305
//#define BP_FOLD_CONSTANT ((BP_ORDER-1)>>1)
//#define DIF_ORDER 32
//#define DIF_FOLD_CONSTANT (DIF_ORDER>>1)
//#define LP_ORDER 46
//#define LP_FOLD_CONSTANT (LP_ORDER>>1)
//#define INTEGRATION_LENGTH 32
//#define INTPOLE 1022
//
//// FIR BPF 2-35 hz (500 SPS / ORDER 305)
////FOLDED
//const int32_t band_pass_coef[BP_FOLD_CONSTANT + 1] = {
//  -10, -15, 1, -8, -2, -4, -1, -1, 0, 0, 0, 0, -1, -2, -2, -3, -3, -3, -3, -2, 
//  -2, -1, 0, 0, 0, -1, -1, -2, -3, -4, -4, -4, -3, -3, -2, -1, 0, 0, 0, -1, -2,
//  -3, -4, -5, -5, -5, -4, -3, -2, -1, 0, 0, 0, -1, -2, -3, -5, -5, -6, -6, -5,
//  -4, -2, -1, 0, 0, 0, -1, -3, -4, -6, -7, -7, -7, -6, -4, -2, -1, 0, 0, 0, -1,
//  -3, -5, -7, -8, -8, -8, -6, -4, -2, 0, 1, 1, 1, -1, -3, -6, -8, -9, -10, -9, 
//  -7, -5, -2, 1, 2, 3, 2, 0, -4, -7, -10, -12, -13, -11, -9, -5, -1, 3, 5, 6, 4,
//  1, -4, -9, -14, -17, -18, -16, -12, -5, 2, 8, 13, 14, 12, 5, -4, -15, -25, 
//  -33, -37, -33, -23, -5, 18, 46, 75, 101, 123, 137, 142
//};
//// FIR DIF (500 SPS)
////FOLDED
//const int32_t diferentiator_coef[DIF_FOLD_CONSTANT] = {
//  -4, 6, -3, 3, -3, 3, -4, 5, -6, 8, -11, 16, -27, 52, -145, 1304 };
//
//// FIR LPF 140 (f3dB = 150) (500 SPS)
////FOLDED
//const int32_t low_pass_coef[LP_FOLD_CONSTANT] = {  
//  -3, -1, 3, -1, -4, 5, 3, -9, 4, 10, -13, -3, 21, -13, -19, 34, -1, -50, 46, 
//  40, -127, 53, 535};
//
////FOLDED VERSION
//int32_t band_pass_filterino(int32_t value)
//{
//  static int32_t bp_buffer[BP_ORDER] = {0};
//  int i;
//  int32_t y_n = 0; 
//
//  bp_buffer[0] = value;
//
//  y_n += ((band_pass_coef[BP_FOLD_CONSTANT] * bp_buffer[BP_FOLD_CONSTANT])>>10);
//  
//  for(i = 0 ; i < BP_FOLD_CONSTANT; i++ ){
//    y_n += ((band_pass_coef[i] * (bp_buffer[i] + bp_buffer[BP_ORDER - 1 - i]))>>10);
//  } 
//  
//  for(i = (BP_ORDER - 1) ; i > 0; i-- ){
//    bp_buffer[i] = bp_buffer[i-1];
//  }  
// 
//  return y_n;
//}
//
////FOLDED VERSION
//int32_t diferentiator_3000(int32_t value){
//  static int32_t dif_buffer[DIF_ORDER] = {0};
//  int i;
//  int32_t y_n = 0; 
//
//  dif_buffer[0] = value;
//  
//  for(i = 0 ; i < DIF_FOLD_CONSTANT; i++ ){
//    y_n += ((diferentiator_coef[i] * (dif_buffer[(DIF_ORDER - 1) - i] - dif_buffer[i]))>>10);
//  } 
//  
//  for(i = (DIF_ORDER - 1) ; i > 0; i-- ){
//    dif_buffer[i] = dif_buffer[i-1];
//  }  
// 
//  return y_n;
//}
//
//float integrator_3000(float value)
//{
//  static float buffer_x[INTEGRATION_LENGTH] = {0};
//  float x_n = value, y_n = 0;
//  uint8_t i;
//
//  for ( i = INTEGRATION_LENGTH - 1; i > 0; i--){
//    y_n += buffer_x[i];
//    buffer_x[i] = buffer_x[i-1];
//  }
//
//  y_n += buffer_x[0];
//  buffer_x[0] = x_n;
//
//  return (y_n / INTEGRATION_LENGTH);
//}
//
//int32_t chuster_abs(int32_t value){
//  if(value > 0){
//    return value;
//  }else{
//    return (-1)*value;
//  }
//}
//
//int32_t bpm_preprocessing_legacy(int32_t value)
//{
//  int32_t bp_filtered, dif_filtered, denormalized;
//  static int32_t max_value = 1000;
//  float normalized, squared, integrated;
//
//  bp_filtered = band_pass_filterino(value);
//  dif_filtered = diferentiator_3000(bp_filtered);
//  
//  if( (chuster_abs(dif_filtered) > max_value)){ 
//     max_value = chuster_abs(dif_filtered);
//  }else{
//    max_value = ((max_value > 1) ? ((max_value*511)>>9): 1);
//  }
//  normalized = ((float)dif_filtered)/max_value;
//  squared = (normalized) * (normalized);
//  integrated = integrator_3000(squared);
//  denormalized = (int32_t)(integrated * 10000);
//  
//  return denormalized;
//}
//
//int32_t dc_blocker(int32_t value)
//{
//  static int32_t prev_x = 0, prev_y = 0;
//  int32_t dife = 0, inte = 0;
//  
//  dife = value - prev_x;
//  prev_x = value;
//  
//  inte = ((INTPOLE * prev_y) >> 10) + dife;
//  prev_y = inte;
//  
//  return inte;
//}
//
////FOLDED VERSION
//int32_t low_pass_filterino(int32_t value){
//  static int32_t lp_buffer[LP_ORDER] = {0};
//  int i;
//  int32_t y_n = 0; 
//
//  lp_buffer[0] = value;
//  
//  for(i = 0 ; i < LP_FOLD_CONSTANT; i++ ){
//    y_n += ((low_pass_coef[i] * (lp_buffer[i] + lp_buffer[(LP_ORDER - 1) - i]))>>10);
//  } 
//  
//  for(i = (LP_ORDER - 1) ; i > 0; i-- ){
//    lp_buffer[i] = lp_buffer[i-1];
//  }  
// 
//  return y_n;
//}
//
//int32_t show_filter_legacy(int32_t value)
//{
//  int32_t dc_blocked, lp_filtered;
//  
//  dc_blocked = dc_blocker(value);
//  lp_filtered = low_pass_filterino(dc_blocked);
//  
//  return lp_filtered;
//}