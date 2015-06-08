#include "settings.h"
#include <avr/io.h>
#include <util/delay.h>
#include "stk.h"
#include "stk_pic.h"
#include "pic.h"
#include "serial.h"

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
	pic_pin_mclr_input();
	pic_pin_power_input();
}

void stk_pic_commit(unsigned int addr) {
}

unsigned int stk_pic_flash_read(unsigned int addr) {
	pic_go_to_program(addr);
	return pic_read_program();
}

unsigned char stk_pic_eeprom_read(unsigned int addr) {
	pic_go_to_data(addr);
	return pic_read_data();
}

void stk_pic_write_eeprom(unsigned int addr, unsigned char val) {
	pic_go_to_data(addr);
	pic_load_data(val);
	pic_begin_programming_data();
}

void stk_pic_write_flash(unsigned int addr, unsigned int val) {
	pic_go_to_program(addr);
	pic_load_program(val);
	pic_begin_programming_program();
}