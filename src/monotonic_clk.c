#include "timer.h"
#include "stdint.h"
#include "config.h"

static uint32_t monotonic_clk_overflow;

void monotonic_clk_init(void)
{
  RCC->APB1ENR |= RCC_APB1ENR_TIM5EN_Msk;
  TIM5->ARR = 0xFFFFFFFF;
  TIM5->CNT = 0;

  uint32_t psc_val = (APB1_MAX_FREQ / 1000000) - 1;
  TIM5->PSC = psc_val;

  TIM5->DIER |= TIM_DIER_UIE_Msk;

  TIM5->EGR |= TIM_EGR_UG_Msk;

  NVIC_EnableIRQ(TIM5_IRQn);
  NVIC_SetPriority(TIM5_IRQn, 5);

  TIM5->CR1 |= TIM_CR1_CEN_Msk;
}

void TIM5_IRQHandler(void)
{
  if (TIM5->SR & TIM_SR_UIF_Msk)
  {
    TIM5->SR &= ~TIM_SR_UIF_Msk;
  }
  monotonic_clk_overflow++;
}