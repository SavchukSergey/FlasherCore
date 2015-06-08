#include "settings.h"
#include <util/delay.h>
#include "pic.h"

#define time_dly2 1   //micros
#define time_prog 4   //millis
#define time_dprog 4  //millis
#define time_era 6  //millis

void pic_send_cmd (unsigned char cmd) {
	for (unsigned char i = 0; i < 6; i++) {
		_delay_us(1);
		pic_pin_clk_1();
		if ((cmd & 0x01) != 0) {
			pic_pin_data_1();
		} else {
			pic_pin_data_0();
		}
		cmd = cmd >> 1;
		pic_pin_clk_0();
	}
	pic_pin_data_0();
}

void pic_send_data (unsigned int data) {
	data = data & 0x3fff;
	data = data << 1;
	for (unsigned char i = 0; i < 16; i++) {
		_delay_us(1);
		pic_pin_clk_1();
		if ((data & 0x01) != 0){
			pic_pin_data_1();
		} else {
			pic_pin_data_0();
		}
		data = data >> 1;
		pic_pin_clk_0();
	}
	pic_pin_data_0();
}

unsigned int pic_receive_data () {
	unsigned int data = 0;
	pic_pin_data_input();
	pic_pin_data_1(); //turn on pull-up

	for (unsigned char i = 0; i < 16; i++) {
		pic_pin_clk_1();
		_delay_us(1);
		pic_pin_clk_0();
		_delay_us(1);
		data = data >> 1;
		if (pic_pin_data_value()) {
			data |= 0x8000;
		} else {
			data &= 0x7fff;
		}
	}
	data = data >> 1;
	data = data & 0x3fff;
	pic_pin_data_output();
	return data;
}

void pic_load_program(unsigned int data) {
	pic_send_cmd(0x02);
	_delay_us(time_dly2);
	pic_send_data(data);
	_delay_us(10);
}

void pic_load_data(unsigned char data) {
	pic_send_cmd(0x03);
	_delay_us(time_dly2);
	pic_send_data(data);
	_delay_us(10);
}

void pic_load_config(unsigned char data) {
	pic_send_cmd(0x00);
	_delay_us(time_dly2);
	pic_send_data(data);
	_delay_us(10);
}

void pic_begin_programming_program() {
	pic_send_cmd(0x08);
	_delay_ms(time_prog);
}

void pic_begin_programming_data() {
	pic_send_cmd(0x08);
	_delay_ms(time_dprog);
}

void pic_increment_address() {
	pic_send_cmd(0x06);
	_delay_us(time_dly2);
}

unsigned int pic_read_program() {
	pic_send_cmd(0x04);
	_delay_us(time_dly2);
	return pic_receive_data();
}

unsigned char pic_read_data() {
	pic_send_cmd(0x05);
	_delay_us(time_dly2);
	//todo:pic_receive_data return 16 bit. mask to first byte?
	return pic_receive_data();
}

void pic_erase_program() {
	pic_send_cmd(0x09);
	_delay_ms(time_era);
}

void pic_erase_data() {
	pic_send_cmd(0x0b);
	_delay_ms(time_era);
}

