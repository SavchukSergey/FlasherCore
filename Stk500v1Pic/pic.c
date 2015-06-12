#include "settings.h"
#include <util/delay.h>
#include "pic.h"

#define PIC_ADDRESS_SPACE_PROGRAM 'P'
#define PIC_ADDRESS_SPACE_CONFIG 'C'

unsigned int address;
unsigned char pic_address_space;

inline static void pic_wait_erase() {
	_delay_ms(6);
}

inline static void pic_wait_prog() {
	_delay_ms(8);
}

//Delay between clock? to clock? of next command or data
inline static void pic_wait_dly2() {
	asm("nop");
}

//Clock? to data out valid (during read data)
inline void pic_wait_dly3() {
	asm("nop");
}

inline void pic_delay_bit() {
	asm("nop");
}

inline void pic_send_bit(unsigned char val) {
	pic_pin_clk_1();
	if (val) {
		pic_pin_data_1();
	} else {
		pic_pin_data_0();
	}
	pic_delay_bit();
	pic_pin_clk_0();
	pic_delay_bit();
}

inline unsigned char pic_receive_bit() {
	pic_pin_clk_1();
	pic_wait_dly3();
	unsigned char val = pic_pin_data_value();
	pic_pin_clk_0();
	pic_delay_bit();
	return val;
}

static void pic_send_cmd (unsigned char cmd) {
	for (unsigned char i = 0; i < 6; i++) {
		pic_send_bit(cmd & 0x01);
		cmd = cmd >> 1;
	}
	pic_pin_data_0();
	pic_wait_dly2();
}

static void pic_send_data (unsigned int data) {
	data = data & 0x3fff;
	data = data << 1;
	for (unsigned char i = 0; i < 16; i++) {
		pic_send_bit(data & 0x01);
		data = data >> 1;
	}
	pic_pin_data_0();
	pic_wait_dly2();
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

unsigned int pic_universal_read(unsigned char cmd) {
	pic_send_cmd(cmd);
	return pic_receive_data();	
}

void pic_universal_write(unsigned char cmd, unsigned int data) {
	pic_send_cmd(cmd);
	pic_send_data(data);
}


void pic_load_program(unsigned int data) {
	pic_send_cmd(0x02);
	pic_send_data(data);
}

void pic_load_data(unsigned char data) {
	pic_send_cmd(0x03);
	pic_send_data(data);
	pic_wait_dly2();
}

void pic_load_config(unsigned int data) {
	pic_send_cmd(0x00);
	pic_send_data(data);
	pic_wait_dly2();
	pic_address_space = PIC_ADDRESS_SPACE_CONFIG;
}

void pic_begin_programming() {
	pic_send_cmd(0x08);
	pic_wait_prog();
}

void pic_increment_address() {
	pic_send_cmd(0x06);
	address++;
}

unsigned int pic_read_program() {
	pic_send_cmd(0x04);
	return pic_receive_data();
}

unsigned char pic_read_data() {
	pic_send_cmd(0x05);
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
	pic_address_space = PIC_ADDRESS_SPACE_PROGRAM;
}

void pic_go_to_program(unsigned int adr) {
	if (pic_address_space == PIC_ADDRESS_SPACE_CONFIG) {
		pic_reset();
	}
	while ((address & 0x7ff) != (adr & 0x7ff)) {
		pic_increment_address();
	}
}

void pic_go_to_data(unsigned int adr) {
	while ((address & 0x7f) != (adr & 0x7f)) {
		pic_increment_address();
	}
}

void pic_go_to_config(unsigned int addr) {
	if (pic_address_space != PIC_ADDRESS_SPACE_CONFIG) {
		pic_load_config(0x3ff);
	}
	while ((address & 0x7ff) != (addr & 0x7ff)) {
		pic_increment_address();
	}
}
