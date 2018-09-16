#include "../settings.h"
#include <util/delay.h>
#include "../io/io.c"
#include "stk.h"
#include "../serial/serial.h"
#include "../serial/stream.h"
#include "stk_pic.h"
#include "stk_avr.h"
#include "../utils.h"

#define EECHUNK (32)

int error = 0;
int pmode = 0;
// address for reading and writing, set by 'U' command
int here;

#define STK_TARGET_PIC 'P'
#define STK_TARGET_AVR 'A'
unsigned char target = STK_TARGET_AVR;

parameter param;


static void breply(unsigned char b) {
	if (CRC_EOP == streamReadChar()) {
		serialPrint(STK_INSYNC);
		serialPrint(b);
		serialPrint(STK_OK);
	} else {
		error++;
		serialPrint(STK_NOSYNC);
	}
}

static void empty_reply() {
	if (CRC_EOP == streamReadChar()) {
		serialPrint(STK_INSYNC);
		serialPrint(STK_OK);
	} else {
		error++;
		serialPrint(STK_NOSYNC);
	}
}

static void stk_read_signature() {
	if (CRC_EOP != streamReadChar()) {
		error++;
		serialPrint(STK_NOSYNC);
	} else {
		if (target == STK_TARGET_AVR) {
			stk_avr_read_signature();
		} else {
			stk_pic_read_signature();
		}
	}
}

static void stk_set_parameters() {
	param.devicecode = streamReadChar();
	param.revision   = streamReadChar();
	param.progtype   = streamReadChar();
	param.parmode    = streamReadChar();
	param.polling    = streamReadChar();
	param.selftimed  = streamReadChar();
	param.lockbytes  = streamReadChar();
	param.fusebytes  = streamReadChar();
	param.flashpoll  = streamReadChar();
	streamReadChar();
	param.eeprompoll = streamReadBeInt16();
	param.pagesize   = streamReadBeInt16();
	param.eepromsize = streamReadBeInt16();
	param.flashsize = streamReadBeInt32();
	empty_reply();
	
	if (param.devicecode == 0x01) {
		target = STK_TARGET_PIC;
	} else {
		target = STK_TARGET_AVR;
	}
}

static void stk_set_ext_parameters() {
	unsigned char len = streamReadChar();
	for (unsigned char i = 1; i < len; i++) {
		streamReadChar();
	}
	empty_reply();
}

static void stk_start_pmode() {
	if (target == STK_TARGET_AVR) {
		stk_avr_start_pmode();
	} else {
		stk_pic_start_pmode();
	}
	pmode = 1;
}

static void stk_end_pmode() {
	if (target == STK_TARGET_AVR) {
		stk_avr_end_pmode();
	} else {
		stk_pic_end_pmode();
	}
	pmode = 0;
	stk_io_power_off();
}

static void stk_get_version(unsigned char c) {
	switch (c) {
		case 0x80:
			breply(HWVER);
			break;
		case 0x81:
			breply(SWMAJ);
			break;
		case 0x82:
			breply(SWMIN);
			break;
		case 0x93:
			breply('S'); // serial programmer
			break;
		default:
			breply(0);
			break;
	}
}

static void stk_chip_erase() {
	if (target == STK_TARGET_AVR) {
		stk_avr_erase();
	} else {
		stk_pic_erase();
	}
}


static void stk_universal() {
	unsigned char a = streamReadChar();
	unsigned char b = streamReadChar();
	unsigned char c = streamReadChar();
	unsigned char d = streamReadChar();

	unsigned char res;
	
	if (target == STK_TARGET_AVR) {
		res = stk_avr_universal(a, b, c, d);
	} else {
		res = stk_pic_universal(a, b, c, d);
	}
	
	breply(res);
}

static void stk_flash(unsigned int addr, int data) {
	if (target == STK_TARGET_AVR) {
		stk_avr_write_flash(addr, data);
	} else {
		stk_pic_write_flash(addr, data);
	}
}

