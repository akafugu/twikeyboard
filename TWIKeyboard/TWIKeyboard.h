/*
 * TWIKeyboard: Arduino Library for Akafugu TWI/I2C Keyboard and LED controller
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

#ifndef TWIKEYBOARD_H
#define TWIKEYBOARD_H

#include <WProgram.h>
#include <../Wire/Wire.h>

#define BUTTON1 0
#define BUTTON2 1
#define BUTTON3 2
#define BUTTON4 3
#define BUTTON5 4
#define BUTTON6 5
#define BUTTON7 6

#define LED1 0
#define LED2 1
#define LED3 2
#define LED4 3
#define LED5 4

#define KEYREPEAT_STOP 0
#define KEYREPEAT_SLOW 1
#define KEYREPEAT_MEDIUMSLOW 2
#define KEYREPEAT_MEDIUM 3
#define KEYREPEAT_MEDIUMFAST 4
#define KEYREPEAT_FAST 5

class TWIKeyboard
{
public:
  TWIKeyboard(int addr);

  void changeAddress(int new_addr);
  void clear();
  void reset(void);
  
  void setLed(uint8_t nr, uint8_t brightness);
  void pulseLed(uint8_t nr, bool on);
  void dimLed(uint8_t nr, uint8_t brightness);

  void setKeyRepeat(uint8_t button, uint8_t mode);
  uint8_t getKeyUp();
  uint8_t getKeyDown();
  
private:
  int m_addr;
};

#endif // TWIKEYBOARD_H
