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