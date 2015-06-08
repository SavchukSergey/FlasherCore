#include "settings.h"
#include "serial.h"
#include <avr/io.h>
#include <math.h>

void serialBegin(unsigned int baudRate) {
	unsigned int ubbrValue = lrint((F_CPU / 16 * baudRate) - 1);
	UBRRH = (unsigned char) (ubbrValue >> 8);
	UBRRL = (unsigned char) (ubbrValue);
	//enable the receiver an transmitter
	UCSRB = (1 << RXEN) | (1 << TXEN);
	UCSRC |= (3 << UCSZ0); //8-bit data length
}

unsigned char serialAvailable() {
	return !(UCSRA & (1 << RXC));
}

unsigned char serialRead() {
	while (!(UCSRA & (1 << RXC)) );
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