#include "stm32f446xx.h"
#include "system_stm32f4xx.h"
#include "config.h"
#include "stm32f4_usart.h"
#include <stdint.h>

static uint32_t sys_clk;
static uint32_t monotonic_clk_overflow;

void SystemInit(void) {
  ClockInit();
  monotonic_clk_init();
  usart2_log_init();
}

void ClockInit(void) {
  uint32_t timeout = 0xFFFFFF, set_hse = 0;

  #ifdef EN_HSE_CLOCK
  /*  Enable the HSE crystal osc. and check if HSE is ready */
  RCC->CR |= RCC_CR_HSEON_Msk;
  while (!((RCC->CR & RCC_CR_HSERDY_Msk) && (--timeout)))
    ;

  #endif

  if (timeout == 0) {
    /*  Log debug message and fallback to internal osc */

    timeout = 0xFFFFFF;
  } else {
    #ifdef EN_HSE_CLOCK
    set_hse = 1;
    #endif
    goto pll;
  }

  /*  Enable HSI Osc. */
  RCC->CR |= RCC_CR_HSION_Msk;
  while (!((RCC->CR & RCC_CR_HSIRDY_Msk) && (--timeout)))
    ;

  if (timeout == 0)
  {
    /*  The control should never come to here */
  }

pll:
  if(RCC->CR & RCC_CR_PLLON_Msk){
    RCC->CR &= ~RCC_CR_PLLON_Msk; 
  }

  uint32_t clock_freq = 0;
  uint32_t plln = 45, pllm = 2, pll_freq = 0, pllp = 2;

  if ((set_hse == 1))
  {
    clock_freq = OSCEX_FEQ;
    pllm = 4;
    plln = 180;

    RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE_Msk;
  }
  else
  {
    clock_freq = OSCIN_FEQ;
  }

  pll_freq = ((clock_freq / pllm) * plln) / pllp;

  if(!(pll_freq == 180000000)){
    /*  log the pll_freq value */
  }

  RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLM_Msk << RCC_PLLCFGR_PLLM_Pos) | ~(RCC_PLLCFGR_PLLN_Msk << RCC_PLLCFGR_PLLN_Pos) | ~(RCC_PLLCFGR_PLLP_Msk << RCC_PLLCFGR_PLLP_Pos);
  RCC->PLLCFGR |= (pllm << RCC_PLLCFGR_PLLM_Pos) | (plln << RCC_PLLCFGR_PLLN_Pos) | (pllp << RCC_PLLCFGR_PLLP_Pos);

  RCC->CR |= RCC_CR_PLLON_Msk;
  timeout = 0xFFFFFF;
  while(!((RCC->CR & RCC_CR_PLLRDY_Msk) && (--timeout)))
    ;

  sys_clk = pll_freq;

  RCC->CFGR |= RCC_CFGR_SW_PLL;

  if((RCC->CFGR & RCC_CFGR_SWS_Msk) != (RCC_CFGR_SWS_PLL)){
    /*  Log: Not able to switch ot PLL */
  }

  RCC->CFGR |= (RCC_CFGR_PPRE1_DIV4 | RCC_CFGR_PPRE2_2);
}

uint32_t get_sys_clk(void){
  return sys_clk;
}

void monotonic_clk_init(void) {
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

void TIM5_IRQHandler(void){
  if (TIM5->SR & TIM_SR_UIF_Msk){
      TIM5->SR &= ~TIM_SR_UIF_Msk;
  }
   monotonic_clk_overflow++;
}