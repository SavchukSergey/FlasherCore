#include "../settings.h"
#include <avr/io.h>
#include <util/delay.h>
#include "stk.h"
#include "stk_pic.h"
#include "../io/io.c"
#include "../pic.h"
#include "../pic/pic_cmd.h"
#include "../serial/stream.h"

void stk_pic_setup() {
	pic_setup();
}

void stk_pic_read_signature() {
	streamWriteChar(STK_INSYNC);
	streamWriteChar(PIC_SIGNATURE_HIGH);
	streamWriteChar(PIC_SIGNATURE_MIDDLE);
	streamWriteChar(PIC_SIGNATURE_LOW);
	streamWriteChar(STK_OK);
}

void stk_pic_erase() {
	pic_erase();
}

unsigned char stk_pic_universal(unsigned char a, unsigned char b, unsigned char c, unsigned char d) {
	if (a == 0xac) {
		if (b == 0x80) {
			stk_pic_erase();
			return 0;
		}
	}
	
	return 0;
}

void stk_pic_start_pmode() {
	stk_io_power_on();
	_delay_ms(100);
	pic_reset();
}

void stk_pic_end_pmode() {
	pic_io_clk_input();
	pic_io_data_input();
	pic_io_mclr_off();
	pic_io_power_off();
}

void stk_pic_commit(unsigned int addr) {
}

static void stk_pic_go_to(unsigned int addr) {
	if (addr >= 0x2000) {
		pic_go_to_config(addr);
	} else {
		pic_go_to_program(addr);
	}
}

unsigned int stk_pic_flash_read(unsigned int addr) {
	stk_pic_go_to(addr);
	return pic_read_program();
}

void stk_pic_write_flash(unsigned int addr, unsigned int val) {
	stk_pic_go_to(addr);
	pic_load_program(val);
	pic_cmd_begin_programming_only_cycle();
}

unsigned char stk_pic_eeprom_read(unsigned int addr) {
	pic_go_to_data(addr);
	return pic_read_data();
}

void stk_pic_write_eeprom(unsigned int addr, unsigned char val) {
	pic_go_to_data(addr);
	pic_load_data(val);
	pic_begin_programming_only_cycle();
}

void stk_print_hex_uint16(unsigned int data) {
	streamPrintHexUInt16(data);
	streamPrintString(" ");
}

void stk_pic_service(unsigned char ch) {
	if (ch == 'p') {
		unsigned char powerPin = streamReadChar();
		if (powerPin == '0') {
			pic_io_power_off();
		} else if (powerPin == '1') {
			pic_io_power_on();
		}
	} else if (ch == 'm') {
		unsigned char mclrPin = streamReadChar();
		if (mclrPin == '0') {
			pic_io_mclr_off();
		} else if (mclrPin == '1') {
			pic_io_mclr_on();
		}
	} else if (ch == 'C') {
		pic_switch_to_config();
	} else if (ch == 'I') {
		pic_increment_address();
	} else if (ch == 'R') {
		unsigned int data = pic_read_program();
		stk_print_hex_uint16(data);
	} else if (ch == 'W') {
		unsigned int data = streamReadHexUInt16();
		pic_load_program(data);
		pic_cmd_begin_programming_only_cycle();
	} else if (ch == 'G') {
		unsigned int adr = streamReadHexUInt16();
		stk_pic_go_to(adr);
	} else if (ch == 'A') {
		unsigned char sp = pic_get_address_space();
		streamWriteChar(sp);
		unsigned int adr = pic_get_address();
		stk_print_hex_uint16(adr);
	} else if (ch == 'c') {
		unsigned char cmd = streamReadHexUInt8();
		pic_send_cmd(cmd);
	} else if (ch == 'd') {
		unsigned char cmdData = streamReadHexUInt16();
		pic_send_data(cmdData);
	} else if (ch == 'r') {
		unsigned char cmdData = pic_receive_data();
		stk_print_hex_uint16(cmdData);
	}
}