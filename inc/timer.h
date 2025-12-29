#ifndef __TIMER_H__
#define __TIMER_H__

#include "stm32f446xx.h"
#include "stdint.h"

void monotonic_clk_init(void);

static inline uint32_t get_timestamp(void)
{
  return TIM5->CNT;
}

#endif