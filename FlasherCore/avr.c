#include "settings.h"
#include <util/delay.h>
#include "io/io.c"
#include "avr.h"

void spi_init() {
	avr_io_miso_input();
	avr_io_mosi_output();
	avr_io_sck_output();
	avr_io_ss_output();

	unsigned char x;
	SPCR = 0x53;
	x = SPSR;
	x = SPDR;
}

void spi_shutdown() {
	SPCR = 0x00;
	avr_io_miso_input();
	avr_io_mosi_input();
	avr_io_sck_input();
	avr_io_ss_input();
}

void spi_wait() {
	while (!(SPSR & (1 << SPIF)));
}

unsigned char spi_send(unsigned char b) {
	SPDR = b;
	spi_wait();
	return SPDR;
}

unsigned char spi_transaction(unsigned char a, unsigned char b, unsigned char c, unsigned char d) {
	spi_send(a);
	spi_send(b);
	//if (n != a) error = -1;
	spi_send(c);
	return spi_send(d);
}