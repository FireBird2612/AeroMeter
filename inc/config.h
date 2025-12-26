#ifndef __CONFIG_H__
#define  __CONFIG_H__

#include <stdint.h>

#define APB1_MAX_FREQ 45000000

void ClockInit(void);
uint32_t get_sys_clk(void);
void monotonic_clk_init(void);

#endif
