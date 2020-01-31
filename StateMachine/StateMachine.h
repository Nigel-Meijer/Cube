/*
 * StateMachine.h
 *
 *  Created on: Dec 12, 2018
 *      Author: Gebruiker
 */

#ifndef STATEMACHINE_STATEMACHINE_H_
#define STATEMACHINE_STATEMACHINE_H_

#include "stm32f4xx.h"
#include "../Settings/Settings.h"
#include "../Snake/Snake.h"
#include "../Demo/demo.h"

typedef enum{
	STATE_SNAKE,
	STATE_TRON,
	STATE_DEMO
} States;

typedef struct{
	States StateCurrent;
	//States StatePrevious;
} StateMachine;


void State_StateMachineInit();
void State_Switch(States state);
States State_GetCurrentState();

void State_Logic();
void State_Execute();

void State_StateMachine();

#endif /* STATEMACHINE_STATEMACHINE_H_ */
