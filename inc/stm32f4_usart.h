#ifndef __STM32F4_USART_H__
#define __STM32F4_USART_H__

void usart2_log_init(void);
void usart2_log_byte(const char byte);
void usart2_log_string(char *str);

#endif