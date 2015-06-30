#ifndef PIC_COMMANDS_H_
#define PIC_COMMANDS_H_

#include "../settings.h"
#include <util/delay.h>

inline static void pic_wait_erase() {
	_delay_ms(6);
}

inline static void pic_wait_prog() {
	_delay_ms(8);
}

#endif