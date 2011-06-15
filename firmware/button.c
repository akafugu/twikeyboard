/*
 * I2C Keyboard and LED controller
 * (C) 2011 Akafugu
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
#include "button.h"

// Wait to make sure we have disable the interupt.
#define NOP_4 asm volatile("nop\n\tnop\n\tnop\n\tnop\n\t" ::);

uint8_t saved_keystatus = 0x00;
uint8_t keyup_keys = 0x00;
uint8_t keydown_keys = 0x00;

void button_init(void)
{
	// set buttons as input
	BUTTON_DDR &= ~(_BV(BUTTON1_BIT) | _BV(BUTTON2_BIT) | _BV(BUTTON3_BIT) | _BV(BUTTON4_BIT)  | 
			_BV(BUTTON5_BIT) | _BV(BUTTON6_BIT) | _BV(BUTTON7_BIT));
	// set internal pull-up
	BUTTON_PORT  |= (_BV(BUTTON1_BIT) | _BV(BUTTON2_BIT) | _BV(BUTTON3_BIT) | _BV(BUTTON4_BIT)  | 
			_BV(BUTTON5_BIT) | _BV(BUTTON6_BIT) | _BV(BUTTON7_BIT));
}

void button_timer(void)
{
	// Get the keys
	uint8_t keystatus = ~(BUTTON_PIN)&(_BV(BUTTON1_BIT) | _BV(BUTTON2_BIT) | _BV(BUTTON3_BIT) | _BV(BUTTON4_BIT) | _BV(BUTTON5_BIT) | _BV(BUTTON6_BIT) | _BV(BUTTON7_BIT));
	// Register keys that have gone from up to down
	keydown_keys |= (uint8_t)(keystatus & ~(saved_keystatus));
	// Register keysUp events
	keyup_keys |= (uint8_t)(~(keystatus) & saved_keystatus);
	saved_keystatus = keystatus;
}

uint8_t return_keys = 0;

uint8_t translate_keys(uint8_t input)
{
	uint8_t output = 0;
	if(input & _BV(BUTTON1_BIT))
		sbi(output, BUTTON1);
	if(input & _BV(BUTTON2_BIT))
		sbi(output, BUTTON2);
	if(input & _BV(BUTTON3_BIT))
		sbi(output, BUTTON3);
	if(input & _BV(BUTTON4_BIT))
		sbi(output, BUTTON4);
	if(input & _BV(BUTTON5_BIT))
		sbi(output, BUTTON5);
	if(input & _BV(BUTTON6_BIT))
		sbi(output, BUTTON6);
	if(input & _BV(BUTTON7_BIT))
		sbi(output, BUTTON7);
	
	return output;
}

uint8_t getKeyDown()
{
	cli();
	NOP_4
	return_keys = keydown_keys;
	keydown_keys = 0x00;
	sei();
	if (return_keys == 0) // Fast processing of no keys
		return 0;
	
	return translate_keys(return_keys);
}

uint8_t getKeyUp()
{
	cli();
	NOP_4
	return_keys = keyup_keys;
	keyup_keys = 0x00;
	sei();
	if (return_keys == 0) // Fast processing of no keys
		return 0;
	
	return translate_keys(return_keys);
}