static void stk_commit(unsigned int addr) {
	if (target == STK_TARGET_AVR) {
		stk_avr_commit(addr);
	} else {
		stk_pic_commit(addr);
	}
}

static unsigned int stk_current_page(unsigned int addr) {
	if (param.pagesize == 32)  return here & 0xFFFFFFF0;
	if (param.pagesize == 64)  return here & 0xFFFFFFE0;
	if (param.pagesize == 128) return here & 0xFFFFFFC0;
	if (param.pagesize == 256) return here & 0xFFFFFF80;
	return here;
}

static unsigned char stk_write_flash(unsigned int length) {
	unsigned int x = 0;
	unsigned int page = stk_current_page(here);
	while (x < length) {
		if (page != stk_current_page(here)) {
			stk_commit(page);
			page = stk_current_page(here);
		}
		unsigned int val = streamReadChar();
		val |= streamReadChar() * 256;
		x += 2;
		stk_flash(here, val);
		here++;
	}

	stk_commit(page);

	return STK_OK;
}

// write (length) bytes, (start) is a byte address
static unsigned char stk_write_eeprom_chunk(unsigned int start, unsigned char length) {
	// this writes byte-by-byte,
	// page writing may be faster (4 bytes at a time)
	for (unsigned int x = 0; x < length; x++) {
		unsigned int addr = start + x;
		unsigned char data = serialRead();
		if (target == STK_TARGET_AVR) {
			stk_avr_write_eeprom(addr, data);
		} else {
			stk_pic_write_eeprom(addr, data);
		}
	}
	return STK_OK;
}

static unsigned char stk_write_eeprom(unsigned int length) {
	// here is a word address, get the byte address
	unsigned int start = here * 2;
	unsigned int remaining = length;
	while (remaining > EECHUNK) {
		stk_write_eeprom_chunk(start, EECHUNK);
		start += EECHUNK;
		remaining -= EECHUNK;
	}
	if (remaining > 0) {
		stk_write_eeprom_chunk(start, remaining);
	}
	return STK_OK;
}

static void stk_program_page() {
	unsigned char result = STK_FAILED;
	unsigned int length = streamReadBeInt16();
	unsigned char memtype = streamReadChar();
	if (memtype == 'F') {
		result = stk_write_flash(length);
	} else if (memtype == 'E') {
		result = stk_write_eeprom(length);
	} else {
		serialPrint(STK_FAILED);
		return;
	}
	if (CRC_EOP == streamReadChar()) {
		serialPrint(STK_INSYNC);
		serialPrint(result);
	} else {
		error++;
		serialPrint(STK_NOSYNC);
	}
}

static unsigned int stk_flash_read(unsigned int addr) {
	if (target == STK_TARGET_AVR) {
		return stk_avr_flash_read(addr);
	} else {
		return stk_pic_flash_read(addr);
	}
}

static unsigned char stk_eeprom_read(unsigned int addr) {
	if (target == STK_TARGET_AVR) {
		return stk_avr_eeprom_read(addr);
	} else {
		return stk_pic_eeprom_read(addr);
	}
}

static unsigned char stk_flash_read_page(unsigned int length) {
	for (unsigned int x = 0; x < length; x += 2) {
		unsigned int val = stk_flash_read(here);
		serialPrint((char) (val & 0xff));
		serialPrint((char) (val >> 8));
		here++;
	}
	return STK_OK;
}

static unsigned char stk_eeprom_read_page(unsigned int length) {
	// here again we have a word address
	unsigned int start = here * 2;
	for (unsigned int x = 0; x < length; x++) {
		unsigned int addr = start + x;
		unsigned char ee = stk_eeprom_read(addr);
		serialPrint(ee);
	}
	return STK_OK;
}

