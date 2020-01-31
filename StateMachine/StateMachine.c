/*
 * StateMachine.c
 *
 *  Created on: Dec 12, 2018
 *      Author: Gebruiker
 */


#include "StateMachine.h"

StateMachine stateMachine;

void State_StateMachineInit()
{
	stateMachine.StateCurrent = STATE_DEMO;
	//stateMachine.StatePrevious = STATE_DEMO;
}

void State_Switch(States state)
{
	switch (state) {
		case STATE_DEMO:
			stateMachine.StateCurrent = STATE_DEMO;
			//DemoInit(); ?
			break;
		case STATE_SNAKE:
			stateMachine.StateCurrent = STATE_SNAKE;
			SnakeInit();
			break;
		case STATE_TRON:
			stateMachine.StateCurrent = STATE_TRON;
			SnakeInit();
			break;
		default:
			break;
	}

	// Make sure the last pressed button is cleared after a switch to prevent double actions.
	for (uint8_t playerID = 0; playerID < NUMBER_OF_CONTROLLERS; ++playerID) {
		LastPressedButtonClear(playerID);
	}
}

States State_GetCurrentState()
{
	return stateMachine.StateCurrent;
}

// Used to switch states.
void State_Logic()
{
	switch (stateMachine.StateCurrent) {
		case STATE_DEMO:
			for (uint8_t playerID = 0; playerID < NUMBER_OF_CONTROLLERS; ++playerID) {
				if(GetLastPressedButtonWithoutClear(playerID) == BUTTON_A)
				{
					State_Switch(STATE_SNAKE);
				}
				if(GetLastPressedButtonWithoutClear(playerID) == BUTTON_B)
				{
					State_Switch(STATE_TRON);
				}
			}
			break;
		case STATE_SNAKE:
			for (uint8_t playerID = 0; playerID < NUMBER_OF_CONTROLLERS; ++playerID) {
				if(GetLastPressedButtonWithoutClear(playerID) == BUTTON_START)
				{
					State_Switch(State_GetCurrentState());
				}

				if(GetLastPressedButtonWithoutClear(playerID) == BUTTON_SELECT)
				{
					State_Switch(STATE_DEMO);
				}
			}
			break;
		case STATE_TRON:
			for (uint8_t playerID = 0; playerID < NUMBER_OF_CONTROLLERS; ++playerID) {
				if(GetLastPressedButtonWithoutClear(playerID) == BUTTON_START)
				{
					State_Switch(State_GetCurrentState());
				}

				if(GetLastPressedButtonWithoutClear(playerID) == BUTTON_SELECT)
				{
					State_Switch(STATE_DEMO);
				}
			}
			break;
		default:
			break;
	}
}

// Used to execute states.
void State_Execute()
{
	switch (stateMachine.StateCurrent) {
		case STATE_DEMO:
			Demo_Run();
			break;
		case STATE_SNAKE:
			SnakeLoop();
			break;
		case STATE_TRON:
			SnakeLoop();
			break;
		default:
			break;
	}
}

void State_StateMachine()
{
	State_Logic();
	State_Execute();
}
