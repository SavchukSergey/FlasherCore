#ifndef PIC_H_
#define PIC_H_

#include "settings.h"

void pic_load_program(unsigned int data);
void pic_load_data(unsigned char data);
void pic_load_config(unsigned int data);

void pic_begin_programming();

unsigned int pic_read_program();
unsigned char pic_read_data();

void pic_increment_address();

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


#define PIC_PIN_DATA_OR_MASK (1 << PIC_PIN_DATA_BIT)
#define PIC_PIN_DATA_AND_MASK (~PIC_PIN_DATA_OR_MASK)

#define PIC_PIN_CLK_OR_MASK (1 << PIC_PIN_CLK_PORT_BIT)
#define PIC_PIN_CLK_AND_MASK (~PIC_PIN_CLK_OR_MASK)

#define PIC_PIN_MCLR_OR_MASK (1 << PIC_PIN_MCLR_PORT_BIT)
#define PIC_PIN_MCLR_AND_MASK (~PIC_PIN_MCLR_OR_MASK)

#define PIC_PIN_POWER_OR_MASK (1 << PIC_PIN_POWER_PORT_BIT)
#define PIC_PIN_POWER_AND_MASK (~PIC_PIN_POWER_OR_MASK)




inline void pic_pin_data_0() {
	PIC_PIN_DATA_PORT &= PIC_PIN_DATA_AND_MASK;
}

inline void pic_pin_data_1() {
	PIC_PIN_DATA_PORT |= PIC_PIN_DATA_OR_MASK;
}

inline void pic_pin_data_output() {
	PIC_PIN_DATA_DDR |= PIC_PIN_DATA_OR_MASK;
}

inline void pic_pin_data_input() {
	PIC_PIN_DATA_DDR &= PIC_PIN_DATA_AND_MASK;
}

inline unsigned char pic_pin_data_value() {
	return (PIC_PIN_DATA_PIN & PIC_PIN_DATA_OR_MASK);
}


inline void pic_pin_clk_0() {
	PIC_PIN_CLK_PORT &= PIC_PIN_CLK_AND_MASK;
}

inline void pic_pin_clk_1() {
	PIC_PIN_CLK_PORT |= PIC_PIN_CLK_OR_MASK;
}

inline void pic_pin_clk_output() {
	PIC_PIN_CLK_DDR |= PIC_PIN_CLK_OR_MASK;
}

inline void pic_pin_clk_input() {
	PIC_PIN_CLK_DDR &= PIC_PIN_CLK_AND_MASK;
}


inline void pic_pin_mclr_0() {
	PIC_PIN_MCLR_PORT &= PIC_PIN_MCLR_AND_MASK;
}

inline void pic_pin_mclr_1() {
	PIC_PIN_MCLR_PORT |= PIC_PIN_MCLR_OR_MASK;
}

inline void pic_pin_mclr_output() {
	PIC_PIN_MCLR_DDR |= PIC_PIN_MCLR_OR_MASK;
}

inline void pic_pin_mclr_input() {
	PIC_PIN_MCLR_DDR &= PIC_PIN_MCLR_AND_MASK;
}


inline void pic_pin_power_0() {
	PIC_PIN_POWER_PORT &= PIC_PIN_POWER_AND_MASK;
}

inline void pic_pin_power_1() {
	PIC_PIN_POWER_PORT |= PIC_PIN_POWER_OR_MASK;
}

inline void pic_pin_power_output() {
	PIC_PIN_POWER_DDR |= PIC_PIN_POWER_OR_MASK;
}

inline void pic_pin_power_input() {
	PIC_PIN_POWER_DDR &= PIC_PIN_POWER_AND_MASK;
}
#endif