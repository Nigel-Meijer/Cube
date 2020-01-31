/*
 * demo.c
 *
 *  Created on: Dec 3, 2018
 *      Author: Gebruiker
 */

#include "demo.h"
#include "stdlib.h"

uint8_t DemoID = 0;

void Demo_Run()
{
	DemoState state = BUSY;

	switch (DemoID) {
		case 0:
			state = Demo_Pulse(RED);
			break;
		case 1:
			state = Demo_Pulse(GREEN);
			break;
		case 2:
			state = Demo_Pulse(BLUE);
			break;
		case 3:
			state = Demo_Pulse(YELLOW);
			break;
		case 4:
			state = Demo_Pulse(WHITE);
			break;
		case 5:
			state = Demo_Pulse(LIGHTBLUE);
			break;
		case 6:
			state = Demo_Pulse(PURPLE);
			break;
		default:
			break;
	}

	if(state == DONE)
	{
		DemoID = rand() % (6 + 1);
		//DemoID = 3;
	}
}
