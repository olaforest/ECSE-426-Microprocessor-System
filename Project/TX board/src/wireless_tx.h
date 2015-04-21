/*
ECSE 426 - Wireless 3D printing machine project
Maxim Goukhshtein (ID: 260429739)
Olivier Laforest  (ID: 260469066)
Nuri Ege Kozan    (ID: 260359680)
Genevieve Nantel  (ID: 260481768)
Group #4
Date:	April 14th, 2015
*/ 

#ifndef __WIRELESS_TX__
#define __WIRELESS_TX__

#include "cc2500.h"

/* Exported functions --------------------------------------------------------*/

static void delay(__IO uint32_t nCount);
void wireless_init(void);
void transmit(void);
void receive(void);
void send_data(uint8_t *data);

#endif
