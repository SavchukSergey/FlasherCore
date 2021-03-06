#ifndef SERIAL_H_
#define SERIAL_H_

#include <avr/io.h>

void serialBegin(unsigned int baudRate);
unsigned char serialAvailable();
unsigned char serialRead();
void serialPrint(unsigned char chr);

#ifndef UDR

#define UBRR UBRR0
#define UBRRH UBRR0H
#define UBRRL UBRR0L
#define UCSRB UCSR0B
#define UCSRC UCSR0C
#define RXEN RXEN0
#define TXEN TXEN0
#define RXC RXC0
#define UCSZ0 UCSZ00
#define UCSRA UCSR0A
#define U2X U2X0
#define UDR UDR0
#define UDRE UDRE0
#define RXCIE RXCIE0
#define TXCIE TXCIE0
#define UDRIE UDRIE0

#endif

#ifndef USART_RX_vect
#define USART_RX_vect USART_RXC_vect
#endif

#endif