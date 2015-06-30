#include "../settings.h"
#include <util/delay.h>
#include "../io/io.c"
#include "pic_cmd.h"


//Delay between clock? to clock? of next command or data
inline static void pic_wait_dly2() {
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
}

//Clock? to data out valid (during read data)
inline void pic_wait_dly3() {
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
}



inline void pic_delay_bit() {
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
}
inline void pic_send_bit(unsigned char val) {
	pic_io_clk_1();
	if (val) {
		pic_io_data_1();
	} else {
		pic_io_data_0();
	}
	pic_delay_bit();
	pic_io_clk_0();
	pic_delay_bit();
}

inline unsigned char pic_receive_bit() {
	pic_io_clk_1();
	pic_wait_dly3();
	unsigned char val = pic_io_data_value();
	pic_io_clk_0();
	pic_delay_bit();
	return val;
}

static void pic_send_cmd (unsigned char cmd) {
	for (unsigned char i = 0; i < 6; i++) {
		pic_send_bit(cmd & 0x01);
		cmd = cmd >> 1;
	}
	pic_io_data_0();
	pic_wait_dly2();
}

static void pic_send_data (unsigned int data) {
	data = data & 0x3fff;
	data = data << 1;
	for (unsigned char i = 0; i < 16; i++) {
		pic_send_bit(data & 0x01);
		data = data >> 1;
	}
	pic_io_data_0();
	pic_wait_dly2();
}

static unsigned int pic_receive_data () {
	unsigned int data = 0;
	pic_io_data_input();
	pic_io_data_0(); //turn off pull-up

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
	pic_io_data_output();
	return data;
}

void pic_cmd_load_config(unsigned int data) {
	pic_send_cmd(0x00);
	pic_send_data(data);
}

void pic_cmd_load_program(unsigned int data) {
	pic_send_cmd(0x02);
	pic_send_data(data);
}

void pic_cmd_load_data(unsigned int data) {
	pic_send_cmd(0x03);
	pic_send_data(data);
}

void pic_cmd_increment_address() {
	pic_send_cmd(0x06);
}

unsigned int pic_cmd_read_program() {
	pic_send_cmd(0x04);
	return pic_receive_data();
}

unsigned char pic_cmd_read_data() {
	pic_send_cmd(0x05);
	return (unsigned char)pic_receive_data();
}

void pic_cmd_begin_erase_programming_cycle() {
	pic_send_cmd(0x08);
	pic_wait_prog();
	pic_wait_erase();
}

void pic_cmd_begin_programming_only_cycle() {
	pic_send_cmd(0x18);
	pic_wait_prog();
}

void pic_cmd_bulk_erase_program_memory() {
	pic_load_program(0x3fff);
	pic_send_cmd(0x09);
	pic_begin_programming();
	pic_wait_erase();
}

void pic_cmd_bulk_erase_data_memory() {
	pic_load_data(0x3fff);
	pic_send_cmd(0x0b);
	pic_begin_programming();
	pic_wait_erase();
}

void pic_cmd_bulk_erase_setup_1() {
	pic_send_cmd(0x01);
}

void pic_cmd_bulk_erase_setup_2() {
	pic_send_cmd(0x07);
}