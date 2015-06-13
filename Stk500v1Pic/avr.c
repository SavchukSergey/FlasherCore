#include "settings.h"
#include <util/delay.h>
#include "avr.h"

void spi_init() {
	avr_pin_ss_output();
	
	unsigned char x;
	SPCR = 0x53;
	x = SPSR;
	x = SPDR;
}

void spi_wait() {
	while (!(SPSR & (1 << SPIF)));
}

unsigned char spi_send(unsigned char b) {
	unsigned char reply;
	SPDR = b;
	spi_wait();
	reply = SPDR;
	return reply;
}

unsigned char spi_transaction(unsigned char a, unsigned char b, unsigned char c, unsigned char d) {
	spi_send(a);
	spi_send(b);
	//if (n != a) error = -1;
	spi_send(c);
	return spi_send(d);
}