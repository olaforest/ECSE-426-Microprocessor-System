#ifndef __WIRELESS_RX__
#define __WIRELESS_RX__

#include "cc2500.h"

// Definition for rates (in Hz), prescalar and period for the TIM3 clock.
#define TIM3_COUNTER_CLK 1282
#define TIM3_PRESCALAR ((SystemCoreClock / 2) / TIM3_COUNTER_CLK) - 1
#define TIM3_DESIRED_RATE 20
#define TIM3_PERIOD (TIM3_COUNTER_CLK / TIM3_DESIRED_RATE) - 1

void wireless_init(void);
void transmit(void);
void receive(void);

#endif
