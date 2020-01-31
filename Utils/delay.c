/*
 * delay.c
 *
 *  Created on: Nov 13, 2018
 *      Author: Gebruiker
 */

#include "delay.h"

extern uint32_t delayCounter;

void DelayInMs(uint32_t ms)
{
	delayCounter = ms;
	while(delayCounter !=0);
}
