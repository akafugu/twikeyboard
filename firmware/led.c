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
#include "led.h"

uint8_t brightness[5] = {0, 0, 0, 0, 0};
uint8_t set_value[5] = {0, 0, 0, 0, 0};

LED_STATE_VALUE led_state[5] = { LED_STOP, LED_STOP, LED_STOP, LED_STOP, LED_STOP };

// For led pulsing
uint16_t counter = 0;
uint8_t pulse_min = 5;
uint8_t pulse_max = 100;
#define pulse_length	150

void led_init(void)
{
	// set digits as output
	sbi(LED1_DDR, LED1_BIT);
	sbi(LED2_DDR, LED2_BIT);
	sbi(LED3_DDR, LED3_BIT);
	sbi(LED4_DDR, LED4_BIT);
	sbi(LED5_DDR, LED5_BIT);
	
	led_clear();
}

void led_clear(void)
{
	for(uint8_t i = LED1; i <= LED3; i++)
	{
		brightness[i] = 0;
		set_value[i] = 0;
		led_state[i] = LED_STOP;
	}
	sbi(LED1_PORT, LED1_BIT);
	sbi(LED2_PORT, LED2_BIT);
	sbi(LED3_PORT, LED3_BIT);
	sbi(LED4_PORT, LED4_BIT);
	sbi(LED5_PORT, LED5_BIT);
}

void set_brightness(uint8_t led, uint8_t b)
{
	if(led > MAX_LED) return;
	if(b > 100) b = 100;
	brightness[led] = set_value[led] = b;
	led_state[led] = LED_STOP;
}

uint8_t us_counter = 0; // microsecond counter
uint8_t multiplex_counter = 0;

// display multiplexing routine: run once every 5us
void led_timer(void)
{
	if(multiplex_counter > brightness[LED1])
		sbi(LED1_PORT, LED1_BIT); // Turn off Led1
	if(multiplex_counter > brightness[LED2])
		sbi(LED2_PORT, LED2_BIT); // Turn off Led2
	if(multiplex_counter > brightness[LED3])
		sbi(LED3_PORT, LED3_BIT); // Turn off Led3
	if(multiplex_counter > brightness[LED4])
		sbi(LED4_PORT, LED4_BIT); // Turn off Led4
	if(multiplex_counter > brightness[LED5])
		sbi(LED5_PORT, LED5_BIT); // Turn off Led5
		
	// brightness setting: 5~100
	if (multiplex_counter == 100)
	{
		// Reset everything.
		multiplex_counter = 0;
		if(brightness[LED1])
			cbi(LED1_PORT, LED1_BIT); // Turn on Led1
		if(brightness[LED2])
			cbi(LED2_PORT, LED2_BIT); // Turn on Led2
		if(brightness[LED3])
			cbi(LED3_PORT, LED3_BIT); // Turn on Led3
		if(brightness[LED4])
			cbi(LED4_PORT, LED4_BIT); // Turn on Led4
		if(brightness[LED5])
			cbi(LED5_PORT, LED5_BIT); // Turn on Led5
	}
	else
		multiplex_counter++;
	
	// Led puslsing
	if(counter <= pulse_length)
	{
		// Not time yet
		counter++;
	}
	else
	{
		counter = 0;
		for(uint8_t led = LED1; led <= MAX_LED; led++)
		{
			if(led_state[led] == LED_DIM_DOWN || led_state[led] == LED_DIM_UP)
			{
				if(brightness[led] == set_value[led])
					led_state[led] = LED_STOP;
				else
				{
					brightness[led] += (led_state[led] == LED_DIM_DOWN ? -1 : 1);
				}
			}
			else if(led_state[led] == LED_PULSE_DOWN || led_state[led] == LED_PULSE_UP)
			{
				if((brightness[led] >= pulse_max && (led_state[led] == LED_PULSE_UP)) || 
						(brightness[led] <= pulse_min && (led_state[led] == LED_PULSE_DOWN)))
					led_state[led] = (led_state[led] == LED_PULSE_DOWN ? LED_PULSE_UP : LED_PULSE_DOWN);
				else
				{
					brightness[led] += (led_state[led] == LED_PULSE_DOWN ? -1 : 1);
				}
			}
		}
	}
}

void set_led_pulse(uint8_t led, uint8_t on)
{
	if(led > MAX_LED) return;
	
	led_state[led] = (on ? LED_PULSE_UP : LED_STOP);
}

void dim_led(uint8_t led, uint8_t b)
{
	if(led > MAX_LED) return;
	if(b > 100) b = 100;
	if(b > brightness[led])
	{
		led_state[led] = LED_DIM_UP;
		set_value[led] = b;
	}
	else if(b < brightness[led])
	{
		led_state[led] = LED_DIM_DOWN;
		set_value[led] = b;
	}
}
