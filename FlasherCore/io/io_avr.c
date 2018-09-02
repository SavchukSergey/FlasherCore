#include "../settings.h"

#define AVR_IO_RESET_OR_MASK (1 << AVR_IO_RESET_BIT)
#define AVR_IO_RESET_AND_MASK (~AVR_IO_RESET_OR_MASK)

#define AVR_IO_SCK_OR_MASK (1 << AVR_IO_SCK_BIT)
#define AVR_IO_SCK_AND_MASK (~AVR_IO_SCK_OR_MASK)

#define AVR_IO_MOSI_OR_MASK (1 << AVR_IO_MOSI_BIT)
#define AVR_IO_MOSI_AND_MASK (~AVR_IO_MOSI_OR_MASK)

#define AVR_IO_MISO_OR_MASK (1 << AVR_IO_MISO_BIT)
#define AVR_IO_MISO_AND_MASK (~AVR_IO_MISO_OR_MASK)

#define AVR_IO_SS_OR_MASK (1 << AVR_IO_SS_BIT)
#define AVR_IO_SS_AND_MASK (~AVR_IO_SS_OR_MASK)


inline void avr_io_reset_0() {
	AVR_IO_RESET_PORT &= AVR_IO_RESET_AND_MASK;
}

inline void avr_io_reset_1() {
	AVR_IO_RESET_PORT |= AVR_IO_RESET_OR_MASK;
}

inline void avr_io_reset_output() {
	AVR_IO_RESET_DDR |= AVR_IO_RESET_OR_MASK;
}

inline void avr_io_reset_input() {
	AVR_IO_RESET_DDR &= AVR_IO_RESET_AND_MASK;
}



inline void avr_io_sck_0() {
	AVR_IO_SCK_PORT &= AVR_IO_SCK_AND_MASK;
}

inline void avr_io_sck_1() {
	AVR_IO_SCK_PORT |= AVR_IO_SCK_OR_MASK;
}

inline void avr_io_sck_output() {
	AVR_IO_SCK_DDR |= AVR_IO_SCK_OR_MASK;
}

inline void avr_io_sck_input() {
	AVR_IO_SCK_DDR &= AVR_IO_SCK_AND_MASK;
}


inline void avr_io_mosi_0() {
	AVR_IO_MOSI_PORT &= AVR_IO_MOSI_AND_MASK;
}

inline void avr_io_mosi_1() {
	AVR_IO_MOSI_PORT |= AVR_IO_MOSI_OR_MASK;
}

inline void avr_io_mosi_output() {
	AVR_IO_MOSI_DDR |= AVR_IO_MOSI_OR_MASK;
}

inline void avr_io_mosi_input() {
	AVR_IO_MOSI_DDR &= AVR_IO_MOSI_AND_MASK;
}


inline void avr_io_miso_0() {
	AVR_IO_MISO_PORT &= AVR_IO_MISO_AND_MASK;
}

inline void avr_io_miso_1() {
	AVR_IO_MISO_PORT |= AVR_IO_MISO_OR_MASK;
}

inline unsigned char avr_io_miso() {
	return (AVR_IO_MISO_PORT & AVR_IO_MISO_OR_MASK) ? 0x01 : 0x00;
}

inline void avr_io_miso_output() {
	AVR_IO_MISO_DDR |= AVR_IO_MISO_OR_MASK;
}

inline void avr_io_miso_input() {
	AVR_IO_MISO_DDR &= AVR_IO_MISO_AND_MASK;
}


inline void avr_io_ss_0() {
	AVR_IO_SS_PORT &= AVR_IO_SS_AND_MASK;
}

inline void avr_io_ss_1() {
	AVR_IO_SS_PORT |= AVR_IO_SS_OR_MASK;
}

inline void avr_io_ss_output() {
	AVR_IO_SS_DDR |= AVR_IO_SS_OR_MASK;
}

inline void avr_io_ss_input() {
	AVR_IO_SS_DDR &= AVR_IO_SS_AND_MASK;
}
