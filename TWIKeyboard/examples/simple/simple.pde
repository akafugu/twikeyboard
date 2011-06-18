/*
 * TWIKeyboard: Arduino Library for Akafugu TWI/I2C Keyboard and LED controller
 * Simple demo: 
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

#include <Wire.h>
#include <TWIKeyboard.h>

#define SLAVE_ADDR 0x10

TWIKeyboard keyb(SLAVE_ADDR);

void setup()
{
  Wire.begin();
  
  Serial.begin(9600);
  Serial.println("Keyboard Test");

  keyb.reset();
  keyb.setLed(LED1, 100);
  keyb.pulseLed(LED2, 1);
  keyb.setKeyRepeat(BUTTON1, KEYREPEAT_MEDIUM);
  keyb.setKeyRepeat(BUTTON2, KEYREPEAT_FAST);
}

void loop()
{
  uint8_t keydown = keyb.getKeyDown();
  if (keydown)
  {
    Serial.print("Keydown event: 0b");
    Serial.println(keydown, BIN);
    if (keydown & _BV(BUTTON1))
      keyb.setLed(LED3, 100);
    else if(keydown & _BV(BUTTON2))
      keyb.setLed(LED3, 60);
    else if(keydown & _BV(BUTTON3))
      keyb.dimLed(LED3, 90);
    else if(keydown & _BV(BUTTON4))
      keyb.dimLed(LED3, 0);
    else if(keydown & _BV(BUTTON5))
      keyb.setLed(LED3, 0);
  }
  
  uint8_t keyup = keyb.getKeyUp();
  if (keyup)
  {
    Serial.print("Keyup event: 0b");
    Serial.println(keyup, BIN);
    if (keyup & _BV(BUTTON1))
      keyb.setLed(LED3, 0);
  }
}

