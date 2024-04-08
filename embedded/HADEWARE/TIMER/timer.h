#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

extern int Sec,Waring_Time;//倒计时参数
extern unsigned int System_Flag,System_Time;
extern int flag;
extern uint16_t CH4_max,CH4_MAX;
extern uint16_t H2_max,H2_MAX;
extern uint16_t CO_max,CO_MAX;
extern uint16_t CO2_max,CO2_MAX;

	  


void TIM4_Init(void);
extern unsigned int Sys_Tick_Timer;
#endif
