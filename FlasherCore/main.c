#include <avr/io.h>
#include <avr/interrupt.h>
#include "stk/stk.h"

int main() {
	stk_setup();
	sei();
	stk_loop();
}
