#include "../settings.h"
#include <avr/io.h>
#include <util/delay.h>
#include "stk.h"
#include "stk_pic.h"
#include "../pic.h"
#include "../serial/serial.h"

void stk_pic_read_signature() {
	serialPrint(STK_INSYNC);
	serialPrint(PIC_SIGNATURE_HIGH);
	serialPrint(PIC_SIGNATURE_MIDDLE);
	serialPrint(PIC_SIGNATURE_LOW);
	serialPrint(STK_OK);
}

unsigned char stk_pic_universal(unsigned char a, unsigned char b, unsigned char c, unsigned char d) {
	if (a == 0xac) {
		if (b == 0x80) {
			pic_erase_program();
			pic_erase_data();
			return 0;
		}
	}
	
	return 0;
}

void stk_pic_start_pmode() {
	pic_reset();
}

void stk_pic_end_pmode() {
	pic_pin_clk_input();
	pic_pin_data_input();
//	pic_pin_mclr_input();
	pic_pin_mclr_0();
	pic_pin_mclr_output();
	pic_pin_power_input();
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
	pic_begin_programming();
}

unsigned char stk_pic_eeprom_read(unsigned int addr) {
	pic_go_to_data(addr);
	return pic_read_data();
}

void stk_pic_write_eeprom(unsigned int addr, unsigned char val) {
	pic_go_to_data(addr);
	pic_load_data(val);
	pic_begin_programming();
}

void stk_print_hex_uint16(unsigned int data) {
	serialPrintHexUInt16(data);
	serialPrintString(" ");
}

void stk_pic_service() {
	serialPrintString("Hello. Pic Service Mode ");
	
	while (1) {
		char ch = serialRead();
		if (ch == 'P') {
			stk_pic_start_pmode();
		} else if (ch == 'Q') {
			stk_pic_end_pmode();
		} else if (ch == 'E') {
			return;
		} else if (ch == ' ') {
			serialPrint(' ');
		} else if (ch == 'C') {
			pic_switch_to_config();
		} else if (ch == 'I') {
			pic_increment_address();
		} else if (ch == 'R') {
			unsigned int data = pic_read_program();
			stk_print_hex_uint16(data);
		} else if (ch == 'W') {
			unsigned int data = serialReadHexUInt16();
			stk_print_hex_uint16(data);
			pic_load_program(data);
			pic_begin_programming();
		} else if (ch == 'G') {
			unsigned int adr = serialReadHexUInt16();
			stk_pic_go_to(adr);
		} else if (ch == 'A') {
			unsigned char sp = pic_get_address_space();
			serialPrint(sp);
			unsigned int adr = pic_get_address();
			stk_print_hex_uint16(adr);
		}
	}
} 