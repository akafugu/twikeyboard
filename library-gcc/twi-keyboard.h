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

#ifndef TWI_KEYBOARD_H
#define TWI_KEYBOARD_H

#include <stdbool.h>
#include "twi.h"

#define KEYBOARD_BUTTON1 0
#define KEYBOARD_BUTTON2 1
#define KEYBOARD_BUTTON3 2
#define KEYBOARD_BUTTON4 3
#define KEYBOARD_BUTTON5 4
#define KEYBOARD_BUTTON6 5
#define KEYBOARD_BUTTON7 6

#define KEYBOARD_LED1 0
#define KEYBOARD_LED2 1
#define KEYBOARD_LED3 2
#define KEYBOARD_LED4 3
#define KEYBOARD_LED5 4

#define KEYREPEAT_STOP 0
#define KEYREPEAT_SLOW 1
#define KEYREPEAT_MEDIUMSLOW 2
#define KEYREPEAT_MEDIUM 3
#define KEYREPEAT_MEDIUMFAST 4
#define KEYREPEAT_FAST 5

void keyboard_reset(uint8_t addr);
void keyboard_change_address(uint8_t cur_addr, uint8_t new_addr);

void keyboard_led_clearall(uint8_t addr);
void keyboard_led_set(uint8_t addr, uint8_t no, uint8_t brightness);
void keyboard_led_pulse(uint8_t addr, uint8_t no, bool on);
void keyboard_led_dim(uint8_t addr, uint8_t no, uint8_t brightness);

void keyboard_key_set_repeat(uint8_t addr, uint8_t no, uint8_t mode);
uint8_t keyboard_key_get_keyup(uint8_t addr);
uint8_t keyboard_key_get_keydown(uint8_t addr);

#endif // TWI_KEYBOARD_H
