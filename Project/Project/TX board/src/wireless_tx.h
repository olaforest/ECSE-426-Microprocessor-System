#ifndef __WIRELESS_TX__
#define __WIRELESS_TX__

#include "cc2500.h"

static void delay(__IO uint32_t nCount);
void wireless_init(void);
void transmit(void);
void receive(void);
void send_data(uint8_t *data);

#endif
