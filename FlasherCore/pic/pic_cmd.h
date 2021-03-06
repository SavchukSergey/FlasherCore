#ifndef PIC_COMMANDS_H_
#define PIC_COMMANDS_H_

#include "../settings.h"
#include <util/delay.h>

void pic_send_cmd(unsigned char cmd);
void pic_send_data(unsigned int data);
unsigned int pic_receive_data();

void pic_cmd_increment_address();
void pic_cmd_load_program(unsigned int data);
void pic_cmd_load_data(unsigned int data);
void pic_cmd_load_config(unsigned int data);
unsigned int pic_cmd_read_program();
unsigned char pic_cmd_read_data();
void pic_cmd_begin_programming_only_cycle();
void pic_cmd_bulk_erase_program_memory();
void pic_cmd_bulk_erase_data_memory();

inline static void pic_wait_erase() {
	_delay_ms(6);
}

inline static void pic_wait_prog() {
	_delay_ms(8);
}


//delay between rising Vpp and Vdd
inline static void pic_wait_ppdp() {
	_delay_us(5);
}

inline static void pic_wait_hld0() {
	_delay_us(5);
}


#endif