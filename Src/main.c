#include "stm32f4xx.h"
#include "../Utils/delay.h"
#include "../Drivers/SPI/Spi1-CubeHandler.h"
#include "../Cube/CubeHandler.h"
#include "../Drivers/Controller/nes.h"
#include "../Settings/Settings.h"
#include "../StateMachine/StateMachine.h"

int main(void)
{
	// set systick to millisecond
  	SysTick_Config(SystemCoreClock/1000);

 	// Init Frame buffers
 	settings.DisplayFrameBuffer = 0;
 	settings.WriteFrameBuffer = 1;

 	// Init settings
 	settings.debugMode = 0;
 	settings.PrevUpdateTick = 0;

  	// Init Cube, Clear buffers
	SwitchBuffer(CLEAR);
	SwitchBuffer(CLEAR);

  	ControllerInit();
  	CubeHandlerInit();

  	State_StateMachineInit();

	while (1)
	{
		State_StateMachine();

		// Debug data:
		if(settings.debugMode)
		{
			DisplayController(0);
			DisplayController(1);
			DisplayController(2);
			DisplayController(3);
			if(IsControllerConnected(0))
				WritePixel(0,0,0,RED,15);
			if(IsControllerConnected(1))
				WritePixel(1,0,0,RED,15);
			if(IsControllerConnected(2))
				WritePixel(2,0,0,RED,15);
			if(IsControllerConnected(3))
				WritePixel(3,0,0,RED,15);
		}
		SwitchBuffer(CLEAR);

		// Game is delay independent. Demo isnt.
		DelayInMs(25);
	}
}

