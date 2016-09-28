/*
 * authentication.c
 *
 *  Created on: 15/12/2015
 *      Author: tvalno
 */

#include "authentication.h"

#define MASK_X4 0x1111111111111111
#define MASK_X3 0x2222222222222222
#define MASK_X2 0x4444444444444444
#define MASK_X1 0x8888888888888888

#define N_P_THRESHOLD -102898

/* Tables with the log of the probabilty of each position x4,x3,x2,x1 and the */
/* adversary probability( which is constant for every position)               */

static const int32_t log_Q_x4[NUMBER_OF_IPI +1] = {
  -151679, -2174278, -4991369, -8305964, -11999925, -16011799, -20306089, 
  -24861958, -29668323, -34721697, -40025567, -45591016, -51438881, -57604660, 
  -64149803, -71192452, -79029591};

static const int32_t log_Q_x3[NUMBER_OF_IPI +1] = {
  -304741, -1590957, -3671663, -6249875, -9207451, -12482942, -16040848, 
  -19860333, -23930314, -28247304, -32814790, -37643855, -42755337, -48184732, 
  -53993491, -60299755, -67400511};

static const int32_t log_Q_x2[NUMBER_OF_IPI +1] = {
  -667412, -1120213, -2367504, -4112301, -6236462, -8678536, -11403027, 
  -14389097, -17625663, -21109237, -24843308, -28838958, -33117024, -37713004, 
  -42688348, -48161197, -54428537};

static const int32_t log_Q_x1[NUMBER_OF_IPI +1] = {
  -1398911, -1052628, -1500835, -2446548, -3771625, -5414616, -7340023, 
  -9527009, -11964491, -14648982, -17583969, -20780535, -24259517, -28056413, 
  -32232673, -36906439, -42374695};

static const int32_t log_P_x[NUMBER_OF_IPI +1] = {
 -11629080, -8721810, -6609031, -4993757, -3757848, -2839852, -2204273, 
 -1830272, -1706768, -1830272, -2204273, -2839852, -3757848, -4993757, 
 -6609031, -8721810, -11629080};

void init_key(validation_key_t* key, key_origin_t origin)
{
  key->token = 0;
  key->index = 0;
  key->state = NOT_READY;
  key->origin = origin;  
}

key_state_t write_key(uint32_t bpm, validation_key_t* key)
{
  uint64_t IPI = 0;
  
  IPI =  (((uint64_t)bpm) & 0xF);
  /* We round F to 0 to avoid increasing error reate innecesaryly */
  if( IPI == 0xF)
  {
    IPI = 0;
  }
  key->token |= ( IPI << ( 4 * key->index));
  key->index++;
  if (key->index > 15 )
  {
    key->state = READY;
  }
  return key->state;
}

void write_token_key(validation_key_t* key, uint64_t token)
{
  key->token = token;
  key->index = 15;
  key->state = READY;
}

void erase_key(validation_key_t* key)
{
  key->token = 0;
  key->index = 0;
  key->state = NOT_READY;
}

autentitication_t validate(validation_key_t* master_key, validation_key_t* received_key)
{
  uint64_t diff = 0;
  uint64_t diff_x4 = 0,diff_x3 = 0,diff_x2 = 0,diff_x1 = 0;
  uint8_t dif_bits_x4 = 0,dif_bits_x3 = 0,dif_bits_x2 = 0,dif_bits_x1 = 0;
  int32_t hypothesis = 0;
  diff = (master_key->token) ^ (received_key->token);
  
  // we mask and align so the first posible one is the least significant bit */
  diff_x4 = diff & MASK_X4;
  diff_x3 = ((diff & MASK_X3)>>1);
  diff_x2 = ((diff & MASK_X2)>>2);
  diff_x1 = ((diff & MASK_X1)>>3);
  
  while(diff_x4 > 0)
  {
    if( diff_x4 & 0xF)
    { 
      dif_bits_x4++;
    }
    diff_x4 = diff_x4 >> 4;
  }
  
  while(diff_x3 > 0)
  {
    if( diff_x3 & 0xF)
    { 
      dif_bits_x3++;
    }
    diff_x3 = diff_x3 >> 4;
  }  
  
  while(diff_x2 > 0)
  {
    if( diff_x2 & 0xF)
    { 
      dif_bits_x2++;
    }
    diff_x2 = diff_x2 >> 4;
  } 
  while(diff_x1 > 0)
  {
    if( diff_x1 & 0xF)
    { 
      dif_bits_x1++;
    }
    diff_x1 = diff_x1 >> 4;
  }  
  
  hypothesis = (log_P_x[dif_bits_x4] + log_P_x[dif_bits_x3] +
               log_P_x[dif_bits_x2] +log_P_x[dif_bits_x1]) -
               (log_Q_x4[dif_bits_x4] + log_Q_x3[dif_bits_x3] +
               log_Q_x2[dif_bits_x2] +log_Q_x1[dif_bits_x1]);
  
  if(hypothesis < N_P_THRESHOLD)
  {          
    return ACCEPTED;
    
  }
  else
  {
    return REJECTED;
  }
}
