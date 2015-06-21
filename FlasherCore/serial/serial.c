#include "../settings.h"
#include "serial.h"
#include "ringbuffer.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>

ringbuffer receiveBuffer;
ringbuffer transmitBuffer;

void serialBegin(unsigned int baudRate) {
	unsigned int ubbrValue = lrint((F_CPU / (16ul * baudRate)) - 1);

	#ifndef URSEL
	UBRR = ubbrValue;
	#else
	UBRRH = (ubbrValue >> 8);
	UBRRL = ubbrValue & 0xff;
	#endif
	
	//enable the receiver an transmitter, enable interrupts
	UCSRB = (1 << RXEN) | (1 << TXEN) | (1 << RXCIE);
	//asynchronous, no parity, 8 bits, 1 stop bit

	#ifndef URSEL
	UCSRC = (3 << UCSZ0);
	#else
	UCSRC = (1 << URSEL) | (3 << UCSZ0);
	#endif
	//to do: for atmega8 use URSEL?
	
	ring_buffer_reset(&receiveBuffer);
	ring_buffer_reset(&transmitBuffer);
}

unsigned char serialAvailable() {
	return receiveBuffer.size > 0;
}

unsigned char serialRead() {
	return ring_buffer_dequeue(&receiveBuffer);
}

void serialPrint(unsigned char chr) {
	ring_buffer_queue(&transmitBuffer, chr);
	UCSRB |= (1 << UDRIE);
}

void serialPrintString(char* chrs) {
	while (chrs[0]) {
		serialPrint(chrs[0]);
		chrs++;
	}
}

char serialReadHexDigit() {
	char ch = serialRead();
	if (ch >= '0' && ch <= '9') return ch - '0';
	if (ch >= 'A' && ch <= 'F') return ch - 'A' + 10;
	if (ch >= 'a' && ch <= 'f') return ch - 'a' + 10;
	return 0;
}

void serialPrintHexDigit(char ch) {
	ch = ch & 0x0f;
	if (ch >= 0 && ch <= 9) {
		serialPrint(ch + '0');
	} else if (ch <= 15) {
		serialPrint(ch - 10 + 'A');
	} else {
		serialPrint('-');
	}
}

void serialPrintHexUInt8(unsigned char data) {
	serialPrintHexDigit(data >> 4);
	serialPrintHexDigit(data >> 0);
}

void serialPrintHexUInt16(unsigned int data) {
	serialPrintHexDigit(data >> 12);
	serialPrintHexDigit(data >> 8);
	serialPrintHexDigit(data >> 4);
	serialPrintHexDigit(data >> 0);
}

unsigned char serialReadHexUInt8() {
	unsigned char res = 0;
	res |= serialReadHexDigit();
	res = res << 4;
	res |= serialReadHexDigit();
	return res;
}

unsigned int serialReadHexUInt16() {
	unsigned int res = 0;
	res |= serialReadHexDigit();
	res = res << 4;
	res |= serialReadHexDigit();
	res = res << 4;
	res |= serialReadHexDigit();
	res = res << 4;
	res |= serialReadHexDigit();
	return res;
}



ISR(USART_RX_vect) {
	unsigned char ch = UDR;
	if (receiveBuffer.size < RING_BUFFER_SIZE) {
		ring_buffer_queue(&receiveBuffer, ch);
	}
}

ISR(USART_UDRE_vect) {
	if (transmitBuffer.size > 0) {
		unsigned char ch = ring_buffer_dequeue(&transmitBuffer);
		UDR = ch;
	} else {
		UCSRB &= ~(1 << UDRIE);
	}
}