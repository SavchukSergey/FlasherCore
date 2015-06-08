#include "settings.h"
#include "serial.h"
#include <avr/io.h>
#include <math.h>

void serialBegin(unsigned int baudRate) {
	unsigned int ubbrValue = lrint((F_CPU / 16 * baudRate) - 1);
	UBRR0H = (unsigned char) (ubbrValue >> 8);
	UBRR0L = (unsigned char) (ubbrValue);
	//enable the receiver an transmitter
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	UCSR0C |= (3 << UCSZ00); //8-bit data length
}

unsigned char serialAvailable() {
	return !(UCSR0A & (1 << RXC0));
}

unsigned char serialRead() {
	while (!(UCSR0A & (1 << RXC0)) );
	return UDR0;
}

void serialPrint(unsigned char chr) {
	while (! (UCSR0A & (1 << UDRE0)) );
	UDR0 = chr;
}

void serialPrintString(char* chrs) {
	while (chrs[0]) {
		serialPrint(chrs[0]);
		chrs++;
	}
}