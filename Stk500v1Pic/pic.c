#include "settings.h"
#include <util/delay.h>
#include "pic.h"

#define PIC_ADDRESS_SPACE_PROGRAM = 'P'
#define PIC_ADDRESS_SPACE_DATA = 'D'
#define PIC_ADDRESS_SPACE_CONFIG = 'C'

unsigned int address = 0;
unsigned char address_space;

inline static void pic_wait_erase() {
	_delay_ms(6);
}

inline static void pic_wait_prog() {
	_delay_ms(4);
}

inline static void pic_wait_dprog() {
	_delay_ms(4);
}

inline static void pic_wait_dly2() {
	_delay_us(1);
}

inline static void pic_send_bit(unsigned char val) {
	_delay_us(1);
	pic_pin_clk_1();
	if (val) {
		pic_pin_data_1();
	} else {
		pic_pin_data_0();
	}
	_delay_us(1);
	pic_pin_clk_0();
}

inline static unsigned char pic_receive_bit() {
	pic_pin_clk_1();
	pic_wait_dly2();
	pic_pin_clk_0();
	pic_wait_dly2();
	return pic_pin_data_value();
}

static void pic_send_cmd (unsigned char cmd) {
	for (unsigned char i = 0; i < 6; i++) {
		pic_send_bit(cmd & 0x01);
		cmd = cmd >> 1;
	}
	pic_pin_data_0();
}

static void pic_send_data (unsigned int data) {
	data = data & 0x3fff;
	data = data << 1;
	for (unsigned char i = 0; i < 16; i++) {
		pic_send_bit(data & 0x01);
		data = data >> 1;
	}
	pic_pin_data_0();
}

static unsigned int pic_receive_data () {
	unsigned int data = 0;
	pic_pin_data_input();
	pic_pin_data_1(); //turn on pull-up

	for (unsigned char i = 0; i < 16; i++) {
		data = data >> 1;
		if (pic_receive_bit()) {
			data |= 0x8000;
		} else {
			data &= 0x7fff;
		}
	}
	data = data >> 1;
	data = data & 0x3fff;
	pic_pin_data_output();
	return data;
}

void pic_load_program(unsigned int data) {
	pic_send_cmd(0x02);
	pic_wait_dly2();
	pic_send_data(data);
	_delay_us(10);
}

void pic_load_data(unsigned char data) {
	pic_send_cmd(0x03);
	pic_wait_dly2();
	pic_send_data(data);
	_delay_us(10);
}

void pic_load_config(unsigned char data) {
	pic_send_cmd(0x00);
	pic_wait_dly2();
	pic_send_data(data);
	_delay_us(10);
}

void pic_begin_programming_program() {
	pic_send_cmd(0x08);
	pic_wait_prog();
}

void pic_begin_programming_data() {
	pic_send_cmd(0x08);
	pic_wait_dprog();
}

void pic_increment_address() {
	pic_send_cmd(0x06);
	pic_wait_dly2();
	address++;
}

unsigned int pic_read_program() {
	pic_send_cmd(0x04);
	pic_wait_dly2();
	return pic_receive_data();
}

unsigned char pic_read_data() {
	pic_send_cmd(0x05);
	pic_wait_dly2();
	return (unsigned char)pic_receive_data();
}

void pic_erase_program() {
	pic_send_cmd(0x09);
	pic_wait_erase();
}

void pic_erase_data() {
	pic_send_cmd(0x0b);
	pic_wait_erase();
}

void pic_reset() {
	pic_pin_clk_output();
	pic_pin_data_output();
	pic_pin_mclr_output();
	pic_pin_power_output();

	pic_pin_power_1();
	_delay_ms(500);
	pic_pin_mclr_1();
	_delay_us(100);
	pic_pin_power_0();
	address = 0;
}

void pic_go_to_program(unsigned int adr) {
	while ((address & 0x7ff) != (adr & 0x7ff)) {
		pic_increment_address();
	}
}

void pic_go_to_data(unsigned int adr) {
	while ((address & 0x7f) != (adr & 0x7f)) {
		pic_increment_address();
	}
}

void pic_go_to_config(unsigned int adr) {
	while ((address & 0x0f) != (adr & 0x0f)) {
		pic_increment_address();
	}
}
