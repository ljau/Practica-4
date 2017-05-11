#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef RS485_DRIVER_H
#define RS485_DRIVER_H
#define RX_232        PIN_C7
#define TX_232        PIN_C6
#use RS232(BAUD=9600, XMIT=TX_232, RCV=RX_232, BITS=8,PARITY=N, STOP=1)
#endif
