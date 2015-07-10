#include "../settings.h"
#include <avr/io.h>
#include <util/delay.h>
#include "../io/io.c"
#include "../avr.h"
#include "stk.h"
#include "stk_avr.h"
#include "../serial/stream.h"


void stk_avr_read_signature() {
	streamWriteChar(STK_INSYNC);
	unsigned char high = spi_transaction(0x30, 0x00, 0x00, 0x00);
	streamWriteChar((unsigned char) high);
	unsigned char middle = spi_transaction(0x30, 0x00, 0x01, 0x00);
	streamWriteChar((unsigned char) middle);
	unsigned char low = spi_transaction(0x30, 0x00, 0x02, 0x00);
	streamWriteChar((unsigned char) low);
	streamWriteChar(STK_OK);
}

void stk_avr_erase() {
	spi_transaction(0xac, 0x80, 0x00, 0x00);
}

unsigned char stk_avr_universal(unsigned char a, unsigned char b, unsigned char c, unsigned char d) {
	return spi_transaction(a, b, c, d);
}

void stk_avr_start_pmode() {
	spi_init();
	// following delays may not work on all targets...
	avr_io_reset_output();
	avr_io_reset_1();
	_delay_ms(50);
	avr_io_reset_0();
	_delay_ms(50);
	spi_transaction(0xAC, 0x53, 0x00, 0x00);
}

void stk_avr_end_pmode() {
	spi_shutdown();
	avr_io_reset_input();
}

#define PTIME 30
void stk_avr_commit(unsigned int addr) {
	spi_transaction(0x4C, (addr >> 8) & 0xFF, addr & 0xFF, 0);
	_delay_ms(PTIME);
}

unsigned int stk_avr_flash_read(unsigned int addr) {
	unsigned int low = spi_transaction(0x20, (addr >> 8) & 0xFF, addr & 0xFF, 0);
	unsigned int high = spi_transaction(0x28, (addr >> 8) & 0xFF, addr & 0xFF, 0);
	return (high << 8) | low;
}

unsigned char stk_avr_eeprom_read(unsigned int addr) {
	return spi_transaction(0xA0, (addr >> 8) & 0xFF, addr & 0xFF, 0xFF);
}

void stk_avr_write_eeprom(unsigned int addr, unsigned char val) {
	spi_transaction(0xC0, (addr >> 8) & 0xFF, addr & 0xFF, val);
	_delay_ms(9);
}

void stk_avr_write_flash(unsigned int addr, unsigned int val) {
	spi_transaction(0x40, addr >> 8 & 0xFF, addr & 0xFF, (unsigned char)(val));
	spi_transaction(0x48, addr >> 8 & 0xFF, addr & 0xFF, (unsigned char)(val >> 8));
}

void stk_avr_service(unsigned char ch) {
	if (ch == 'U') {
		spi_init();
		unsigned char a = streamReadHexUInt8();
		unsigned char b = streamReadHexUInt8();
		unsigned char c = streamReadHexUInt8();
		unsigned char d = streamReadHexUInt8();
		unsigned char res = spi_transaction(a, b, c, d);
		streamPrintHexUInt8(res);
	}
}