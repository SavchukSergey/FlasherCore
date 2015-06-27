#include "../settings.h"
#include "io_pic.c"
#include "io_avr.c"

#define STK_IO_POWER_OR_MASK (1 << STK_IO_POWER_BIT)
#define STK_IO_POWER_AND_MASK (~STK_IO_POWER_OR_MASK)

inline void _stk_io_power_0() {
	STK_IO_POWER_PORT &= STK_IO_POWER_AND_MASK;
}

inline void _stk_io_power_1() {
	STK_IO_POWER_PORT |= STK_IO_POWER_OR_MASK;
}

inline void stk_io_power_output() {
	STK_IO_POWER_DDR |= STK_IO_POWER_OR_MASK;
}

inline void stk_io_power_input() {
	STK_IO_POWER_DDR &= STK_IO_POWER_AND_MASK;
}

inline void stk_io_power_off() {
	_stk_io_power_0();
}

inline void stk_io_power_on() {
	_stk_io_power_1();
}