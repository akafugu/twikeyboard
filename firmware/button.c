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

uint8_t keyrepeat_keydown_keys;
uint8_t keyrepeat_keyup_keys;

BUTTON_KEYREPEAT_STATE keyrepeat_state[8];

// This helper function makes sure we store keydown and keyup in correct order.
uint8_t translate_keys(uint8_t input)
{
	uint8_t output = 0;
	if(input == 0) // Fast processing of no keys
		return 0;
	
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

void button_init(void)
{
	// set buttons as input
	BUTTON_DDR &= ~(_BV(BUTTON1_BIT) | _BV(BUTTON2_BIT) | _BV(BUTTON3_BIT) | _BV(BUTTON4_BIT)  | 
			_BV(BUTTON5_BIT) | _BV(BUTTON6_BIT) | _BV(BUTTON7_BIT));
	// set internal pull-up
	BUTTON_PORT  |= (_BV(BUTTON1_BIT) | _BV(BUTTON2_BIT) | _BV(BUTTON3_BIT) | _BV(BUTTON4_BIT)  | 
			_BV(BUTTON5_BIT) | _BV(BUTTON6_BIT) | _BV(BUTTON7_BIT));
	
	saved_keystatus = 0;
	keyup_keys = 0;
	keydown_keys = 0;
	keyrepeat_keydown_keys = 0;
	keyrepeat_keyup_keys = 0;
	for(uint8_t b = 0; b < 8; b++)
	{
		keyrepeat_state[b] = KEYREPEAT_STOP;
	}
}

void button_timer(void)
{
	// Get the keys
	uint8_t keystatus = ~(BUTTON_PIN)&(_BV(BUTTON1_BIT) | _BV(BUTTON2_BIT) | _BV(BUTTON3_BIT) | _BV(BUTTON4_BIT) | _BV(BUTTON5_BIT) | _BV(BUTTON6_BIT) | _BV(BUTTON7_BIT));
	// Register keysDown events (in translated order)
	keydown_keys |= translate_keys((uint8_t)(keystatus & ~(saved_keystatus)));
	// Register keysUp events (in translated order)
	keyup_keys |= translate_keys((uint8_t)(~(keystatus) & saved_keystatus));
	saved_keystatus = keystatus;
	
	keyrepeat_keydown_keys |= keydown_keys;
	keyrepeat_keydown_keys &= ~keyup_keys;
	keyrepeat_keyup_keys |= keyup_keys;
}

uint16_t keyboard_counter[8];

void set_keyrepeat(uint8_t button, uint8_t state)
{
	if(button > BUTTON7 || state > KEYREPEAT_FAST)
		return;
	keyrepeat_state[button] = state;
}

void process_keyrepeat(void)
{
	for(uint8_t i = 0; i < 8; i++)
	{
		if(keyrepeat_state[i] == KEYREPEAT_STOP || ~(keyrepeat_keydown_keys) & _BV(i))
			; // Do nothing, no keyrepeat is needed
		else if(keyrepeat_keyup_keys & _BV(i))
		{
			// Key was released, so reset timer
			keyboard_counter[i] = 0;
		}
		else
		{
			if((keyrepeat_state[i] == KEYREPEAT_FAST && keyboard_counter[i] >= 75) ||
					(keyrepeat_state[i] == KEYREPEAT_MEDIUMFAST && keyboard_counter[i] >= 250) ||
					(keyrepeat_state[i] == KEYREPEAT_MEDIUM && keyboard_counter[i] >= 500) ||
					(keyrepeat_state[i] == KEYREPEAT_MEDIUMSLOW && keyboard_counter[i] >= 2000) ||
					(keyrepeat_state[i] == KEYREPEAT_SLOW && keyboard_counter[i] >= 4000))
			{
				keydown_keys |= _BV(i);
				keyboard_counter[i] = 0;
			}
			else
				keyboard_counter[i]++;
		}
	}
	// Reset keyup keys;
	keyrepeat_keyup_keys = 0;
}

uint8_t return_keys = 0;

uint8_t getKeyDown()
{
	cli();
	NOP_4
	return_keys = keydown_keys;
	keydown_keys = 0x00;
	sei();
	
	return return_keys;
}

uint8_t getKeyUp()
{
	cli();
	NOP_4
	return_keys = keyup_keys;
	keyup_keys = 0x00;
	sei();
	
	return return_keys;
}
