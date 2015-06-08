#include "settings.h"
#include "serial.h"
#include <avr/io.h>
#include <math.h>

void serialBegin(unsigned int baudRate) {
	unsigned int ubbrValue = lrint((F_CPU / (16ul * baudRate)) - 1);
	UBRR = ubbrValue;
	//enable the receiver an transmitter
	UCSRB = (1 << RXEN) | (1 << TXEN);
	//asynchronous, no parity, 8 bits, 1 stop bit
	UCSRC = (3 << UCSZ0);
	//to do: for atmega8 use URSEL?
}

unsigned char serialAvailable() {
	return UCSRA & (1 << RXC);
}

unsigned char serialRead() {
	while (!serialAvailable()) {}
	return UDR;
}

void serialPrint(unsigned char chr) {
	while (! (UCSRA & (1 << UDRE)) );
	UDR = chr;
}

void serialPrintString(char* chrs) {
	while (chrs[0]) {
		serialPrint(chrs[0]);
		chrs++;
	}
}