#include "utils.h"
#include "settings.h"
#include <util/delay.h>

void delay_ms(unsigned int val) {
	while (val > 100) {
		_delay_ms(100);
		val -= 100;
	}
	while (val > 10) {
		_delay_ms(10);
		val -= 10;
	}
	while (val) {
		_delay_ms(1);
		val--;
	}
}

void delay_us(unsigned int val) {
	while (val > 100) {
		_delay_us(100);
		val -= 100;
	}
	while (val > 10) {
		_delay_us(10);
		val -= 10;
	}
	while (val) {
		_delay_us(1);
		val--;
	}
}
