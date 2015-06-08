#ifndef SERIAL_H_
#define SERIAL_H_


void serialBegin(unsigned int baudRate);
unsigned char serialAvailable();
unsigned char serialRead();
void serialPrintString(char* chrs);
void serialPrint(unsigned char chr);


#endif /* SERIAL_H_ */