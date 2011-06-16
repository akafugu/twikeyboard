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
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "usiTwiSlave.h"
#include "led.h"
#include "button.h"

// To support both ATTiny2313 and ATMegaXX8
#ifndef TIMSK0
#define TIMSK0 TIMSK
#endif

#ifndef SLAVE_ADDRESS
  #define SLAVE_ADDRESS 0x10
#endif

uint8_t EEMEM b_slave_address = SLAVE_ADDRESS;


void init_EEPROM(void)
{
	eeprom_update_byte(&b_slave_address, SLAVE_ADDRESS);
}

void init(void)
{
	cli();	// disable interrupts

	led_init();
	button_init();
	
	_delay_ms(1);
	// Check for magic reset sequence
	if ((~BUTTON_PIN & (_BV(BUTTON1_BIT) | _BV(BUTTON2_BIT) | _BV(BUTTON5_BIT))) ==
			(_BV(BUTTON1_BIT) | _BV(BUTTON2_BIT) | _BV(BUTTON5_BIT)))
	{
		// Do a reset
		init_EEPROM();
		for(uint8_t i = 0; i < 4; i++)
		{
			cbi(LED1_PORT, LED1_BIT);
			_delay_ms(125);
			sbi(LED1_PORT, LED1_BIT);
			_delay_ms(125);
		}
	}

	uint8_t stored_address = eeprom_read_byte(&b_slave_address);
	// Check that stored_address is sane otherwise fallback to factory default
	if (stored_address >= 128)
		stored_address = SLAVE_ADDRESS;
	
	usiTwiSlaveInit(stored_address);

	// Inititalize timer0 for led multiplexing
	TCCR0B = (1<<CS01); // Set Prescaler to clk/8 : 1 click = 1us. CS01=1 
	TIMSK0 |= (1<<TOIE0); // Enable Overflow Interrupt Enable
	TCNT0 = 0; // Initialize counter
	
	// Inititalize timer1 for led multiplexing
	TCCR1B = (1<<CS11); // Set Prescaler to clk/8 : 1 click = 1us. CS01=1 
	TIMSK0 |= (1<<TOIE1); // Enable Overflow Interrupt Enable
	TCNT1 = 0; // Initialize counter
	
	sei(); // enable interrupts
}

uint8_t timer = 0;

// run once every 100 us
ISR(TIMER0_OVF_vect)
{
	led_timer();
	TCNT0 = 0xFF - 100; // Overflow again after 100 ticks = 100 us
}

// run once every 65000 us = 65 ms
ISR(TIMER1_OVF_vect)
{
	button_timer();
}


// Define interupt so we don't reset on bad interupt
ISR(BADISR_vect)
{
}

void processTWI( void )
{
	uint8_t b,p1,p2;

	b = usiTwiReceiveByte();

	switch (b) {
	case 0x81: // set slave address
		p1 = usiTwiReceiveByte();
		if(p1 < 128) // Address is 7 bit
		{
			eeprom_update_byte(&b_slave_address, p1);
			usiTwiSlaveInit(eeprom_read_byte(&b_slave_address));
		}
		break;
	case 0x82: // clear led
		led_clear();
		break;
	case 0x83: // set led brightness, p1=led, p2=brightness
		p1 = usiTwiReceiveByte();
		p2 = usiTwiReceiveByte();
		set_led_pulse(p1,0); // Turn off pulsing
		set_brightness(p1,p2);
		break;
	case 0x84: // Set to pulse led, p1=led, p2 = (0 = OFF, 1 = ON)
		set_led_pulse(usiTwiReceiveByte(),usiTwiReceiveByte());
		break;
	case 0x85: // Dim up/down led to specific value, p1=led, p2=value to dim to
		dim_led(usiTwiReceiveByte(),usiTwiReceiveByte());
		break;
	case 0x90: // get keyUp Event
		usiTwiTransmitByte(getKeyUp());
		break;
	case 0x91: // get KeyDown Event
		usiTwiTransmitByte(getKeyDown());
		break;
	case 0xFE: // reset to known state
		led_clear();
		flushTwiBuffers();
		break;
	case 0xFF: // flush the bus
		break;
	default:
		break;
	}
}

void main(void) __attribute__ ((noreturn));

#ifndef DEMO
void main(void)
{
	init();
	while (1) {
		while (usiTwiDataInReceiveBuffer())	{ // process I2C command
			processTWI();
		}
	}
}
#else
void main(void)
{
	init();

	// for testing
	set_brightness(LED1, 2);
	set_led_pulse(LED2, 1);
	//set_brightness(LED2, 50);
	//set_brightness(LED3, 100);
	uint8_t keyup = 0, keydown = 0;
	while (1)
	{
		keydown = getKeyDown();
		if (keydown)
		{
			if (keydown & _BV(BUTTON1))
				set_brightness(LED3, 100);
			else if (keydown & _BV(BUTTON2))
				set_brightness(LED3, 60);
			else if (keydown & _BV(BUTTON3))
				dim_led(LED3, 90);
			else if (keydown & _BV(BUTTON4))
				dim_led(LED3, 0);
			else if (keydown & _BV(BUTTON5))
				set_brightness(LED3, 0);
		}
		keyup = getKeyUp();
		if (keyup)
		{
			if (keyup & _BV(BUTTON2))
				set_brightness(LED3, 0);
		}
	}
}
#endif // DEMO
