#include "stream.h"
#include "serial.h"

unsigned char streamReadChar() {
	return serialRead();
}

void streamWriteChar(unsigned char ch) {
	serialPrint(ch);
}

unsigned int streamReadBeInt16() {
	unsigned int res = streamReadChar();
	res <<= 8;
	res |= streamReadChar();
	return res;
}

unsigned long streamReadBeInt32() {
	unsigned long res = streamReadChar();
	res <<= 8;
	res |= streamReadChar();
	res <<= 8;
	res |= streamReadChar();
	res <<= 8;
	res |= streamReadChar();
	return res;
}

unsigned int streamReadLeInt16() {
	unsigned int res = streamReadChar();
	res += 256 * streamReadChar();
	return res;
}

unsigned int streamReadDecimal() {
	unsigned int res = 0;
	while (1) {
		unsigned char ch = streamReadChar();
		if (ch >= '0' && ch <= '9') {
			res = res * 10;
			res += ch - '0';
		} else {
			break;
		}
	}
	return res;
}

unsigned char streamReadHexUInt8() {
	unsigned char res = 0;
	res |= streamReadHexDigit();
	res = res << 4;
	res |= streamReadHexDigit();
	return res;
}

unsigned int streamReadHexUInt16() {
	unsigned int res = 0;
	res |= streamReadHexDigit();
	res = res << 4;
	res |= streamReadHexDigit();
	res = res << 4;
	res |= streamReadHexDigit();
	res = res << 4;
	res |= streamReadHexDigit();
	return res;
}

void streamPrintString(char* chrs) {
	while (chrs[0]) {
		serialPrint(chrs[0]);
		chrs++;
	}
}

unsigned char streamReadHexDigit() {
	unsigned char ch = streamReadChar();
	if (ch >= '0' && ch <= '9') return ch - '0';
	if (ch >= 'A' && ch <= 'F') return ch - 'A' + 10;
	if (ch >= 'a' && ch <= 'f') return ch - 'a' + 10;
	return 0;
}

void streamPrintHexDigit(char ch) {
	ch = ch & 0x0f;
	if (ch >= 0 && ch <= 9) {
		serialPrint(ch + '0');
	} else if (ch <= 15) {
		serialPrint(ch - 10 + 'A');
	} else {
		serialPrint('-');
	}
}

void streamPrintHexUInt8(unsigned char data) {
	streamPrintHexDigit(data >> 4);
	streamPrintHexDigit(data >> 0);
}

void streamPrintHexUInt16(unsigned int data) {
	streamPrintHexDigit(data >> 12);
	streamPrintHexDigit(data >> 8);
	streamPrintHexDigit(data >> 4);
	streamPrintHexDigit(data >> 0);
}