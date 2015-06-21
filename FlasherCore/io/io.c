#include "../settings.h"
#include "io_pic.c"
#include "io_avr.c"


#define STK_IO_LED_OR_MASK (1 << STK_IO_LED_BIT)
#define STK_IO_LED_AND_MASK (~STK_IO_LED_OR_MASK)


inline void _stk_io_led_0() {
	STK_IO_LED_PORT &= STK_IO_LED_AND_MASK;
}

inline void _stk_io_led_1() {
	STK_IO_LED_PORT |= STK_IO_LED_OR_MASK;
}

inline void stk_io_led_output() {
	STK_IO_LED_DDR |= STK_IO_LED_OR_MASK;
}

inline void stk_io_led_input() {
	STK_IO_LED_DDR &= STK_IO_LED_AND_MASK;
}

inline void stk_io_led_off() {
	_stk_io_led_0();
}

inline void stk_io_led_on() {
	_stk_io_led_1();
}