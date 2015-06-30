#ifndef PIC_H_
#define PIC_H_

#include "settings.h"

void pic_load_program(unsigned int data);
void pic_load_data(unsigned int data);
void pic_load_config(unsigned int data);

void pic_begin_erase_programming_cycle();
void pic_begin_programming_only_cycle();

unsigned int pic_read_program();
unsigned char pic_read_data();

void pic_increment_address();

void pic_erase();
void pic_erase_program();
void pic_erase_data();

void pic_setup();
void pic_reset();
void pic_switch_to_config();

void pic_go_to_program(unsigned int adr);
void pic_go_to_data(unsigned int adr);
void pic_go_to_config(unsigned int adr);

unsigned int pic_get_address();
unsigned char pic_get_address_space();

#endif