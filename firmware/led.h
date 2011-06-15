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

#ifndef LED_H_
#define LED_H_

#include <avr/io.h>

#define sbi(var, mask)   ((var) |= (uint8_t)(1 << mask))
#define cbi(var, mask)   ((var) &= (uint8_t)~(1 << mask))

typedef enum LED_STATE_VALUE {
	LED_STOP = 0,
	LED_DIM_DOWN,
	LED_DIM_UP,
	LED_PULSE_UP,
	LED_PULSE_DOWN
} LED_STATE_VALUE;

void led_init(void);
void led_clear(void);
void set_brightness(uint8_t led, uint8_t b);
void dim_led(uint8_t led, uint8_t b);
void led_timer(void);
void set_led_pulse(uint8_t led, uint8_t on);

// PINOUT for ATMEGA328 in TQFP package
#ifdef ATMEGA328_TQFP
#error "Pins not defined for ATMega328"
// Pinout for ATTiny2313
#else

#define MAX_LED LED5

// Led1: PB0
#define LED1_DDR  DDRB
#define LED1_PORT PORTB
#define LED1_BIT  PB0
#define LED1      0
// Led2: PB1
#define LED2_DDR  DDRB
#define LED2_PORT PORTB
#define LED2_BIT  PB1
#define LED2      1
// Led3: PB2
#define LED3_DDR DDRB
#define LED3_PORT PORTB
#define LED3_BIT  PB2
#define LED3      2
// Led4: PA0
#define LED4_DDR DDRA
#define LED4_PORT PORTA
#define LED4_BIT  PA0
#define LED4      3
// Led5: PA1
#define LED5_DDR DDRA
#define LED5_PORT PORTA
#define LED5_BIT  PA1
#define LED5      4
#endif

#endif // LED_H_
