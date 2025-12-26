#include "stm32f4xx.h"
#include "config.h"
#include <stdint.h>

void usart2_log_init(uint32_t baudrate)
{
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN_Msk;
  RCC->APB1ENR |= RCC_APB1ENR_USART2EN_Msk;

  GPIOA->MODER |= (GPIO_MODER_MODE2_1) | (GPIO_MODER_MODE3_1);
  GPIOA->AFR[0] |= (7 << GPIO_AFRL_AFSEL2_Pos) | (7 << GPIO_AFRL_AFSEL3_Pos);

  /* Make sure usart is disable */
  USART1->CR1 &= ~(USART_CR1_UE);

  /* By default */
  float udiv;
  uint32_t mantissa, frac;
  uint16_t brr;
  udiv = APB1_MAX_FREQ / (16 * baudrate);
  mantissa = (uint32_t)(udiv);
  frac = (udiv - mantissa) * 16;
  brr = (((mantissa & 0xFFF) << 4) | (frac & 0xF));

  USART2->BRR = brr;
  USART2->CR1 |= (USART_CR1_TE_Msk | USART_CR1_RE | USART_CR1_RXNEIE);

  NVIC_SetPriority(USART2_IRQn, 2);
  NVIC_EnableIRQ(USART2_IRQn);
}

void usart2_log_byte(const char byte){  
  while((USART2->SR & USART_SR_TXE_Msk))
    ;
  USART2->DR = byte;
  while(!(USART2->SR & USART_SR_TC_Msk))
    ;
}

void usart2_log_string(char *str){
  while(*str){
    usart2_log_byte(*str++);
  }
}