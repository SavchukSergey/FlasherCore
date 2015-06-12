#include "../settings.h"
#include <util/delay.h>
#include "stk.h"
#include "../serial.h"
#include "stk_pic.h"
#include "stk_avr.h"

#define BUFFER_SIZE 256
unsigned char buff[BUFFER_SIZE]; // global block storage

#define EECHUNK (32)

#if EECHUNK >= BUFFER_SIZE
#error "EECHUNK must be less than buffer size"
#endif


int error = 0;
int pmode = 0;
// address for reading and writing, set by 'U' command
int here;

#define STK_TARGET_PIC 'P'
#define STK_TARGET_AVR 'A'
unsigned char target = STK_TARGET_PIC;

#define beget16(addr) (*addr * 256 + *(addr+1) )


parameter param;

static unsigned char getch() {
	static char cnt = 0;
	cnt++;
	if (cnt & 1) {
		stk_pin_led_1();
	} else {
		stk_pin_led_0();
	}
	return serialRead();
}

static void breply(unsigned char b) {
	if (CRC_EOP == getch()) {
		serialPrint(STK_INSYNC);
		serialPrint(b);
		serialPrint(STK_OK);
	} else {
		error++;
		serialPrint(STK_NOSYNC);
	}
}


static void stk_fill(unsigned int n) {
	for (unsigned int x = 0; x < n; x++) {
		buff[x] = getch();
	}
}

#define PTIME 10
static void pulse(unsigned char times) {
	do {
		stk_pin_led_1();
		_delay_ms(PTIME);
		stk_pin_led_0();
		_delay_ms(PTIME);
	}
	while (times--);
}

static void empty_reply() {
	if (CRC_EOP == getch()) {
		serialPrint(STK_INSYNC);
		serialPrint(STK_OK);
	} else {
		error++;
		serialPrint(STK_NOSYNC);
	}
}

static void stk_read_signature() {
	if (CRC_EOP != getch()) {
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
	// call this after reading parameter packet into buff[]
	param.devicecode = buff[0];
	param.revision   = buff[1];
	param.progtype   = buff[2];
	param.parmode    = buff[3];
	param.polling    = buff[4];
	param.selftimed  = buff[5];
	param.lockbytes  = buff[6];
	param.fusebytes  = buff[7];
	param.flashpoll  = buff[8];
	// ignore buff[9] (= buff[8])
	// following are 16 bits (big endian)
	param.eeprompoll = beget16(&buff[10]);
	param.pagesize   = beget16(&buff[12]);
	param.eepromsize = beget16(&buff[14]);

	// 32 bits flashsize (big endian)
	param.flashsize = buff[16] * 0x01000000
	+ buff[17] * 0x00010000
	+ buff[18] * 0x00000100
	+ buff[19];

}

static void stk_start_pmode() {
	pulse(5);
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

static void stk_universal() {
	stk_fill(4);
	unsigned char res;
	
	if (target == STK_TARGET_AVR) {
		res = stk_avr_universal(buff[0], buff[1], buff[2], buff[3]);
	} else {
		res = stk_pic_universal(buff[0], buff[1], buff[2], buff[3]);
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

static unsigned char stk_write_flash_pages(unsigned int length) {
	unsigned int x = 0;
	unsigned int page = stk_current_page(here);
	while (x < length) {
		if (page != stk_current_page(here)) {
			stk_commit(page);
			page = stk_current_page(here);
		}
		unsigned int val = buff[x+1] * 256 + buff[x];
		x += 2;
		stk_flash(here, val);
		here++;
	}

	stk_commit(page);

	return STK_OK;
}

//to do: buffer may overflow. need to split by BUFFER_SIZE
static unsigned char stk_write_flash(unsigned int length) {
	stk_fill(length);
	return stk_write_flash_pages(length);
}

// write (length) bytes, (start) is a byte address
static unsigned char stk_write_eeprom_chunk(unsigned int start, unsigned char length) {
	// this writes byte-by-byte,
	// page writing may be faster (4 bytes at a time)
	stk_fill(length);
	for (unsigned int x = 0; x < length; x++) {
		unsigned int addr = start + x;
		if (target == STK_TARGET_AVR) {
			stk_avr_write_eeprom(addr, buff[x]);
		} else {
			stk_pic_write_eeprom(addr, buff[x]);
		}
	}
	return STK_OK;
}

static unsigned char stk_write_eeprom(int length) {
	// here is a word address, get the byte address
	unsigned int start = here * 2;
	unsigned int remaining = length;
	if (length > param.eepromsize) {
		error++;
		return STK_FAILED;
	}
	while (remaining > EECHUNK) {
		stk_write_eeprom_chunk(start, EECHUNK);
		start += EECHUNK;
		remaining -= EECHUNK;
	}
	stk_write_eeprom_chunk(start, remaining);
	return STK_OK;
}

static void stk_program_page() {
	unsigned char result = STK_FAILED;
	unsigned int length = 256 * getch();
	length += getch();
	unsigned char memtype = getch();
	if (memtype == 'F') {
		result = stk_write_flash(length);
	} else if (memtype == 'E') {
		result = stk_write_eeprom(length);
	} else {
		serialPrint(STK_FAILED);
		return;
	}
	if (CRC_EOP == getch()) {
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
		serialPrint((char) (val >> 8));
		serialPrint((char) (val & 0xff));
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
	unsigned int length = 256 * getch();
	length += getch();
	unsigned char memtype = getch();
	if (CRC_EOP != getch()) {
		error++;
		serialPrint(STK_NOSYNC);
	} else {
		serialPrint(STK_INSYNC);
		if (memtype == 'F') result = stk_flash_read_page(length);
		if (memtype == 'E') result = stk_eeprom_read_page(length);
		serialPrint(result);
	}
}

static void avrisp() {
	unsigned char ch = getch();
	switch (ch) {
		case '0':
			error = 0;
			empty_reply();
			break;
		case '1':
			if (getch() == CRC_EOP) {
				serialPrint(STK_INSYNC);
				serialPrintString("AVR ISP PIC");
				serialPrint(STK_OK);
			}
			break;
		case 'A':
			stk_get_version(getch());
			break;
		case 'B':
			stk_fill(20);
			stk_set_parameters();
			empty_reply();
			break;
		case 'E': // extended parameters - ignore for now
			stk_fill(5);
			empty_reply();
			break;
		case 'P':
			stk_start_pmode();
			empty_reply();
			break;
		case 'U': // set address (word)
			here = getch();
			here += 256 * getch();
			empty_reply();
			break;

		case 0x60: //STK_PROG_FLASH
			getch(); //low
			getch(); //high
			empty_reply();
			break;
		case 0x61: //STK_PROG_DATA
			getch(); //data
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

		// expecting a command, not CRC_EOP
		// this is how we can get back in sync
		case CRC_EOP:
			error++;
			serialPrint(STK_NOSYNC);
			break;

		// anything else we will return STK_UNKNOWN
		default:
			error++;
			if (CRC_EOP == getch())
				serialPrint(STK_UNKNOWN);
			else
				serialPrint(STK_NOSYNC);
	}
}

void stk_setup() {
	serialBegin(STK_BAUD_RATE);

	stk_pin_led_output();
	pulse(2);
}

void stk_loop() {
	while (1) {
		if (serialAvailable()) {
			stk_pin_led_1();
			avrisp();
			stk_pin_led_0();
		}
	}
}