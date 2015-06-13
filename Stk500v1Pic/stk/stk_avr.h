#ifndef STK_AVR_H_
#define STK_AVR_H_

#include "../settings.h"

void stk_avr_service();

void stk_avr_read_signature();
unsigned char stk_avr_universal(unsigned char a, unsigned char b, unsigned char c, unsigned char d);
void stk_avr_start_pmode();
void stk_avr_end_pmode();

unsigned int stk_avr_flash_read(unsigned int addr);
unsigned char stk_avr_eeprom_read(unsigned int addr);

void stk_avr_write_flash(unsigned int addr, unsigned int val);
void stk_avr_write_eeprom(unsigned int addr, unsigned char val);

void stk_avr_commit(unsigned int addr);


#endif