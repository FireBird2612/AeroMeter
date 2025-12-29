#include "stm32f446xx.h"
#include "system_stm32f4xx.h"
#include "config.h"
#include "ulogger.h"
#include "timer.h"
#include <stdint.h>

//#define EN_HSE_CLOCK  /* Use HSI only */
#define TIMEOUT   0xFFFF

static uint32_t sys_clk;

void SystemInit(void) {
  ClockInit();
  monotonic_clk_init();
  ulogger_init();
}

void ClockInit(void) {
  uint32_t timeout = TIMEOUT, set_hse = 0;
  RCC->APB1ENR |= RCC_APB1ENR_TIM5EN_Msk;
  TIM5->CNT = 0;

#ifdef EN_HSE_CLOCK
  /*  Enable the HSE crystal osc. and check if HSE is ready */
  RCC->CR |= RCC_CR_HSEON_Msk;
  while (!((RCC->CR & RCC_CR_HSERDY_Msk) && (--timeout)))
    ;
  
  if(timeout == 0){
    ulog_error("Timedout waiting for HSE clock to be ready");
  }
  ulog_info("HSE Osc. selected as clock source");

  set_hse = 1;
#endif

  /*  For configuring power and voltage */
  RCC->APB1ENR |= RCC_APB1ENR_PWREN;
  PWR->CR |= PWR_CR_VOS_Msk;

  /*  Configure Flash Prefetch Enable */
  FLASH->ACR |= (FLASH_ACR_DCEN | FLASH_ACR_ICEN | FLASH_ACR_PRFTEN | FLASH_ACR_LATENCY_5WS);

  /*  Configure the Prescalers for AHB, APB1, APB2  */
  RCC->CFGR &= ~(RCC_CFGR_HPRE_Msk);
  RCC->CFGR |= (RCC_CFGR_PPRE1_DIV4 | RCC_CFGR_PPRE2_2);

  /*  Calculate PLL to be 180MHz  */
  uint32_t pllm = 8, plln = 180, pllp = 2;
  uint32_t pll_freq = 0;
  uint32_t pll_src = 0;

  if ((set_hse == 1))
  {
    /* Using HSE (8MHz): SYSCLK = (8 / 4) * 180 / 2 = 180MHz */
    pllm = 4;
    plln = 180;
    pllp = 2;
    pll_src = 1;  /* HSE source */
    pll_freq = ((OSCEX_FEQ * plln) / (pllm * pllp));
  }
  else
  {
    /* Using HSI (16MHz): SYSCLK = (16 / 8) * 180 / 2 = 180MHz */
    pll_freq = ((OSCIN_FEQ * plln) / (pllm * pllp));
  }
  
  if(!(pll_freq == 180000000)){
    /*  log the pll_freq value */
    ulog_error("pll_freq is not set to 180000000");
  }

  /* Set PLLCFGR register completely */
  RCC->PLLCFGR &= ~((RCC_PLLCFGR_PLLM_Msk | RCC_PLLCFGR_PLLN_Msk) | (RCC_PLLCFGR_PLLP_Msk) | (RCC_PLLCFGR_PLLSRC_Msk));
  RCC->PLLCFGR |= (pllm << RCC_PLLCFGR_PLLM_Pos) | (plln << RCC_PLLCFGR_PLLN_Pos) | (0 << RCC_PLLCFGR_PLLP_Pos) | (pll_src << RCC_PLLCFGR_PLLSRC_Pos);

  /*  Turn on PLL */
  RCC->CR |= RCC_CR_PLLON_Msk;
  timeout = TIMEOUT;
  while (!((RCC->CR & RCC_CR_PLLRDY_Msk) && (--timeout)))
    ;

  /*  Select PLL as clock source and wait for it to be set  */
  RCC->CFGR |= RCC_CFGR_SW_PLL;
  if ((RCC->CFGR & RCC_CFGR_SWS_1) != (RCC_CFGR_SWS_1))
  {
    /*  Log: Not able to switch ot PLL */
  }

  ulog_info("Sysclk configured to be PLL");

  sys_clk = pll_freq;
}

uint32_t get_sys_clk(void){
  return sys_clk;
}
