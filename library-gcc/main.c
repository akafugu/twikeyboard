/*
 * Akafugu TWI Keyboard Library: Test
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
#include <avr/interrupt.h>
#include <util/delay.h>

#include <string.h>

#include "twi-keyboard.h"

#define SLAVE_ADDR 0x10

void main(void) __attribute__ ((noreturn));

void main(void)
{
	uint8_t keydown, keyup;

	twi_init_master();
	sei();

	keyboard_reset(SLAVE_ADDR); // Reset keyboard module

	keyboard_led_set(SLAVE_ADDR, KEYBOARD_LED1, 100);
	keyboard_led_pulse(SLAVE_ADDR, KEYBOARD_LED2, true);
	
	while(1)
	{
		keydown = keyboard_key_get_keydown(SLAVE_ADDR);
		if(keydown) // Check if any keydown was recorded
		{
			keyboard_led_dim(SLAVE_ADDR, KEYBOARD_LED1, 0);
			keyboard_led_set(SLAVE_ADDR, KEYBOARD_LED3, 100);
		}
		keyup = keyboard_key_get_keyup(SLAVE_ADDR);
		if(keyup) // Check if any keyup was recorded
		{
			keyboard_led_dim(SLAVE_ADDR, KEYBOARD_LED1, 100);
			keyboard_led_set(SLAVE_ADDR, KEYBOARD_LED3, 0);
		}
		_delay_ms(5);
	}
}
