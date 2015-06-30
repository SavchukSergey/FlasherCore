#include "settings.h"
#include <util/delay.h>
#include "io/io.c"
#include "pic.h"
#include "pic/pic_cmd.h"

#define PIC_ADDRESS_SPACE_PROGRAM 'P'
#define PIC_ADDRESS_SPACE_CONFIG 'C'

unsigned int address;
unsigned char pic_address_space;






void pic_load_program(unsigned int data) {
	pic_cmd_load_program(data);
}

void pic_load_data(unsigned int data) {
	pic_cmd_load_data(data);
}

void pic_load_config(unsigned int data) {
	pic_cmd_load_config(data);
	pic_address_space = PIC_ADDRESS_SPACE_CONFIG;
}

void pic_begin_programming() {
	pic_cmd_begin_erase_programming_cycle();
}

void pic_increment_address() {
	pic_cmd_increment_address();
	address++;
}

unsigned int pic_read_program() {
	return pic_cmd_read_program();
}

unsigned char pic_read_data() {
	return (unsigned char)pic_cmd_read_data();
}

void pic_erase_program() {
	if (pic_address_space != PIC_ADDRESS_SPACE_PROGRAM) {
		pic_reset();
	}
	pic_cmd_bulk_erase_program_memory();
}

void pic_erase_data() {
	pic_cmd_bulk_erase_data_memory();
}

void pic_setup() {
	pic_io_mclr_output();
	pic_io_mclr_off();

	pic_io_power_output();
	pic_io_power_off();

	pic_io_clk_input();
	pic_io_data_input();
	pic_io_pgm_input();
}

inline static void pic_enter_high_voltage_program_mode() {
	pic_setup();
	pic_io_clk_output();
	pic_io_clk_0();
	pic_io_data_output();
	pic_io_data_0();
	pic_io_power_output();
	pic_io_power_off();
	pic_io_mclr_output();
	pic_io_mclr_off();
	pic_io_pgm_output();
	pic_io_pgm_0();

	_delay_ms(50);
	pic_io_mclr_on();
	pic_wait_ppdp();
	pic_io_power_on();
	pic_wait_hld0();
}

inline static void pic_enter_low_voltage_program_mode() {
	pic_setup();
	pic_io_clk_output();
	pic_io_clk_0();
	pic_io_data_output();
	pic_io_data_0();
	pic_io_power_output();
	pic_io_power_off();
	pic_io_mclr_output();
	pic_io_mclr_off();
	pic_io_pgm_output();
	pic_io_pgm_0();

	_delay_ms(50);
	pic_io_power_on();
	pic_wait_ppdp();
	pic_io_mclr_on();
	pic_io_pgm_1();
	pic_wait_hld0();
}

void pic_reset() {
	pic_setup();
	pic_io_clk_output();
	pic_io_data_output();

	_delay_ms(500);
	pic_io_mclr_on();
	_delay_us(100);
	pic_io_power_on();
	
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


void pic_switch_to_config() {
	if (pic_address_space != PIC_ADDRESS_SPACE_CONFIG) {
		pic_load_config(0x3ff);
		address = 0;
	}
}

void pic_go_to_config(unsigned int addr) {
	if (pic_address_space != PIC_ADDRESS_SPACE_CONFIG) {
		pic_switch_to_config();
	}
	while ((address & 0x7ff) != (addr & 0x7ff)) {
		pic_increment_address();
	}
}

unsigned int pic_get_address() {
	return address;
}

unsigned char pic_get_address_space() {
	return pic_address_space;
}