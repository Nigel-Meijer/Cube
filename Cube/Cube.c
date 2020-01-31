/*
 * Cube.c
 *
 *  Created on: Dec 3, 2018
 *      Author: Gebruiker
 */

#include "Cube.h"

void DisplayController(uint8_t controllerID)
{
	// TODO isbuttonpressed
	if(GetController(controllerID)->A){
		WritePixel(0,7-controllerID,0,WHITE,8);
	}
	if(GetController(controllerID)->B){
		WritePixel(1,7-controllerID,0,WHITE,8);
	}
	if(GetController(controllerID)->Start){
		WritePixel(2,7-controllerID,0,WHITE,8);
	}
	if(GetController(controllerID)->Select){
		WritePixel(3,7-controllerID,0,WHITE,8);
	}
	if(GetController(controllerID)->Up){
		WritePixel(4,7-controllerID,0,WHITE,8);
	}
	if(GetController(controllerID)->Down){
		WritePixel(5,7-controllerID,0,WHITE,8);
	}
	if(GetController(controllerID)->Left){
		WritePixel(6,7-controllerID,0,WHITE,8);
	}
	if(GetController(controllerID)->Right){
		WritePixel(7,7-controllerID,0,WHITE,8);
	}
}

void SwitchBuffer(BufferSwitchCommand command)
{
	// First switch the buffers
	uint8_t temp = settings.DisplayFrameBuffer;
	settings.DisplayFrameBuffer =settings.WriteFrameBuffer;
	settings.WriteFrameBuffer = temp;

	switch (command) {
		case CLEAR:

			ClearBuffer();
			break;
		case COPY:

			///Buffer[settings.WriteFrameBuffer] = Buffer[settings.DisplayFrameBuffer];
			// TODO
			memcpy(&Buffer[settings.WriteFrameBuffer], &Buffer[settings.DisplayFrameBuffer] , sizeof(Buffer[settings.DisplayFrameBuffer]));
			break;
		default:
			break;
	}
}

DemoState Demo_Pulse(Color color)
{
	static uint8_t brightness = 0;
	static uint8_t direction = 0;

	DemoState state = BUSY;

	if (direction == 0)
	{
		brightness++;
		if(brightness > 31-1)
			direction = 1;
	}
	else
	{
		brightness--;
		if(brightness == 0)
		{
			direction = 0;
			state = DONE;
		}
	}

	for (uint8_t x = 0; x < 8; ++x) {
	  for (uint8_t y = 0; y < 8; ++y) {
		  for (uint8_t z = 0; z < 8; ++z) {
			  WritePixel(x,y,z,color,brightness);
		  }
	  }
	}

	return state;
}

void Demo_Rain()
{

}



void Demo_Sweep_In_From(Direction side, Color color)
{
	static uint8_t step = 0;
	switch (side) {
		case DIR_UP:
			for (uint8_t y = 0; y < 8; ++y) {
				for (uint8_t x = 0; x < 8; ++x) {
					for (uint8_t z = 0; z < 8; ++z) {
						WritePixel(x,y,z,RED,15);
					}
				}
				DelayInMs(100);
			}
			break;
		default:
			break;
	}
}

void WritePixel(uint8_t x, uint8_t y, uint8_t z, Color color, uint8_t brightness)
{
	if(x > 7) return;
	if(y > 7) return;
	if(z > 7) return;
	if(brightness > 31) return;

	uint8_t Z1 = (0x01 << z);
	uint8_t column = y*8+x;

	if((color & RED) == RED)
	{
		for (uint8_t i = 0; i < 5; ++i) {
			if((1 << i) == (brightness & (1 << i)))
			{
				Buffer[settings.WriteFrameBuffer].cubeBuffer[i][0][column] |= Z1;
			}
		}
	}
	if((color & GREEN) == GREEN)
	{
		for (uint8_t i = 0; i < 5; ++i) {
			if((1 << i) == (brightness & (1 << i)))
			{
				Buffer[settings.WriteFrameBuffer].cubeBuffer[i][1][column] |= Z1;
			}
		}
	}
	if((color & BLUE) == BLUE)
	{
		for (uint8_t i = 0; i < 5; ++i) {
			if((1 << i) == (brightness & (1 << i)))
			{
				Buffer[settings.WriteFrameBuffer].cubeBuffer[i][2][column] |= Z1;
			}
		}
	}
}

void ClearBuffer()
{
	for(int buffer = 0; buffer < 5; buffer++)
	{
		for(int i = 0; i < BUFFER_SIZE; i++)
		{
			Buffer[settings.WriteFrameBuffer].cubeBuffer[buffer][0][i] = 0x00;
			Buffer[settings.WriteFrameBuffer].cubeBuffer[buffer][1][i] = 0x00;
			Buffer[settings.WriteFrameBuffer].cubeBuffer[buffer][2][i] = 0x00;
		}
	}
}
