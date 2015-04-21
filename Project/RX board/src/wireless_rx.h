/*
ECSE 426 - Wireless 3D printing machine project
Maxim Goukhshtein (ID: 260429739)
Olivier Laforest  (ID: 260469066)
Nuri Ege Kozan    (ID: 260359680)
Genevieve Nantel  (ID: 260481768)
Group #4
Date:	April 14th, 2015
*/ 
#ifndef __WIRELESS_RX__
#define __WIRELESS_RX__

#include "cc2500.h"

// Definition for rates (in Hz), prescalar and period for the TIM3 clock.
#define TIM3_COUNTER_CLK 1282
#define TIM3_PRESCALAR ((SystemCoreClock / 2) / TIM3_COUNTER_CLK) - 1
#define TIM3_DESIRED_RATE 20
#define TIM3_PERIOD (TIM3_COUNTER_CLK / TIM3_DESIRED_RATE) - 1

/* Exported functions --------------------------------------------------------*/
void wireless_init(void);
void transmit(void);
void receive(void);

#endif
