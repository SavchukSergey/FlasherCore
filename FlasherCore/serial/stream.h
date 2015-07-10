#ifndef STREAM_H_
#define STREAM_H_


unsigned char streamReadChar();
void streamWriteChar(unsigned char ch);

unsigned int streamReadBeInt16();
unsigned long streamReadBeInt32();
unsigned int streamReadLeInt16();
unsigned int streamReadDecimal();

unsigned char streamReadHexDigit();
unsigned char streamReadHexUInt8();
unsigned int streamReadHexUInt16();

void streamPrintHexUInt16(unsigned int data);
void streamPrintHexUInt8(unsigned char data);

void streamPrintString(char* chrs);

#endif