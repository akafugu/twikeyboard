/*
 * Akafugu TWI Keyboard Library
 * (C) 2013 Akafugu Corporation
 *
 * This program is free software; you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 *
 */

#include <avr/io.h>
#include <string.h>
#include <util/delay.h>

#include "twi.h"
#include "twi-keyboard.h"

void keyboard_reset(uint8_t addr)
{
	twi_begin_transmission(addr);
	twi_send_byte(0xFF); // sending some NOP to clear input buffer on display
	twi_send_byte(0xFF); // sending some NOP to clear input buffer on display
	twi_send_byte(0xFF); // sending some NOP to clear input buffer on display
	twi_send_byte(0xFF); // sending some NOP to clear input buffer on display
	twi_send_byte(0xFE); // clear
	twi_end_transmission();	
}

void keyboard_change_address(uint8_t cur_addr, uint8_t new_addr)
{
	twi_begin_transmission(cur_addr);
	twi_send_byte(0x81); // change address
	twi_send_byte(new_addr);
	twi_end_transmission();
	_delay_ms(5);
}


void keyboard_led_clearall(uint8_t addr)
{
	twi_begin_transmission(addr);
	twi_send_byte(0x82);
	twi_end_transmission();
}

void keyboard_led_set(uint8_t addr, uint8_t no, uint8_t brightness)
{
	twi_begin_transmission(addr);
	twi_send_byte(0x83);
	twi_send_byte(no);
	twi_send_byte(brightness);
	twi_end_transmission();
}

void keyboard_led_pulse(uint8_t addr, uint8_t no, bool on)
{
	twi_begin_transmission(addr);
	twi_send_byte(0x84);
	twi_send_byte(no);
	twi_send_byte(on);
	twi_end_transmission();
}

void keyboard_led_dim(uint8_t addr, uint8_t no, uint8_t brightness)
{
	twi_begin_transmission(addr);
	twi_send_byte(0x85);
	twi_send_byte(no);
	twi_send_byte(brightness);
	twi_end_transmission();
}

// ------

void keyboard_key_set_repeat(uint8_t addr, uint8_t no, uint8_t mode)
{
	twi_begin_transmission(addr);
	twi_send_byte(0x92);
	twi_send_byte(no);
	twi_send_byte(mode);
	twi_end_transmission();
}

uint8_t keyboard_key_get_keyup(uint8_t addr)
{
  twi_begin_transmission(addr);
  twi_send_byte(0x90);
  twi_end_transmission();

  twi_request_from(addr, 1);
  return twi_receive();
}

uint8_t keyboard_key_get_keydown(uint8_t addr)
{
  twi_begin_transmission(addr);
  twi_send_byte(0x91);
  twi_end_transmission();

  twi_request_from(addr, 1);
  return twi_receive();
}

