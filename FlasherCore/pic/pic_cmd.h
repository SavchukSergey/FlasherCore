#ifndef PIC_COMMANDS_H_
#define PIC_COMMANDS_H_

#include "../settings.h"
#include <util/delay.h>


void pic_cmd_increment();
void pic_cmd_load_program(unsigned int data);
void pic_cmd_load_data(unsigned int data);
unsigned int pic_cmd_read_program();
unsigned char pic_cmd_read_data();
void pic_cmd_begin_erase_programming_cycle();
void pic_cmd_begin_programming_only_cycle();


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