#ifndef __WIRELESS_TX__
#define __WIRELESS_TX__

#include "cc2500.h"

void wireless_init(void);
void transmit(void);
void receive(void);
void read_beacons(void);

#endif