static void stk_read_page() {
	unsigned char result = STK_FAILED;
	unsigned int length = streamReadBeInt16();
	unsigned char memtype = streamReadChar();
	if (CRC_EOP != streamReadChar()) {
		error++;
		serialPrint(STK_NOSYNC);
	} else {
		serialPrint(STK_INSYNC);
		if (memtype == 'F') {
			result = stk_flash_read_page(length);
		} else if (memtype == 'E') {
			result = stk_eeprom_read_page(length);
		}
		serialPrint(result);
	}
}

static void stk_service() {
	unsigned char ch = streamReadChar();
	switch (ch) {
		case 'P':
			target = STK_TARGET_PIC;
			streamPrintString("Hello. Pic Service Mode ");
			break;
		case 'A':
			target = STK_TARGET_AVR;
			streamPrintString("Hello. Avr Service Mode ");
			break;
		default:
			streamPrintString("Unknown target");
			return;
	}
	
	while(1) {
		ch = serialRead();
		if (ch == 'P') {
			stk_start_pmode();
		} else if (ch == 'Q') {
			stk_end_pmode();
		} else if (ch == 'E') {
			streamPrintString("Bye");
			return;
		} else if (ch == 'Z') {
			stk_chip_erase();
			streamPrintString("Chip erased");
		} else if (ch == ' ') {
			serialPrint(' ');
		} else if (ch == 'k') {
			unsigned char powerPin = serialRead();
			if (powerPin == '0') {
				stk_io_power_off();
			} else if (powerPin == '1') {
				stk_io_power_on();
			}
		} else if (ch == 't') {
			unsigned int micros = streamReadDecimal();
			delay_us(micros);
		} else if (ch == 'T') {
			unsigned int millis = streamReadDecimal();
			delay_ms(millis);
		} else {
			if (target == STK_TARGET_AVR) {
				stk_avr_service(ch);
			} else {
				stk_pic_service(ch);
			}
		}
	}
}

static void avrisp() {
	unsigned char ch = streamReadChar();
	switch (ch) {
		case '0':
			error = 0;
			empty_reply();
			break;
		case '1':
			if (streamReadChar() == CRC_EOP) {
				serialPrint(STK_INSYNC);
				streamPrintString("AVR ISP PIC");
				serialPrint(STK_OK);
			}
			break;
		case 'A':
			stk_get_version(streamReadChar());
			break;
		case 'B':
			stk_set_parameters();
			break;
		case 'E': // extended parameters - ignore for now
			stk_set_ext_parameters();
			break;
		case 'P':
			stk_start_pmode();
			empty_reply();
			break;
		case 'U': // set address (word)
			here = streamReadLeInt16();
			empty_reply();
			break;

		case 0x60: //STK_PROG_FLASH
			streamReadChar(); //low
			streamReadChar(); //high
			empty_reply();
			break;
		case 0x61: //STK_PROG_DATA
			streamReadChar(); //data
			empty_reply();
			break;

		case 0x64: //STK_PROG_PAGE
			stk_program_page();
			break;

		case 0x74: //STK_READ_PAGE 't'
			stk_read_page();
			break;

		case 'V': //0x56
			stk_universal();
			break;
		case 'Q': //0x51
			error = 0;
			stk_end_pmode();
			empty_reply();
			break;

		case 0x75: //STK_READ_SIGN 'u'
			stk_read_signature();
			break;
			
		case 'S':
			stk_service();
			break;

		// expecting a command, not CRC_EOP
		// this is how we can get back in sync
		case CRC_EOP:
			error++;
			serialPrint(STK_NOSYNC);
			break;

		// anything else we will return STK_UNKNOWN
		default:
			error++;
			if (CRC_EOP == streamReadChar())
				serialPrint(STK_UNKNOWN);
			else
				serialPrint(STK_NOSYNC);
	}
}

void stk_setup() {
	serialBegin(STK_BAUD_RATE);
	stk_pic_setup();
	stk_io_power_output();
	stk_io_power_off();
}

void stk_loop() {
	while (1) {
		if (serialAvailable()) {
			avrisp();
		}
	}
}
