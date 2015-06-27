#include <avr/io.h>
#include <avr/interrupt.h>
#include "stk/stk.h"

int main() {
	cli();
//	PORTB = 0x00;
	PORTC = 0x00;
	PORTD = 0x01;
//	DDRB = 0x00;
	DDRC = 0x00;
	DDRD = 0x02;

	stk_setup();
	sei();
	stk_loop();
}
