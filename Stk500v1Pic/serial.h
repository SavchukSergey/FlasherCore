#ifndef SERIAL_H_
#define SERIAL_H_


void serialBegin(unsigned int baudRate);
unsigned char serialAvailable();
unsigned char serialRead();
void serialPrintString(char* chrs);
void serialPrint(unsigned char chr);

#ifndef UBRRH

#define UBRRH UBRR0H
#define UBRRL UBRR0L
#define UCSRB UCSR0B
#define UCSRC UCSR0C
#define RXEN RXEN0
#define TXEN TXEN0
#define RXC RXC0
#define UCSZ0 UCSZ00
#define UCSRA UCSR0A
#define UDR UDR0
#define UDRE UDRE0

#endif

#endif