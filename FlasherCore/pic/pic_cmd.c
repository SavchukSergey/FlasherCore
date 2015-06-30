#include "../settings.h"
#include <util/delay.h>
#include "../io/io.c"
#include "pic_cmd.h"

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
	pic_send_cmd(0x09);
}

void pic_cmd_bulk_erase_data_memory() {
	pic_send_cmd(0x0b);
}

void pic_cmd_bulk_erase_setup_1() {
	pic_send_cmd(0x01);
}

void pic_cmd_bulk_erase_setup_2() {
	pic_send_cmd(0x07);
}