#include <avr/io.h>
#include "stk.h"

int main() {
	stk_setup();
	stk_loop();
}
