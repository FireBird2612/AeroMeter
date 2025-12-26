#ifndef __CONFIG_H__
#define  __CONFIG_H__

#include <stdint.h>

#define HSI_OSC 16000000U
//#define HSE_OSC

void ClockInit(void);
uint32_t get_sys_clk(void);

#endif
