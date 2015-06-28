#ifndef STK_PIC_H_
#define STK_PIC_H_

void stk_pic_setup();
void stk_pic_service(unsigned char ch);

void stk_pic_read_signature();
unsigned char stk_pic_universal(unsigned char a, unsigned char b, unsigned char c, unsigned char d);
void stk_pic_start_pmode();
void stk_pic_end_pmode();
void stk_pic_erase();

unsigned int stk_pic_flash_read(unsigned int addr);
unsigned char stk_pic_eeprom_read(unsigned int addr);

void stk_pic_write_flash(unsigned int addr, unsigned int val);
void stk_pic_write_eeprom(unsigned int addr, unsigned char val);

void stk_pic_commit(unsigned int addr);


#endif