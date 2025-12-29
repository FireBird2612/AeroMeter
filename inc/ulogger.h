#ifndef __ULOGGER_H__
#define __ULOGGER_H__

#include <stdint.h>
#include <stdbool.h>

#define ULOG_BAUDRATE   115200
#define LOG_CAPACITY  8
#define LOG_MSG_SZ    128


typedef enum {
  LOG_INFO,
  LOG_WARN,
  LOG_ERROR
} ulog_level_t;

typedef struct {
  char buffer[LOG_CAPACITY][LOG_MSG_SZ];
  uint32_t head;
  uint32_t tail;
} log_buffer_t;

void ulogger_init(void);

void ulogger_log(ulog_level_t log_level, const char *fmt);
void ulog_info(const char *fmt);
void ulog_warn(const char *fmt);
void ulog_error(const char *fmt);
void ulog_flush(void);

void append_log(log_buffer_t *copy_to_buffer, char *copy_from_buffer);

void usart2_log_byte(char byte);
void usart2_log_string(char *str);



#endif