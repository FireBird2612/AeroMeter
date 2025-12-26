#include "stm32f446xx.h"
#include "system_stm32f4xx.h"
#include "config.h"
#include <stdint.h>

static uint32_t sys_clk;

void SystemInit(void) {
  ClockInit();
}

void ClockInit(void) {
  uint32_t timeout = 0xFFFFFF, set_hse = 0;

  /*  Enable the HSE crystal osc. and check if HSE is ready */
  RCC->CR |= RCC_CR_HSEON_Msk;
  while (!((RCC->CR & RCC_CR_HSERDY_Msk) && (--timeout)))
    ;

  if (timeout == 0) {
    /*  Log debug message and fallback to internal osc */

    timeout = 0xFFFFFF;
  } else {
    set_hse = 1;
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

  if ((set_hse = 1))
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

}

uint32_t get_sys_clk(void){
  return sys_clk;
}