/*
 * leads.h
 *
 *  Created on: 20/06/2016
 *      Author: slopez
 */
#ifndef LEADS_H_
#define LEADS_H_

#include <stdint.h>

typedef struct leads_t
{
  int32_t lead_I;
  int32_t lead_II;
  int32_t lead_III;
  int32_t lead_aVR;
  int32_t lead_aVL;
  int32_t lead_aVF;  
}
leads_t;

#endif /* LEADS_H_ */