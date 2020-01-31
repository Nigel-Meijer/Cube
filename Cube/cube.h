/*
 * cube.h
 *
 *  Created on: Dec 3, 2018
 *      Author: Gebruiker
 */

#ifndef CUBE_CUBE_H_
#define CUBE_CUBE_H_

#include "stm32f4xx.h"
#include "../Utils/delay.h"
#include "../Settings/Settings.h"
#include "../Controller/nes.h"
#include "string.h"

void ClearBuffer();
void SwitchBuffer(BufferSwitchCommand command);

void WritePixel(uint8_t x, uint8_t y, uint8_t z, Color color, uint8_t brightness);
void DisplayController(uint8_t controllerID);

void Demo_AllColors();
DemoState Demo_Pulse(Color color);

#endif /* CUBE_CUBE_H_ */
