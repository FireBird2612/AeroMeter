#include "stm32f4xx.h"
#include "config.h"
#include "ulogger.h"
#include "timer.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static log_buffer_t buffer_info, buffer_warn, buffer_error;
static uint32_t timestamp;

void ulogger_init(void)
{
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN_Msk;
  RCC->APB1ENR |= RCC_APB1ENR_USART2EN_Msk;

  GPIOA->MODER |= (GPIO_MODER_MODE2_1) | (GPIO_MODER_MODE3_1);
  GPIOA->AFR[0] |= (7 << GPIO_AFRL_AFSEL2_Pos) | (7 << GPIO_AFRL_AFSEL3_Pos);

  /* By default */
  float udiv, fck;
  uint32_t mantissa, frac;
  uint16_t brr;

  fck = APB1_MAX_FREQ;
  udiv = (float)(fck / (16 * ULOG_BAUDRATE));
  mantissa = (uint32_t)(udiv);
  frac = (udiv - mantissa) * 16;
  brr = (((mantissa & 0xFFF) << 4) | (frac & 0xF));

  USART2->BRR = brr;
  USART2->CR1 |= ((USART_CR1_UE_Msk) | (USART_CR1_TE_Msk) | (USART_CR1_RE_Msk) | (USART_CR1_RXNEIE_Msk));

  NVIC_SetPriority(USART2_IRQn, 2);
  NVIC_EnableIRQ(USART2_IRQn);

  ulog_info("USART2 instance initialized for logging with 115200 baudrate");
}

void ulogger_log(ulog_level_t log_level, const char *fmt){

  log_buffer_t *buffer = NULL;
  char tmp_buffer[LOG_MSG_SZ];
  uint32_t log_timestamp;

  log_timestamp = timestamp;
  timestamp = 0;

  /* Common: append the timestamp */
  snprintf(tmp_buffer, LOG_MSG_SZ, "[%ld]:", log_timestamp);

  /*  TODO: check the lengths of buffer when copying */
  switch (log_level)
  {
  case LOG_INFO:
    strcat(tmp_buffer, "[INFO]");
    buffer = &buffer_info;
    break;

  case LOG_WARN:
    strcat(tmp_buffer, "[WARN]");
    buffer = &buffer_warn;
    break;

  case LOG_ERROR:
    strcat(tmp_buffer, "[ERROR]");
    buffer = &buffer_error;
    break;

  default:
    break;
  }

  strcat(tmp_buffer, fmt);
  strcat(tmp_buffer, "\r\n");
  append_log(buffer, tmp_buffer);

}

void append_log(log_buffer_t *copy_to_buffer, char *copy_from_buffer){
  if(copy_to_buffer == NULL || copy_from_buffer == NULL){
    return;
  }
  
  if(copy_to_buffer->head == (LOG_CAPACITY - 1)){
    /*  TODO: Buffer capacity full  
        Track the read end of the log and if full flush the buffer.
        after flushing make head to zero
    */
    return;
  }
  strcpy((copy_to_buffer->buffer)[copy_to_buffer->head], copy_from_buffer);
  copy_to_buffer->head++;
}

void ulog_info(const char *fmt){
  if(fmt == NULL){
    return;
  }
  timestamp = get_timestamp();
  ulogger_log(LOG_INFO, fmt);
}

void ulog_warn(const char *fmt)
{
  if (fmt == NULL)
  {
    return;
  }
  timestamp = get_timestamp();
  ulogger_log(LOG_WARN, fmt);
}

void ulog_error(const char *fmt){
  if(fmt == NULL){
    return;
  }
  timestamp = get_timestamp();
  ulogger_log(LOG_ERROR, fmt);
}

void ulog_flush(void){
  if(buffer_info.tail < buffer_info.head){
    for (uint32_t i = buffer_info.tail; i < buffer_info.head; i++){
      usart2_log_string(buffer_info.buffer[i]);
      buffer_info.tail++;
    }
  }
}

void USART2_IRQHandler(void){
  /*  confirm the interrupt */
  if(USART2->SR & USART_SR_RXNE_Msk){

  }
}

/* Keep this for now */
void usart2_log_byte(char byte){
  while(!(USART2->SR & USART_SR_TXE))
    ;
  USART2->DR = byte;
}

void usart2_log_string(char *str){
  while(*str)
    usart2_log_byte(*str++);
  
  while(!(USART2->SR & USART_SR_TC))
    ;
}