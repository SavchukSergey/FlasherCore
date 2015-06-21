#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <avr/io.h>

#define F_CPU 16000000UL
#define STK_BAUD_RATE 57600

#define STK_IO_LED_PORT PORTB
#define STK_IO_LED_DDR DDRB
#define STK_IO_LED_BIT PORTB5



#define PIC_IO_DATA_PORT PORTC
#define PIC_IO_DATA_PIN PINC
#define PIC_IO_DATA_DDR DDRC
#define PIC_IO_DATA_BIT PORTC1

#define PIC_IO_CLK_PORT PORTC
#define PIC_IO_CLK_DDR DDRC
#define PIC_IO_CLK_PORT_BIT PORTC2

#define PIC_IO_MCLR_PORT PORTC
#define PIC_IO_MCLR_DDR DDRC
#define PIC_IO_MCLR_PORT_BIT PORTC0

#define PIC_IO_POWER_PORT PORTC
#define PIC_IO_POWER_DDR DDRC
#define PIC_IO_POWER_PORT_BIT PORTC3



#define PIC_SIGNATURE_HIGH 0x1e
#define PIC_SIGNATURE_MIDDLE 0x93
#define PIC_SIGNATURE_LOW 0x07




#define AVR_IO_RESET_PORT PORTB
#define AVR_IO_RESET_DDR DDRB
#define AVR_IO_RESET_BIT PORTB2

#define AVR_IO_SCK_PORT PORTB
#define AVR_IO_SCK_DDR DDRB
#define AVR_IO_SCK_BIT PORTB5

#define AVR_IO_MISO_PORT PORTB
#define AVR_IO_MISO_DDR DDRB
#define AVR_IO_MISO_BIT PORTB4

#define AVR_IO_MOSI_PORT PORTB
#define AVR_IO_MOSI_DDR DDRB
#define AVR_IO_MOSI_BIT PORTB3

#define AVR_IO_SS_PORT PORTB
#define AVR_IO_SS_DDR DDRB
#define AVR_IO_SS_BIT PORTB2

#endif