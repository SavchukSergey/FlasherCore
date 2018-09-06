#include "settings.h"
#include <util/delay.h>
#include "io/io.c"
#include "avr.h"

unsigned char avr_spi_type; //'H' stays for Hard, 'S' stays for Soft
unsigned char avr_spi_delay;

void spi_init() {
	avr_io_miso_input();
	avr_io_mosi_output();
	avr_io_sck_output();
	avr_io_ss_output();
	avr_spi_type = 'H';
	avr_spi_delay = 2;

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


unsigned char spi_soft_send(unsigned char data) {
	unsigned char miso = 0;
	unsigned char mask = 0x80; // use 0x01 for Little Endian
	while (mask > 0) {
		if (data & mask) {
			avr_io_mosi_1();
		} else {
			avr_io_mosi_0();
		}
		delay_us(2);
		avr_io_sck_1();
		delay_us(2);
		if (avr_io_miso()) {
			miso |= mask;
		}
		avr_io_sck_0();
		mask >>= 1; // use <<= 1 for Little Endian
	}
	return miso;
}

void spi_wait() {
	while (!(SPSR & (1 << SPIF)));
}

unsigned char spi_hard_send(unsigned char b) {
	SPDR = b;
	spi_wait();
	return SPDR;
}

unsigned char spi_send(unsigned char b) {
	if (avr_spi_type == 'H') {
		return spi_hard_send(b);
	} else {
		return spi_soft_send(b);
	}
}	

unsigned char spi_transaction(unsigned char a, unsigned char b, unsigned char c, unsigned char d) {
	spi_send(a);
	spi_send(b);
	spi_send(c);
	return spi_send(d);
}

unsigned char spi_transaction2(unsigned char a, unsigned char b, unsigned char c, unsigned char d) {
	spi_send(a);
	spi_send(b);
	unsigned char ch = spi_send(c);
	spi_send(d);
	return ch;
}

void spi_set_type(unsigned char type) {
	avr_spi_type = type;
}

void spi_set_delay(unsigned char delay) {
	avr_spi_delay = delay;
}
