#ifndef AVR_H_
#define AVR_H_

#include "settings.h"

void spi_init();
void spi_wait();
unsigned char spi_send(unsigned char b) ;
unsigned char spi_transaction(unsigned char a, unsigned char b, unsigned char c, unsigned char d);


#define AVR_PIN_RESET_OR_MASK (1 << AVR_PIN_RESET_BIT)
#define AVR_PIN_RESET_AND_MASK (~AVR_PIN_RESET_OR_MASK)

#define AVR_PIN_SCK_OR_MASK (1 << AVR_PIN_SCK_BIT)
#define AVR_PIN_SCK_AND_MASK (~AVR_PIN_SCK_OR_MASK)

#define AVR_PIN_MOSI_OR_MASK (1 << AVR_PIN_MOSI_BIT)
#define AVR_PIN_MOSI_AND_MASK (~AVR_PIN_MOSI_OR_MASK)

#define AVR_PIN_MISO_OR_MASK (1 << AVR_PIN_MISO_BIT)
#define AVR_PIN_MISO_AND_MASK (~AVR_PIN_MISO_OR_MASK)


inline void avr_pin_reset_0() {
	AVR_PIN_RESET_PORT &= AVR_PIN_RESET_AND_MASK;
}

inline void avr_pin_reset_1() {
	AVR_PIN_RESET_PORT |= AVR_PIN_RESET_OR_MASK;
}

inline void avr_pin_reset_output() {
	AVR_PIN_RESET_DDR |= AVR_PIN_RESET_OR_MASK;
}

inline void avr_pin_reset_input() {
	AVR_PIN_RESET_DDR &= AVR_PIN_RESET_AND_MASK;
}



inline void avr_pin_sck_0() {
	AVR_PIN_SCK_PORT &= AVR_PIN_SCK_AND_MASK;
}

inline void avr_pin_sck_1() {
	AVR_PIN_SCK_PORT |= AVR_PIN_SCK_OR_MASK;
}

inline void avr_pin_sck_output() {
	AVR_PIN_SCK_DDR |= AVR_PIN_SCK_OR_MASK;
}

inline void avr_pin_sck_input() {
	AVR_PIN_SCK_DDR &= AVR_PIN_SCK_AND_MASK;
}


inline void avr_pin_mosi_0() {
	AVR_PIN_MOSI_PORT &= AVR_PIN_MOSI_AND_MASK;
}

inline void avr_pin_mosi_1() {
	AVR_PIN_MOSI_PORT |= AVR_PIN_MOSI_OR_MASK;
}

inline void avr_pin_mosi_output() {
	AVR_PIN_MOSI_DDR |= AVR_PIN_MOSI_OR_MASK;
}

inline void avr_pin_mosi_input() {
	AVR_PIN_MOSI_DDR &= AVR_PIN_MOSI_AND_MASK;
}


inline void avr_pin_miso_0() {
	AVR_PIN_MISO_PORT &= AVR_PIN_MISO_AND_MASK;
}

inline void avr_pin_miso_1() {
	AVR_PIN_MISO_PORT |= AVR_PIN_MISO_OR_MASK;
}

inline void avr_pin_miso_output() {
	AVR_PIN_MISO_DDR |= AVR_PIN_MISO_OR_MASK;
}

inline void avr_pin_miso_input() {
	AVR_PIN_MISO_DDR &= AVR_PIN_MISO_AND_MASK;
}

#endif