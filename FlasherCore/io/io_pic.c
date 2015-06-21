#include "../settings.h"

#define PIC_IO_DATA_OR_MASK (1 << PIC_IO_DATA_BIT)
#define PIC_IO_DATA_AND_MASK (~PIC_IO_DATA_OR_MASK)

#define PIC_IO_CLK_OR_MASK (1 << PIC_IO_CLK_PORT_BIT)
#define PIC_IO_CLK_AND_MASK (~PIC_IO_CLK_OR_MASK)

#define PIC_IO_MCLR_OR_MASK (1 << PIC_IO_MCLR_PORT_BIT)
#define PIC_IO_MCLR_AND_MASK (~PIC_IO_MCLR_OR_MASK)

#define PIC_IO_POWER_OR_MASK (1 << PIC_IO_POWER_PORT_BIT)
#define PIC_IO_POWER_AND_MASK (~PIC_IO_POWER_OR_MASK)

inline void pic_io_data_0() {
	PIC_IO_DATA_PORT &= PIC_IO_DATA_AND_MASK;
}

inline void pic_io_data_1() {
	PIC_IO_DATA_PORT |= PIC_IO_DATA_OR_MASK;
}

inline void pic_io_data_output() {
	PIC_IO_DATA_DDR |= PIC_IO_DATA_OR_MASK;
}

inline void pic_io_data_input() {
	PIC_IO_DATA_DDR &= PIC_IO_DATA_AND_MASK;
}

inline unsigned char pic_io_data_value() {
	return (PIC_IO_DATA_PIN & PIC_IO_DATA_OR_MASK);
}


inline void pic_io_clk_0() {
	PIC_IO_CLK_PORT &= PIC_IO_CLK_AND_MASK;
}

inline void pic_io_clk_1() {
	PIC_IO_CLK_PORT |= PIC_IO_CLK_OR_MASK;
}

inline void pic_io_clk_output() {
	PIC_IO_CLK_DDR |= PIC_IO_CLK_OR_MASK;
}

inline void pic_io_clk_input() {
	PIC_IO_CLK_DDR &= PIC_IO_CLK_AND_MASK;
}

inline void _pic_io_mclr_0() {
	PIC_IO_MCLR_PORT &= PIC_IO_MCLR_AND_MASK;
}

inline void _pic_io_mclr_1() {
	PIC_IO_MCLR_PORT |= PIC_IO_MCLR_OR_MASK;
}

inline void pic_io_mclr_output() {
	PIC_IO_MCLR_DDR |= PIC_IO_MCLR_OR_MASK;
}

inline void pic_io_mclr_input() {
	PIC_IO_MCLR_DDR &= PIC_IO_MCLR_AND_MASK;
}


inline void _pic_io_power_0() {
	PIC_IO_POWER_PORT &= PIC_IO_POWER_AND_MASK;
}

inline void _pic_io_power_1() {
	PIC_IO_POWER_PORT |= PIC_IO_POWER_OR_MASK;
}

inline void pic_io_power_output() {
	PIC_IO_POWER_DDR |= PIC_IO_POWER_OR_MASK;
}

inline void pic_io_power_input() {
	PIC_IO_POWER_DDR &= PIC_IO_POWER_AND_MASK;
}




inline void pic_io_mclr_on() {
	_pic_io_mclr_1();
}

inline void pic_io_mclr_off() {
	_pic_io_mclr_0();
}

inline void pic_io_power_on() {
	_pic_io_power_0();
}

inline void pic_io_power_off() {
	_pic_io_power_1();
}

