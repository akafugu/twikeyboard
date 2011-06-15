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

#ifndef BUTTON_H_
#define BUTTON_H_

#include <avr/io.h>

#define sbi(var, mask)   ((var) |= (uint8_t)(1 << mask))
#define cbi(var, mask)   ((var) &= (uint8_t)~(1 << mask))

void button_init(void);
void button_timer(void);

uint8_t getKeyDown(void);
uint8_t getKeyUp(void);

// PINOUT for ATMEGA328 in TQFP package
#ifdef ATMEGA328_TQFP
#error "Pins not defined for ATMega328"
// Pinout for ATTiny2313
#else

#define BUTTON_DDR  DDRD
#define BUTTON_PORT PORTD
#define BUTTON_PIN	PIND

// BUTTON1: PD5
#define BUTTON1_BIT  PD5
#define BUTTON1      0
// BUTTON2: PD6
#define BUTTON2_BIT  PD6
#define BUTTON2      1
// BUTTON3: PD2
#define BUTTON3_BIT  PD2
#define BUTTON3      2
// BUTTON4: PD3
#define BUTTON4_BIT  PD4
#define BUTTON4      3
// BUTTON5: PD4
#define BUTTON5_BIT  PD3
#define BUTTON5      4
// BUTTON6: PD1
#define BUTTON6_BIT  PD1
#define BUTTON6      5
// BUTTON7: PD4
#define BUTTON7_BIT  PD0
#define BUTTON7      6
#endif

#endif // BUTTON_H_
