/*
 * Settings.h
 *
 *  Created on: Dec 3, 2018
 *      Author: Gebruiker
 */

#ifndef SETTINGS_SETTINGS_H_
#define SETTINGS_SETTINGS_H_

#include "stm32f4xx.h"

#define FALSE 					0
#define TRUE 					!(FALSE)
#define NULL					0

#define BUFFER_SIZE  			8 * 8
#define NUMBER_OF_CONTROLLERS  	4

struct cubeFrameBuffer
{
	uint8_t cubeBuffer[5][3][BUFFER_SIZE];
};

typedef enum{
	MODE_SNAKE,
	MODE_TRON,
	MODE_DEMO,
	MODE_MENU
} Mode;

struct Settings{
	uint8_t DisplayFrameBuffer;
	uint8_t WriteFrameBuffer;
	uint32_t PrevUpdateTick;
	uint8_t debugMode;
};

struct cubeFrameBuffer Buffer[2];
struct Settings settings;

typedef enum Direction{
	DIR_UP,
	DIR_DOWN,
	DIR_NORTH,
	DIR_EAST,
	DIR_SOUTH,
	DIR_WEST
} Direction;

typedef enum {
	BUSY,
	DONE
} DemoState;

typedef enum{
	NONE =		0x00,
	BLUE = 		0x01,
	GREEN = 	0x02,
	LIGHTBLUE = 0x03, // green blue
	RED = 		0x04,
	PURPLE =	0x05, // Red blue
	YELLOW = 	0x06, // green red
	WHITE =		0x07
} Color;

typedef enum{
	CLEAR =		0x00,
	COPY = 		0x01
} BufferSwitchCommand;


#endif /* SETTINGS_SETTINGS_H_ */
