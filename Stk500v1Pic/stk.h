#ifndef STK_H_
#define STK_H_

#include "settings.h"

#define STK_LED_HEARBEAT 13

//extern int error;

// STK Definitions
#define STK_OK      0x10
#define STK_FAILED  0x11
#define STK_UNKNOWN 0x12
#define STK_INSYNC  0x14
#define STK_NOSYNC  0x15
#define CRC_EOP     0x20 //ok it is a space...

#define HWVER 2
#define SWMAJ 1
#define SWMIN 18

void stk_setup();
void stk_loop();

typedef struct param {
	unsigned char devicecode;
	unsigned char revision;
	unsigned char progtype;
	unsigned char parmode;
	unsigned char polling;
	unsigned char selftimed;
	unsigned char lockbytes;
	unsigned char fusebytes;
	int flashpoll;
	int eeprompoll;
	int pagesize;
	int eepromsize;
	int flashsize;
} parameter;



#define STK_PIN_LED_OR_MASK (1 << STK_PIN_LED_BIT)
#define STK_PIN_LED_AND_MASK (~STK_PIN_LED_OR_MASK)


inline void stk_pin_led_0() {
	STK_PIN_LED_PORT &= STK_PIN_LED_AND_MASK;
}

inline void stk_pin_led_1() {
	STK_PIN_LED_PORT |= STK_PIN_LED_OR_MASK;
}

inline void stk_pin_led_output() {
	STK_PIN_LED_DDR |= STK_PIN_LED_OR_MASK;
}

inline void stk_pin_led_input() {
	STK_PIN_LED_DDR &= STK_PIN_LED_AND_MASK;
}

#endif