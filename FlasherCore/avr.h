#ifndef AVR_H_
#define AVR_H_

#include "settings.h"

void spi_init();
void spi_shutdown();
void spi_wait();
unsigned char spi_send(unsigned char b) ;
unsigned char spi_transaction(unsigned char a, unsigned char b, unsigned char c, unsigned char d);

#endif